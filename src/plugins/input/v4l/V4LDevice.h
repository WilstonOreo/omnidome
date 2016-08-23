#ifndef OMNI_INPUT_V4L_DEVICE_H_
#define OMNI_INPUT_V4L_DEVICE_H_

#include <QObject>
#include <QSocketNotifier>
#include <QSize>

extern "C" {
  #include <libavutil/frame.h>
  #include <libavutil/imgutils.h>
  #include <libswscale/swscale.h>
}


namespace omni {
  namespace input {

    typedef struct {
      void   *start;
      size_t  length;
    } buffer_t;

    typedef struct {
      AVFrame* rgbFrame;
      int bufNdx_;
    } V4LFrame;

    class V4LDevice : public QObject {
      Q_OBJECT

    private:
      const QString& dev_;
      int fd_;
      buffer_t *buffers_;
      size_t nBuffers_;
      QSocketNotifier* notifier_;

      SwsContext* swsCtx_;
      AVFrame* srcFrame;
      AVFrame* rgbFrame;

      bool setupBuffers();
      bool start();
      bool readFrame(V4LFrame*);


    public:
      V4LDevice(const QString& dev);
      ~V4LDevice();

      bool open();
      int fd();
      void releaseFrame(const V4LFrame& frame);

      signals:
      //void captureStarted();
      void frameArrived(const V4LFrame& frame);
      //void resolutionChanged(const QSize& size);

      private slots:
      void readyRead();

    };
  }
}

#endif
