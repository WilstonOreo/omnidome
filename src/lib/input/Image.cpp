#include <omni/input/Image.h>

#include <QOpenGLContext>
#include <QFileInfo>

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

    void Image::free()
    {
      texture_.reset();
    }

    void Image::update()
    {
      if (!QOpenGLContext::currentContext() || !needsUpdate_ || image_.width() == 0) return;
     

      texture_.reset(new QOpenGLTexture(image_));
    
      texture_->setMinMagFilters(
          QOpenGLTexture::Linear,
          QOpenGLTexture::Linear);
      
      needsUpdate_ = false;
    }

    GLuint Image::textureId() const
    {
      return !texture_ ? 0 : texture_->textureId();
    }

    void Image::load(QString const& _path)
    {
      image_ = QImage(_path).mirrored();
      path_=_path;
      needsUpdate_ = true;
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

    QString Image::infoText() const
    {
      QFileInfo fi(path_);
      return fi.baseName();
    }

    QSize Image::size() const
    {
      return image_.size();
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
