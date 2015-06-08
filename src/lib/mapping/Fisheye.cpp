#include <omni/mapping/Fisheye.h>

#include <QDataStream>
#include <QOpenGLShaderProgram>

namespace omni
{
  namespace mapping
  {
    Fisheye::Fisheye(input::List const& _inputList) :
      Rotatable(_inputList)
    {
    }

    Fisheye::~Fisheye()
    {
    }

    MappingMode Fisheye::mode() const
    {
      return FISHEYE;
    }

    void Fisheye::bindShader()
    {
      Rotatable::bindShader();
      this->shader_->setUniformValue("stretch",GLfloat(stretch_));
    }

    qreal Fisheye::stretch() const
    {
      return stretch_;
    }

    void Fisheye::setStretch(qreal _stretch)
    {
      if (_stretch <  0.0) _stretch = 0.0;
      if (_stretch >= 1.0) _stretch = 1.0;
      stretch_ = _stretch;
    }

    void Fisheye::fromStream(QDataStream& _stream)
    {
      Rotatable::fromStream(_stream);
      _stream >> stretch_;
      setStretch(stretch_);
    }

    void Fisheye::toStream(QDataStream& _stream) const
    {
      Rotatable::toStream(_stream);
      _stream << stretch();
    }
  }
}
