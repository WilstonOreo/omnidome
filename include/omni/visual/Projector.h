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

#ifndef OMNI_VISUAL_PROJECTOR
#define OMNI_VISUAL_PROJECTOR

#include <QColor>
#include <QVector3D>
#include <omni/visual/Interface.h>
#include <omni/proj/Projector.h>

namespace omni
{
  namespace visual
  {
    class Projector : public Interface
    {
    public:
      Projector(const proj::Projector&);
      ~Projector() {}

      void update();

      qreal size() const;
      void setSize(qreal);

      bool isSelected() const;
      void setSelected(bool);

      QColor color() const;
      void setColor(QColor);

      void draw() const;
      void drawHalo() const;

      void drawPositioning(QVector3D const& _center) const;

    private:

      proj::Projector const& proj_;

      bool selected_ = false;
      float size_ = 2.0;

      QColor color_;
      QVector3D eye_, topLeft_, topRight_, bottomLeft_, bottomRight_;
    };
  }
}

#endif /* OMNI_VISUAL_PROJECTOR */
