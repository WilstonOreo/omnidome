/* Copyright (c) 2014-2017 "Omnidome" by Michael Winkelmann
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

#ifndef OMNI_CANVAS_CYLINDER_H_
#define OMNI_CANVAS_CYLINDER_H_

#include <omni/canvas/Envelope.h>
#include <omni/visual/VertexVBO.h>

namespace omni
{
  namespace canvas
  {
    /// A cylindrical canvas
    class Cylinder : public QObject, public Envelope
    {
        OMNI_CANVAS_PLUGIN_DECL
        OMNI_PLUGIN_INFO("Cylinder canvas","Copyright (C) 2017")
    public:
      OMNI_REGISTER_CLASS(Factory,Cylinder)

      Cylinder();
      ~Cylinder();

      void draw() const;
      void update();

      void bind();

      float diameter() const;
      void setDiameter(float);

      float height() const;
      void setHeight(float);

      float beginAngle() const;
      void setBeginAngle(float);

      float endAngle() const;
      void setEndAngle(float);

      bool topCap() const;
      void setTopCap(bool);

      bool bottomCap() const;
      void setBottomCap(bool);

      /// Make new box parameter widget
      ui::CanvasParameters* widget();
    private:

      float diameter_ = 5.0;
      float height_ = 2.0;
      float beginAngle_ = 0.0;
      float endAngle_ = 360.0;
      const size_t segments_ = 64;

      bool topCap_ = false;
      bool bottomCap_ = false;

      visual::VertexVBO vbo_;
      visual::VertexVBO::vertex_buffer_type vertices_;
      visual::VertexVBO::index_buffer_type indices_;
    };
  }
}

#endif /* OMNI_CANVAS_CYLINDER_H_ */
