/* Copyright (c) 2014-2016 "Omnidome" by Michael Winkelmann
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

#ifndef OMNI_VISUAL_INTERFACE_H_
#define OMNI_VISUAL_INTERFACE_H_

#include <QOpenGLFunctions>
#include <omni/global.h>

namespace omni {
  namespace visual {
    /// Basic visual interface
    class OMNI_EXPORT Interface {
      public:
        virtual ~Interface() {}

        /// Abstract method for visualing
        virtual void draw() const = 0;

        /// Update geometry
        inline virtual void update()
        {}

      protected:
        /// glVertex3f from QVector3D
        static void vertex3(QVector3D const&);

        /// Draws a line
        static void visualLine(QVector3D const& _from,
                               QVector3D const& _to);

        /// glColor4f from QColor
        static void color(QColor _color,
                          float _alpha = 1.0);
    };
  }
}


#endif /* OMNI_VISUAL_INTERFACE_H_ */
