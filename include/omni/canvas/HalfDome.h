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

#ifndef OMNI_CANVAS_HALFDOME_H_
#define OMNI_CANVAS_HALFDOME_H_

#include <omni/Vertex.h>
#include <omni/canvas/Dome.h>
#include <omni/EulerAngles.h>

namespace omni
{
  namespace canvas
  {
    /// A rotatable half sphere dome
    class HalfDome : public Dome
    {
    public:
      OMNI_REGISTER_CLASS(Factory,HalfDome)

      HalfDome(qreal _diameter = 10.0);
      ~HalfDome();

      void draw() const;

      EulerAngles& angles();
      EulerAngles const& angles() const;

      void fromStream(QDataStream&);
      void toStream(QDataStream&) const;

      QMatrix4x4 matrix() const;

      inline MappingMode defaultMappingMode() const {
          return MappingMode::FISHEYE;
      }

    private:
      EulerAngles angles_;
    };
  }
}

#endif /* OMNI_CANVAS_HALFDOME_H_ */
