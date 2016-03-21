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

#ifndef OMNI_CANVAS_PLANAR_H_
#define OMNI_CANVAS_PLANAR_H_

#include <omni/visual/Plane.h>
#include <omni/canvas/Interface.h>
#include <omni/EulerAngles.h>

namespace omni
{
  namespace canvas
  {
    /// A planar canvas
    class Planar : public QObject, public Canvas
    {
        OMNI_CANVAS_PLUGIN_DECL
        OMNI_PLUGIN_CR8TR("Planar canvas","Copyright (C) 2016")
    public:
      OMNI_REGISTER_CLASS(Factory,Planar)

      Planar();
      ~Planar();

      void draw() const;
      void drawAux() const;

      void update();

      float width() const;
      void setWidth(float _width);

      float height() const;
      void setHeight(float _height);

      omni::Box bounds() const;

      void fromStream(QDataStream&);
      void toStream(QDataStream&) const;

      QWidget* widget();

    private:
      float width_ = 1.0, height_ = 1.0;
      visual::Plane plane_;
    };
  }
}


#endif /* OMNI_CANVAS_PLANAR_H_ */
