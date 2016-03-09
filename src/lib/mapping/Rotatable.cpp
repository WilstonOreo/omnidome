/* Copyright (c) 2014-2015 "Omnidome" by cr8tr
 * Dome Mapping Projection Software (http://omnido.me).
 * Omnidome was created by Michael Winkelmann aka Wilston Oreo (@WilstonOreo)
 *
 * This file is part of Omnidome.
 *
 * Omnidome is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

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
