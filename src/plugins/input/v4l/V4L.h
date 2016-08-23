
#ifndef OMNI_INPUT_V4L_H_
#define OMNI_INPUT_V4L_H_

#include <omni/input/Interface.h>
#include <omni/media/Location.h>

#include "V4LDevice.h"

namespace omni {
  namespace input {
    /// Still image input
    class V4L : public QObject, public Interface {
      OMNI_INPUT_PLUGIN_DECL
      //OMNI_PLUGIN_CR8TR("V4L", "Copyright (C) 2016")


      OMNI_PLUGIN_NAME("V4L Input")
      OMNI_PLUGIN_VERSION(0, 1, 0, 0)
      OMNI_PLUGIN_DESCRIPTION("Video for Linux Input")
      OMNI_PLUGIN_AUTHOR("Niklas Schulze")
      OMNI_PLUGIN_URL("")


      public:
        OMNI_REGISTER_CLASS(Factory, V4L)

        /// Default Constructor
        V4L(Interface const * = nullptr);

        /// Destructor
        ~V4L();

        /// Update image
        void     update();

        QStringList GetDevices();

        /// Returns texture ID of image
        GLuint   textureId() const;

        /// Load image from file and stores path
        bool     load(QString const& _path);

        /// InfoText is basename of stored path
        QString  infoText() const;

        /// Return image dimensions in pixels
        QSize    size() const;

        /// Serialize image path to stream
        void     toPropertyMap(PropertyMap&) const;

        /// Deserialize from stream and load image
        void     fromPropertyMap(PropertyMap const&);

        bool     canAdd();

     public slots:
     //void onCaptureStarted();
     void onFrameArrived(const V4LFrame&);
     //void onResolutionChanged(const QSize&);

      private:
        std::unique_ptr<QOpenGLTexture> texture_;

        std::unique_ptr<V4LDevice> device_;

        QSize size_;
    };
  }
}

#endif /* OMNI_INPUT_V4L_H_ */
