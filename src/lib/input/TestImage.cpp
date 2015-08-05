#include <omni/input/TestImage.h>

#include <QOpenGLContext>
#include <omni/util.h>

namespace omni
{
  namespace input 
  {
    TestImage::TestImage() :
      pos_(0.0,0.0)
    {
    }

    TestImage::~TestImage()
    {
    }

    void TestImage::bind(mapping::Interface* _mapping)
    {
      if (!shader_) {
        update();
        return;
      }

      shader_->bind();
      shader_->setUniformValue("ruler_pos",GLfloat(pos_.x()),GLfloat(pos_.y()));
    }

    void TestImage::release(mapping::Interface*)
    {
      if (!shader_) return;
      shader_->release();
    }
      
    void TestImage::free()
    {
      shader_.reset();
    }

    QSize TestImage::size() const 
    {
      return QSize(1024,1024);
    }

    void TestImage::setRulerPos(QPointF const& _rulerPos) 
    {
      pos_ = _rulerPos;
      update();
    }

    QPointF TestImage::rulerPos() const
    {
      return pos_;
    }

    void TestImage::update()
    {
      if (!QOpenGLContext::currentContext()) return;

      if (!shader_)
      {
        static QString _vertSrc = util::fileToStr(":/shaders/test_image.vert");
        static QString _fragmentSrc = util::fileToStr(":/shaders/test_image.frag");
        
        shader_.reset(new QOpenGLShaderProgram());
        shader_->addShaderFromSourceCode(QOpenGLShader::Vertex,_vertSrc);
        shader_->addShaderFromSourceCode(QOpenGLShader::Fragment,_fragmentSrc);
        shader_->link();
      }
    }
      
    void TestImage::toStream(QDataStream& _stream) const
    {
      _stream << pos_;
    }

    void TestImage::fromStream(QDataStream& _stream)
    {
      _stream >> pos_;
      update();
    }
  }
}

