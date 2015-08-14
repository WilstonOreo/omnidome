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

#include <omni/visual/Light.h>

#include <omni/visual/util.h>

namespace omni
{
  namespace visual
  {

    Light::Light( const Tracker& _tracker,
          qreal const& _intensity) :
      Tracker(_tracker),
      ambient_("#111111"),
      diffuse_("#FFFFFF"),
      specular_("#FFFFFF"),
      intensity_(_intensity)
    {
    }

    Light::Light(
      const Tracker& _tracker,
      const QColor& _ambient,
      const QColor& _diffuse,
      const QColor& _specular,
      const qreal& _intensity) :
      Tracker(_tracker),
      ambient_(_ambient),
      diffuse_(_diffuse),
      specular_(_specular),
      intensity_(_intensity)
    {
    }

    void Light::setup(GLuint _index)
    {
      // light and material
      visual::with_current_context([&](QOpenGLFunctions& _)
        {
          _.glEnable(GL_LIGHTING);
          _.glEnable(GL_COLOR_MATERIAL);
          glLightfv(_index, GL_DIFFUSE, colorParam(diffuse()).data());
          glLightfv(_index, GL_POSITION, eye4().data());
          glLightModelfv(GL_LIGHT_MODEL_AMBIENT, colorParam(ambient()).data());
          glShadeModel(GL_SMOOTH);
          _.glEnable(_index);
      });
    }

    QColor Light::ambient() const
    {
      return ambient_;
    }
    
    void Light::setAmbient(QColor _ambient)
    {
      ambient_ = _ambient;
    }

    QColor Light::diffuse() const
    {
      return diffuse_;
    }

    void Light::setDiffuse(QColor _diffuse)
    {
      diffuse_ = _diffuse;
    }

    QColor Light::specular() const
    {
      return specular_;
    }

    void Light::setSpecular(QColor _specular)
    {
      specular_ = _specular;
    }

    qreal Light::intensity() const
    {
      return intensity_;
    }

    void Light::setIntensity(qreal _intensity)
    {
      intensity_ = _intensity;
    }

    Light::param_type Light::colorParam(QColor _color) const
    {
      return param_type({ 
          GLfloat(_color.redF() * intensity_),
          GLfloat(_color.greenF() * intensity_),
          GLfloat(_color.blueF() * intensity_),
          GLfloat(1.0)
          });
    }

    Light::param_type Light::eye4() const
    {
      auto e = eye();
      return param_type({e.x(),e.y(),e.z(),GLfloat(0.0)});
    }
  }
}
