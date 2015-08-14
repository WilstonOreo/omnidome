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

#ifndef OMNI_VISUAL_LIGHT_H_
#define OMNI_VISUAL_LIGHT_H_

#include <array>
#include <QColor>
#include <QOpenGLFunctions>
#include "Tracker.h"

namespace omni
{
  namespace visual
  {
    /// A light with position and phong shader attributes
    struct Light : Tracker
    {
      Light( const Tracker& _tracker = Tracker(),
          qreal const& _intensity = 1.0);
      
      Light( const Tracker& _tracker,
             const QColor& _ambient,
             const QColor& _diffuse,
             const QColor& _specular,
             const qreal& _intensity = 1.0);


      /// Setup light in OpenGL
      void setup(GLuint _index = GL_LIGHT0);

      QColor ambient() const;
      void setAmbient(QColor);

      QColor diffuse() const;
      void setDiffuse(QColor);
      
      QColor specular() const;
      void setSpecular(QColor);

      qreal intensity() const;
      void setIntensity(qreal);

    private:
      typedef std::array<GLfloat,4> param_type;
      param_type colorParam(QColor) const;
      param_type eye4() const;

      QColor ambient_, diffuse_, specular_;
      qreal intensity_;
    };
  }
}

#endif /* OMNI_VISUAL_LIGHT_H_ */

