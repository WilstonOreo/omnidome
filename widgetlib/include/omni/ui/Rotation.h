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

#ifndef OMNI_UI_ROTATION_H_
#define OMNI_UI_ROTATION_H_

#include <omni/geometry/EulerAngles.h>
#include "Dial.h"

namespace omni {
  namespace ui {
    /// Rotation widget has three dials for rotation in X,Y,Z direction
    class OMNIWIDGET_EXPORT Rotation :
      public QWidget {
        Q_OBJECT

      public:
        /// Construct from optional parent widget
        Rotation(QWidget * = nullptr);

        /// Construct from given x,y,z angle values
        Rotation(double _x,
                 double _y,
                 double _z,
                 QWidget * = nullptr);
        ~Rotation();

        /// Return x angle (roll) value as double
        double      x() const;

        /// Return y angle (pitch) value as double
        double      y() const;

        /// Return z angle (yaw) value as double
        double      z() const;

        /// Return x,y,z values as euler angles
        EulerAngles angles() const;

      public slots:
        /// Set value for X angle (roll angle)
        void        setX(double);

        /// Set value for Y angle (pitch angle)
        void        setY(double);

        /// Set value for Z angle (yaw angle)
        void        setZ(double);

        /// Set rotation from euler angles
        void        setRotation(EulerAngles const&);

      signals:
        void        xChanged();
        void        yChanged();
        void        zChanged();
        void        rotationChanged();

      private:
        void        updateX();
        void        updateY();
        void        updateZ();

        void        init(double,
                         double,
                         double);
        Dial *x_;
        Dial *y_;
        Dial *z_;
    };
  }
}


#endif /* OMNI_UI_ROTATION_H_ */
