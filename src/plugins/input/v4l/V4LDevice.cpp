#include "V4LDevice.h"

#include <QDebug>
#include <fcntl.h>
#include <libv4l2.h>
#include <linux/videodev2.h>
#include <sys/mman.h>
#include <sys/ioctl.h>


namespace omni {
  namespace input {

    static enum AVPixelFormat v4l2_pixfmt_to_libav_pixfmt(int fmt) {
      switch(fmt) {
        case V4L2_PIX_FMT_RGB24:           return AV_PIX_FMT_RGB24; break;
        case V4L2_PIX_FMT_YUYV:            return AV_PIX_FMT_YUYV422; break;
        case V4L2_PIX_FMT_YUV420:          return AV_PIX_FMT_YUV420P; break;
        case V4L2_PIX_FMT_YUV422P:         return AV_PIX_FMT_YUV422P; break;
        default: return AV_PIX_FMT_NONE;
      }
    }

    static int xioctl(int fh, int request, void *arg) {
  	        int r;
  	        do {
              r = ioctl(fh, request, arg);
  	        } while (-1 == r && EINTR == errno);
  	        return r;
  	}

    V4LDevice::V4LDevice(const QString& dev) : dev_(dev),
      notifier_(nullptr),
      srcFrame(nullptr),
      rgbFrame(nullptr) {
    }

    V4LDevice::~V4LDevice() {

      if(fd_)
        v4l2_close(fd_);

      if(srcFrame) {
        av_frame_free(&srcFrame);
      }

      if(rgbFrame) {
        if(rgbFrame->data[0])
          free(rgbFrame->data[0]);
        av_frame_free(&rgbFrame);
      }
    }

    bool V4LDevice::open() {

      struct v4l2_capability cap;
      struct v4l2_cropcap cropcap;
      struct v4l2_crop crop;
      struct v4l2_format fmt;

  		QByteArray dev = dev_.toLocal8Bit();

  		fd_ = v4l2_open(dev.data(), O_RDONLY | O_NONBLOCK, 0);
      if(!fd_) {
        qDebug() << "Opening " << dev_ << " failed";
        return false;
      }

      qDebug() << "Success opening device " << dev;

      if (-1 == xioctl(fd_, VIDIOC_QUERYCAP, &cap)) {
        if (EINVAL == errno) {
          qDebug() << "No v4l device!";
        } else {
          qDebug() << "VIDIOC_QUERYCAP failed";
        }

        return false;
      }

      if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
        qDebug() << "is no capture device!";
        return false;
      }

      if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
        qDebug() << "doesnt support streaming";
        return false;
      }

      memset(&cropcap, 0, sizeof(cropcap));
      cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

      if (0 == xioctl(fd_, VIDIOC_CROPCAP, &cropcap)) {
        crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        crop.c = cropcap.defrect; /* reset to default */

        if (-1 == xioctl(fd_, VIDIOC_S_CROP, &crop)) {
          switch (errno) {
            case EINVAL:
              /* Cropping not supported. */
              break;
            default:
              /* Errors ignored. */
              break;
            }
          }
        } else {
                /* Errors ignored. */
        }

        memset(&fmt, 0, sizeof(fmt));
        fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

        if(xioctl(fd_, VIDIOC_G_FMT, &fmt) != 0) {
        	qDebug() << "Getting fmt failed";
        	return false;
        }

        qDebug() << "Got size: " << fmt.fmt.pix.width << "x" <<fmt.fmt.pix.height;
        qDebug() << "Got pixt fmt " << fmt.fmt.pix.pixelformat;

        srcFrame = av_frame_alloc();
        srcFrame->format = v4l2_pixfmt_to_libav_pixfmt(fmt.fmt.pix.pixelformat);
        srcFrame->width  = fmt.fmt.pix.width;
        srcFrame->height = fmt.fmt.pix.height;

        rgbFrame = av_frame_alloc();
        rgbFrame->format      = AV_PIX_FMT_RGB24;
        rgbFrame->width       = srcFrame->width;
        rgbFrame->height      = srcFrame->height;
        rgbFrame->linesize[0] = rgbFrame->width * 3;
        rgbFrame->data[0]     = (uint8_t*)malloc(rgbFrame->linesize[0] * rgbFrame->height);

        swsCtx_ = sws_getContext(srcFrame->width, srcFrame->height,
          (AVPixelFormat)srcFrame->format, rgbFrame->width, rgbFrame->height,
          (AVPixelFormat)rgbFrame->format, 0, NULL, NULL, NULL);

        if(!setupBuffers())
          return false;

        if(!start())
          return false;

        notifier_ = new QSocketNotifier(fd_, QSocketNotifier::Read);
        connect(notifier_, SIGNAL(activated(int)), this, SLOT(readyRead()));
        notifier_->setEnabled(true);

        return true;
    }

    bool V4LDevice::setupBuffers() {

      struct v4l2_requestbuffers req;

      memset(&req, 0, sizeof(req));

      req.count = 4;
      req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
      req.memory = V4L2_MEMORY_MMAP;

      if (xioctl(fd_, VIDIOC_REQBUFS, &req) != 0) {
        if (EINVAL == errno) {
          qDebug() << "Device doesnt support mmap";
        } else {
          qDebug() << "oh oh";
        }

        return false;
      }

      if (req.count < 2) {
        qDebug() << "Insuffient memory";
        return false;
      }

      buffers_ = (buffer_t*)calloc(req.count, sizeof(*buffers_));

      if (!buffers_) {
        qDebug() << "Out of memory";
        return false;
      }

      for (nBuffers_ = 0; nBuffers_ < req.count; ++nBuffers_) {
        struct v4l2_buffer buf;

        memset(&buf, 0, sizeof(buf));

        buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory      = V4L2_MEMORY_MMAP;
        buf.index       = nBuffers_;

        if (xioctl(fd_, VIDIOC_QUERYBUF, &buf) != 0) {
          qDebug() << "VIDIOC_QUERYBUF";
          return false;
        }

        buffers_[nBuffers_].length = buf.length;
        buffers_[nBuffers_].start =
        v4l2_mmap(NULL /* start anywhere */,
          buf.length,
          PROT_READ /* required */,
          MAP_SHARED /* recommended */,
          fd_, buf.m.offset);

        if (buffers_[nBuffers_].start == MAP_FAILED) {
          qDebug() << "MMAP failed";
          return false;
        }
      }

      return true;
  	}

    bool V4LDevice::start() {

  		enum v4l2_buf_type type;

      for (size_t i = 0; i < nBuffers_; ++i) {
        struct v4l2_buffer buf;

        memset(&buf, 0, sizeof(buf));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;

        if (xioctl(fd_, VIDIOC_QBUF, &buf) != 0) {
          qDebug() << "VIDIOC_QBUF failed";
          return false;
        }
      }

      type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
      if (xioctl(fd_, VIDIOC_STREAMON, &type) != 0) {
        qDebug() << "VIDIOC_STREAMON failed";
        return false;
      }

      return true;
  	}

    bool V4LDevice::readFrame(V4LFrame* frame) {

      struct v4l2_buffer buf;

      memset(&buf, 0, sizeof(buf));

      buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
      buf.memory = V4L2_MEMORY_MMAP;

      if ( xioctl(fd_, VIDIOC_DQBUF, &buf) != 0) {
        switch (errno) {
          case EAGAIN:
          return false;

          case EIO:
            /* Could ignore EIO, see spec. */
            /* fall through */

          default:
            qDebug() << "VIDIOC_DQBUF";
            return false;
          }
      }

      av_image_fill_arrays(srcFrame->data, srcFrame->linesize,
        (uint8_t*)buffers_[buf.index].start, (AVPixelFormat)srcFrame->format,
        srcFrame->width, srcFrame->height, 1);

      sws_scale(swsCtx_, srcFrame->data, srcFrame->linesize, 0,
        srcFrame->height, rgbFrame->data, rgbFrame->linesize);

      frame->rgbFrame = rgbFrame;

      // private stuff
      frame->bufNdx_ = buf.index;

      return true;

  	}

    void V4LDevice::readyRead() {

      V4LFrame frame;

      if(readFrame(&frame))
        emit frameArrived(frame);
    }

    int V4LDevice::fd() {
      return fd_;
    }

    void V4LDevice::releaseFrame(const V4LFrame& frame) {

      struct v4l2_buffer buf;

      memset(&buf, 0, sizeof(buf));

      buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
      buf.memory = V4L2_MEMORY_MMAP;
      buf.index = frame.bufNdx_;

      if ( xioctl(fd_, VIDIOC_QBUF, &buf) != 0) {
        qDebug() << "VIDIOC_QBUF failed";
      }
    }
  }
}
