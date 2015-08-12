#include <omni/mapping/Equirectangular.h>

#include <QDataStream>
#include <QOpenGLShaderProgram>
#include <omni/util.h>

namespace omni
{
  namespace mapping
  {
    Equirectangular::Equirectangular()
    {
    }

    Equirectangular::~Equirectangular()
    {
    }

    Mode Equirectangular::mode() const
    {
      return Mode::EQUIRECTANGULAR;
    }

    void Equirectangular::bind(OutputMode _outputMode)     
    {
      Rotatable::bind(_outputMode);
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
      
    bool operator==(Equirectangular const& _lhs, Equirectangular const& _rhs)
    {
      const Rotatable& _blhs(_lhs);
      const Rotatable& _brhs(_rhs);
      return (_blhs == _brhs) && 
        OMNI_TEST_MEMBER_EQUAL(stripTop_) &&
        OMNI_TEST_MEMBER_EQUAL(stripBottom_);
    }
  }
}
