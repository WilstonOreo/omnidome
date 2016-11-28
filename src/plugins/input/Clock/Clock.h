
#ifndef OMNI_INPUT_CLOCK_H_
#define OMNI_INPUT_CLOCK_H_

#include <QOpenGLShaderProgram>
#include <omni/input/Framebuffer.h>

namespace omni {
  namespace input {
    /// A digital clock as input
    class Clock : public QObject, public Framebuffer {
      OMNI_INPUT_PLUGIN_DECL
      OMNI_PLUGIN_INFO("Clock", "Copyright (C) 2016")
    public:
      OMNI_REGISTER_CLASS(Factory,Clock)

      /// Default Constructor
      Clock();

      ~Clock();

      /// Install update timer on activate
      void     activate();

      /// Remove update timer on deactivate
      void     deactivate();

      /// Update clock shader image
      void     update();

      void     resetCountDown();
      void     setCountDown(int _hour, int _min, int _sec);

      bool     useSystemTime() const;

      void     setUseSystemTime(bool);

      void     pauseCountDown(bool);

      bool     countDownPaused() const;

      /// Return image dimensions in pixels
      QSize    size() const;

      /// Serialize to property map
      void     toPropertyMap(PropertyMap&) const;

      /// Deserialize from property map
      void     fromPropertyMap(PropertyMap const&);

      QTime    countdownTime() const;

      /// Input widget
      QWidget* widget();

      int getMinutes() const;
      int getHours() const;
      int getSeconds() const;

    protected:
      void timerEvent(QTimerEvent*);
    private:
      int getMilliseconds() const;
      double getTime() const;
      double now() const;

      ContextBoundPtr<QOpenGLShaderProgram> shader_;
      bool useSystemTime_ = true;
      bool countDownPaused_ = true;
      double startTime_ = 0.0;
      double countdownTime_ = 0.0;
      double currentCountDownTime_ = 0.0;
      int timerId_ = 0;
    };
  }
}

#endif /* OMNI_INPUT_CLOCK_H_ */
