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

#ifndef OMNI_PROJ_PROJECTOR_H_
#define OMNI_PROJ_PROJECTOR_H_

#include <memory>
#include <QMatrix4x4>
#include <QScreen>
#include <omni/Angle.h>
#include "Screen.h"
#include "Setup.h"

namespace omni
{
  namespace proj
  {
    class Projector
    {
    public:
      Projector();
      Projector(
          QScreen const* _screen,
          int _subScreenIndex,
          Angle _fov = 45.0);

      /**@brief Make a new projector setup with a certain id and delete old one
       * @return Pointer to new projector setup
       **/
      Setup* setup(Id const& _setupId);

      /// Update projector matrix by current setup and return setup
      Setup* setup();

      /// Returns projector setup (const version)
      Setup const* setup() const;

      qreal aspectRatio() const;
      void setScreen(QScreen const* _screen, int _subScreenIndex);
      QScreen const* screen() const;
      int subScreenIndex() const;

      qreal throwRatio() const;
      void setThrowRatio(qreal);

      Angle fov() const;
      void setFov(Angle _fov);

      QMatrix4x4& matrix();
      QMatrix4x4 const& matrix() const;

      QMatrix4x4 projectionMatrix() const;

      void setMatrix(QMatrix4x4 const&);

      QVector3D pos() const;

      /// Write projector to stream
      void toStream(QDataStream&) const;

      /// Read projector from stream
      void fromStream(QDataStream&);

      friend bool operator==(Projector const&,Projector const&);

    private:
      QMatrix4x4 matrix_;
      QScreen const* screen_ = nullptr;
      int subScreenIndex_ = -1;
      Angle fov_;
      std::unique_ptr<Setup> setup_;
    };
  }

  using proj::Projector;
}

OMNI_DECL_STREAM_OPERATORS(omni::proj::Projector)

#endif /* OMNI_PROJ_PROJECTOR_H_ */
