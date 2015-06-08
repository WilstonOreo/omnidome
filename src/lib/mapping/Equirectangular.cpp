#include <omni/mapping/Equirectangular.h>

#include <QDataStream>
#include <QOpenGLShaderProgram>

namespace omni
{
  namespace mapping
  {
    Equirectangular::Equirectangular(input::List const& _inputList) :
      Rotatable(_inputList)
    {
    }

    Equirectangular::~Equirectangular()
    {
    }

    MappingMode Equirectangular::mode() const
    {
      return EQUIRECTANGULAR;
    }

    void Equirectangular::bindShader()     
    {
      Rotatable::bindShader();
      this->shader_->setUniformValue("strip_top",GLfloat(stripTop_));
      this->shader_->setUniformValue("strip_bottom",GLfloat(stripBottom_));
    }

    qreal Equirectangular::stripTop() const
    {
      return stripTop_;
    }

    void Equirectangular::setStripTop(qreal _stripTop)
    {
      stripTop_ = _stripTop;
    }

    qreal Equirectangular::stripBottom() const
    {
      return stripBottom_;
    }

    void Equirectangular::setStripBottom(qreal _stripBottom)
    {
      stripBottom_ = _stripBottom;
    }
      
    void Equirectangular::validate()
    {
      if (stripBottom_ < 0.0) stripBottom_ = 0.0;
      if (stripTop_ > 1.0) stripTop_ = 1.0;
      if (stripBottom_ > stripTop_) std::swap(stripBottom_,stripTop_);
    }

    void Equirectangular::fromStream(QDataStream& _stream)
    {
      Rotatable::fromStream(_stream);
      _stream >> stripBottom_ >> stripTop_;
      validate();
    }

    void Equirectangular::toStream(QDataStream& _stream) const
    {
      Rotatable::toStream(_stream);
      _stream << stripBottom_ << stripTop_;
    }
  }
}
