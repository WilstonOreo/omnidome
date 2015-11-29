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

#ifndef OMNI_CANVAS_ENVELOPE_H_
#define OMNI_CANVAS_ENVELOPE_H_

#include <omni/canvas/Interface.h>

namespace omni
{
  namespace canvas
  {
    /// An enveloped canvas can enclose the viewer and can be projected from inside or outside
    class Envelope :
      public Canvas
    {
    public:
      Envelope();
      ~Envelope();

      /// Projector mode flag determine if projectors are placed in- or outside
      enum ProjectionMode
      {
        PROJECT_INSIDE,
        PROJECT_OUTSIDE,
        PROJECT_BOTH_SIDES
      };

      ProjectionMode projectionMode() const;
      void setProjectionMode(ProjectionMode);

      /// Returns copy of cached bounding box
      Box bounds() const;

      virtual void draw() const;

      inline MappingModeSet supportedMappingModes() const {
          return MappingModeSet({
              MappingMode::FISHEYE,
              MappingMode::EQUIRECTANGULAR,
              MappingMode::CUBEMAP });
      }
      
    protected:

      /// Protected bounding box member
      omni::Box bounds_;

    private:
      ProjectionMode projectionMode_ = PROJECT_INSIDE;
    };
  }
}

#endif /* OMNI_CANVAS_ENVELOPE_H_ */
