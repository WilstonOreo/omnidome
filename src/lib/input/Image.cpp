#include <omni/input/Image.h>

#include <QOpenGLContext>

namespace omni
{
  namespace input
  {
    Image::Image()
    {
    }

    Image::Image(QString const& _path)
    {
      load(_path);
    }

    Image::~Image()
    {
    }

    void Image::bind()
    {
      if (!texture_) return;

      glEnable(GL_TEXTURE_2D);
      texture_->bind();
    }

    void Image::release()
    {
      if (!texture_) return;
      texture_->release();
    }
      
    void Image::free()
    {
      texture_.reset();
    }

    void Image::update()
    {
      if (!QOpenGLContext::currentContext()) return;

      texture_.reset(new QOpenGLTexture(image_));
    
      texture_->setMinMagFilters(
          QOpenGLTexture::Linear,
          QOpenGLTexture::Linear);
    }

    GLuint Image::textureId() const
    {
      return !texture_ ? 0 : texture_->textureId();
    }

    void Image::load(QString const& _path)
    {
      image_ = QImage(_path).mirrored();
      path_=_path;
      update();
    }

    void Image::reload()
    {
      if (path_.isEmpty()) return;

      load(path_);
    }
    
    QString Image::path() const
    {
      return path_;
    }
      
    void Image::toStream(QDataStream& _stream) const
    {
      _stream << path_;
    }

    void Image::fromStream(QDataStream& _stream) 
    {
      _stream >> path_;
      reload();
    }
  }
}
