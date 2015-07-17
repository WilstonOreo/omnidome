#include <omni/mapping/Rotatable.h>

#include <QOpenGLShaderProgram>
#include <omni/util.h>

namespace omni
{
  namespace mapping
  {
    Rotatable::Rotatable() :
      yaw_(0.0),
      pitch_(0.0),
      roll_(0.0)
    {
    }

    Rotatable::~Rotatable()
    {
    }

    void Rotatable::bind()
    {
      Interface::bind();
      this->shader_->setUniformValue("yaw_angle",GLfloat(yaw().degrees()));
      this->shader_->setUniformValue("pitch_angle",GLfloat(pitch().degrees()));
      this->shader_->setUniformValue("roll_angle",GLfloat(roll().degrees()));
    }

    Angle Rotatable::yaw() const
    {
      return yaw_;
    }

    void Rotatable::setYaw(Angle const& _yaw)
    {
      yaw_=_yaw;
    }

    Angle Rotatable::pitch() const
    {
      return pitch_;
    }

    void Rotatable::setPitch(Angle const& _pitch)
    {
      pitch_=_pitch;
    }

    Angle Rotatable::roll() const
    {
      return roll_;
    }

    void Rotatable::setRoll(Angle const& _roll)
    {
      roll_=_roll;
    }

    void Rotatable::fromStream(QDataStream& _stream)
    {
      Interface::fromStream(_stream);
      _stream >> yaw_ >> pitch_ >> roll_;
    }

    void Rotatable::toStream(QDataStream& _stream) const
    {
      Interface::toStream(_stream);
      _stream << yaw_ << pitch_ << roll_;
    }

    bool operator==(Rotatable const& _lhs, Rotatable const& _rhs)
    {
      return
        OMNI_TEST_MEMBER_EQUAL(yaw_) &&
        OMNI_TEST_MEMBER_EQUAL(pitch_) &&
        OMNI_TEST_MEMBER_EQUAL(yaw_);
    }
  }
}


