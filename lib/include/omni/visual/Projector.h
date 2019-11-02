/* Copyright (c) 2014-2015 "Omnidome" by Michael Winkelmann
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

#include <memory>
#include <QColor>
#include <QVector3D>
#include <QOpenGLShaderProgram>
#include <omni/visual/Interface.h>
#include <omni/proj/Projector.h>
#include <omni/visual/ContextBoundPtr.h>

namespace omni
{
  namespace visual
  {
    /// Visualizer for drawing a projector with halo
    class OMNI_EXPORT Projector : public Interface
    {
    public:
      Projector(const proj::Projector&);
      ~Projector() {}

      void update();

      /// Return size of drawn projector
      qreal size() const;
      /// Set size
      void setSize(qreal);

      /// Halo is bright if projector is selected
      bool isSelected() const;

      /// Set boolean if projector is selected
      void setSelected(bool);

      /// Return drawn color
      QColor color() const;

      /// Set color if drawn projector
      void setColor(QColor);

      /// Draw projector line
      void draw() const;

      /// Draw projector value
      void drawHalo() const;

      /// Draw positioning of projector with center point
      void drawPositioning(QVector3D const& _center) const;

    private:
      proj::Projector const& proj_;

      bool selected_ = false;
      float size_ = 2.0;

      QColor color_;
      QVector3D eye_, topLeft_, topRight_, bottomLeft_, bottomRight_;

      static ContextBoundPtr<QOpenGLShaderProgram> haloShader_;
    };
  }
}

#endif /* OMNI_VISUAL_PROJECTOR */
