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

#include <omni/proj/Projector.h>

#include <omni/proj/ScreenSetup.h>
#include <omni/util.h>

namespace omni
{
  namespace proj
  {
    Projector::Projector() :
      screen_(nullptr),
      fov_(45.0)
    {
    }

    Projector::Projector(QScreen const* _screen,
                         int _subScreenIndex,
                         Angle _fov) :
      screen_(_screen),
      subScreenIndex_(_subScreenIndex),
      fov_(_fov)
    {
    }

    QScreen const* Projector::screen() const
    {
      return screen_ ? screen_ : ScreenSetup::standardScreen();
    }

    int Projector::subScreenIndex() const
    {
      return subScreenIndex_;
    }

    void Projector::setScreen(QScreen const* _screen, int _subScreenIndex)
    {
      screen_=_screen;
      subScreenIndex_ = _subScreenIndex;
    }

    Setup* Projector::setup(Id const& _setupId)
    {
      setup_.reset(SetupFactory::create(_setupId));
      return setup_.get();
    }

    Setup* Projector::setup()
    {
      if (!setup_) return nullptr;
      setup_->setup(*this);
      return setup_.get();
    }

    Setup const* Projector::setup() const
    {
      return setup_.get();
    }

    qreal Projector::aspectRatio() const
    {
      return qreal(ScreenSetup::subScreenWidth(screen())) / screen()->size().height();
    }

    qreal Projector::throwRatio() const
    {
      return tan(fov_.radians() / 2.0) * aspectRatio();
    }

    void Projector::setThrowRatio(qreal _throwRatio)
    {
      fov_ = Angle::fromRad(2.0 * atan(_throwRatio  / aspectRatio()));
    }

    void Projector::setFov(Angle _fov)
    {
      fov_ = _fov;
      if (fov_.degrees() < 1.0) fov_ = 1.0;
      if (fov_.degrees() >= 180.0) fov_ = 180.0;
    }

    Angle Projector::fov() const
    {
      return fov_;
    }

    QMatrix4x4& Projector::matrix()
    {
      return matrix_;
    }

    QMatrix4x4 const& Projector::matrix() const
    {
      return matrix_;
    }

    void Projector::setMatrix(QMatrix4x4 const& _matrix)
    {
      matrix_=_matrix;
    }

    QMatrix4x4 Projector::projectionMatrix() const
    {
      QMatrix4x4 _m;
      _m.perspective(fov().degrees(), aspectRatio(), 0.01, 1000.0);
      _m.lookAt(pos(), pos() + matrix().column(0).toVector3D(),matrix().column(2).toVector3D());
      return _m;
    }

    QVector3D Projector::pos() const
    {
      return matrix().column(3).toVector3D();
    }

    bool operator==(Projector const& _lhs,Projector const& _rhs)
    {
      return
        OMNI_TEST_MEMBER_EQUAL(matrix_) &&
        OMNI_TEST_MEMBER_EQUAL(fov_);
    }
  }
}

QDataStream& operator>>(QDataStream& _stream, omni::proj::Projector& _proj)
{
  omni::util::deserializePtr(_stream,[&](omni::Id const& _id) ->
      omni::proj::Setup*
  {
    return _proj.setup(_id);
  });

  omni::Angle _fov(45.0);
  _stream >> _fov;
  _proj.setFov(_fov);
  return _stream;
}

QDataStream& operator<<(QDataStream& _stream, omni::proj::Projector const& _proj)
{
  omni::util::serializePtr(_stream,_proj.setup());
  _stream << _proj.fov();
  return _stream;
}
