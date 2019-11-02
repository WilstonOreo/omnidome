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

#ifndef OMNI_VISUAL_LIGHT_H_
#define OMNI_VISUAL_LIGHT_H_

#include <array>
#include <QColor>
#include <QOpenGLFunctions>
#include "Tracker.h"

namespace omni {
  namespace visual {
    /// A light with position and phong shader attributes
    struct OMNI_EXPORT Light : Tracker
    {
      public:
        Light(const Tracker& _tracker = Tracker(),
              qreal const& _intensity = 1.0);

        Light(const Tracker& _tracker,
              const QColor& _ambient,
              const QColor& _diffuse,
              const QColor& _specular,
              const qreal& _intensity = 1.0);


        /// Setup light in OpenGL
        void   setup(GLuint _index = GL_LIGHT0);

        /// Return color for ambient
        QColor ambient() const;

        /// Set new ambient color
        void setAmbient(QColor);

        /// Return diffuse color
        QColor diffuse() const;

        /// Set new diffuse color
        void setDiffuse(QColor);

        /// Return specular color
        QColor specular() const;

        /// Set new specular color
        void setSpecular(QColor);

        /// Return intensity value
        qreal  intensity() const;

        /// Set new intensity value
        void setIntensity(qreal);

        /// Deserialize from stream
        void        fromStream(QDataStream&);

        /// Serialize to stream
        void        toStream(QDataStream&) const;

        /// Test for equality. ScreenSetup is ignored
        friend bool operator==(Light const&,
                               Light const&);

      private:
        typedef std::array<GLfloat, 4>param_type;
        param_type colorParam(QColor) const;
        param_type eye4() const;

        QColor ambient_, diffuse_, specular_;
        qreal  intensity_;
    };
  }
}

OMNI_DECL_STREAM_OPERATORS(omni::visual::Light)

#endif /* OMNI_VISUAL_LIGHT_H_ */
