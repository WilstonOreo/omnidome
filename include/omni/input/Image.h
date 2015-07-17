#ifndef OMNI_INPUT_IMAGE_H_
#define OMNI_INPUT_IMAGE_H_

#include "Interface.h"

namespace omni
{
  namespace input
  {
    /// Still image input
    class Image : 
      public Interface,
      private Registrar<Image>
    {
    public:
      OMNI_TYPEID("Image")

      /// Default Constructor
      Image();

      /// Construct from path and load image
      Image(QString const& _path);

      /// Destructor
      ~Image();

      /// Bind image texture to OpenGL context, together with mapping shader
      void bind(mapping::Interface* _mapping);

      /// Release image texture from OpenGL context
      void release(mapping::Interface* _mapping);

      /// Free image from OpenGL context
      void free();

      /// Update image 
      void update();

      /// Returns texture ID of image
      GLuint textureId() const;

      /// Load image from file and stores path
      void load(QString const& _path);
      
      /// Reload image from stored path name
      void reload();

      /// Return stored path
      QString path() const;
   
      /// InfoText is basename of stored path
      QString infoText() const;
     
      /// Serialize image path to stream
      void toStream(QDataStream&) const;
      
      /// Deserialize from stream and load image
      void fromStream(QDataStream&);

    private:
      QImage image_;
      std::unique_ptr<QOpenGLTexture> texture_;
      QString path_;
    };
  }
}

#endif /* OMNI_INPUT_IMAGE_H_ */
