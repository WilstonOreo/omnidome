#include <omni/mapping/Fisheye.h>

#include <QDataStream>
#include <QOpenGLShaderProgram>

namespace omni
{
  namespace mapping
  {
    Fisheye::Fisheye() 
    {
    }

    Fisheye::~Fisheye()
    {
    }

    MappingMode Fisheye::mode() const
    {
      return FISHEYE;
    }

    void Fisheye::bind(OutputMode _outputMode)
    {
      Rotatable::bind(_outputMode);
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
