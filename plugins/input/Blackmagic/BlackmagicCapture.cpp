#include "BlackmagicCapture.h"

#include <sstream>
#include <iostream>

#include <omni/visual/ContextSwitch.h>
#include <QDebug>
#include <QThread>

namespace omni
{
  namespace input
  {
    BlackmagicCapture::BlackmagicCapture(QObject* parent) :
      QObject(parent),
      size_(0,0),
      refCount_(1)
    {
      connect(this,&BlackmagicCapture::frameReceived,
              this,&BlackmagicCapture::update,Qt::QueuedConnection);
      connect(this,&BlackmagicCapture::sizeChanged,
              this,&BlackmagicCapture::setupTexture,Qt::QueuedConnection);
    }

    BlackmagicCapture::~BlackmagicCapture()
    {
      stop();
    }

    void BlackmagicCapture::start()
    {
      if (!isSetup())
      {
        qDebug() << "Is not setup!";
        return;
      }

      if (isCapturing())
      {
        stop();
      }

      HRESULT	result = E_FAIL;
      IDeckLinkInput* inputPtr = nullptr;
      result = device_->QueryInterface(IID_IDeckLinkInput, (void**)&inputPtr);
      BMDDisplayModeSupport			displayModeSupported;

      if (result == S_OK)
      {
        input_.reset(inputPtr);

        result = input_->DoesSupportVideoMode(displayMode_->GetDisplayMode(), pixelFormat_, bmdVideoInputFlagDefault, &displayModeSupported, NULL);
        if (result != S_OK)
        {
          qDebug() << "Video mode is not supported";
          return;
        }

        input_->EnableVideoInput(displayMode_->GetDisplayMode(), pixelFormat_, inputFlags_);
        input_->SetCallback(this);
        input_->StartStreams();
      }
    }

    void BlackmagicCapture::setup(IDeckLink* device, IDeckLinkDisplayMode* displayMode)
    {
      if (!device || !displayMode) return;

      inputFlags_ = bmdVideoInputFlagDefault;

      if (isCapturing())
      {
        stop();
      }

      device_.reset(device);
      displayMode_.reset(displayMode);  
      size_ = QSize(displayMode->GetWidth(),displayMode->GetHeight());
      videoFrameSize_ = QSize(0,0);
      setupTexture(size_);
    }

    void BlackmagicCapture::stop()
    {
      if (input_)
      {
        input_->StopStreams();
        input_->DisableVideoInput();
        input_.reset();
        frameBytes_ = nullptr;
      }
    }

    bool BlackmagicCapture::isSetup() const
    {
      return !(!device_ || !displayMode_);
    }

    bool BlackmagicCapture::isCapturing() const
    {
      return !!input_;
    }

    GLuint BlackmagicCapture::textureId() const
    {
      return textureId_;
    }

    QSize const& BlackmagicCapture::size() const
    {
      return size_;
    }

    ULONG BlackmagicCapture::AddRef(void)
    {
      return refCount_.fetch_add(1);
    }

    ULONG BlackmagicCapture::Release(void)
    {
      int32_t newRefValue = refCount_.fetch_sub(1);
      if (newRefValue == 0)
      {
        delete this;
        return 0;
      }
      return newRefValue;
    }

    HRESULT BlackmagicCapture::VideoInputFrameArrived(
      IDeckLinkVideoInputFrame* videoFrame, IDeckLinkAudioInputPacket*)
    {
      if (!videoFrame) return E_FAIL;

      if (videoFrame->GetFlags() & bmdFrameHasNoInputSource)
      {
        return E_FAIL;
      }

      QMutexLocker lock(&mutex_);
      videoFrame->GetBytes(&frameBytes_);

      if (!frameBytes_) return E_FAIL;

      videoFrameSize_ = QSize(videoFrame->GetWidth(),videoFrame->GetHeight());
      if (videoFrameSize_ != size_) emit sizeChanged(videoFrameSize_);

      emit frameReceived();

      return S_OK;
    }

    void BlackmagicCapture::update()
    {
      QMutexLocker lock(&mutex_);

      if (!textureId_ || size_ != QSize(displayMode_->GetWidth(),displayMode_->GetHeight()))
      {
        setupTexture(videoFrameSize_);
      }

      withCurrentContext([&](QOpenGLFunctions& _)
      {
        _.glBindTexture(GL_TEXTURE_2D, textureId_);
        _.glPixelStorei(GL_UNPACK_ALIGNMENT,   1);
        _.glPixelStorei(GL_UNPACK_ROW_LENGTH,  0);
        _.glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
        _.glPixelStorei(GL_UNPACK_SKIP_ROWS,   0);
        _.glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                          size_.width() / 2, size_.height(), GL_RGBA,
                          GL_UNSIGNED_BYTE, frameBytes_);
        _.glBindTexture(GL_TEXTURE_2D, 0);
      });

      emit updated();
    }

    HRESULT BlackmagicCapture::VideoInputFormatChanged(
      BMDVideoInputFormatChangedEvents events, IDeckLinkDisplayMode *mode,
      BMDDetectedVideoInputFormatFlags formatFlags)
    {
      setup(device_.get(),mode);
      emit sizeChanged(QSize(mode->GetWidth(),mode->GetHeight()));
      return S_OK;
    }

    void BlackmagicCapture::setupTexture(QSize const& size)
    {
      primaryContextSwitch([&](QOpenGLFunctions& _)
      {
        if (textureId_)
        {
          _.glDeleteTextures(1,&textureId_);
          textureId_ = 0;
        }

        size_ = size;
        _.glEnable(GL_TEXTURE_2D);
        _.glGenTextures(1,&textureId_);
        _.glBindTexture(GL_TEXTURE_2D, textureId_);
        _.glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        _.glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        _.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        _.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        _.glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA, size.width() / 2, size.height(), 0,
                       GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        _.glBindTexture(GL_TEXTURE_2D, 0);
      });
    }
  }
}
