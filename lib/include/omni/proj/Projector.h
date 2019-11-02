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

#ifndef OMNI_PROJ_PROJECTOR_H_
#define OMNI_PROJ_PROJECTOR_H_

#include <memory>
#include <QMatrix4x4>
#include <QScreen>
#include <omni/geometry/EulerAngles.h>
#include <omni/serialization/Interface.h>

namespace omni {
  namespace proj {
    /// A projector with a transformation matrix and setup
    class OMNI_EXPORT Projector : public EulerAngles {
      public:
        enum Setup {
          PERIPHERAL,
          FREE
        };

        Projector(Setup = PERIPHERAL);

        /// Return projector setup mode (FREE vs PERIPHERAL)
        Setup             setup() const;

        /// Set projector setup mode
        void              setSetup(Setup);

        void              setupFree(EulerAngles const&, QVector3D const&);
        void              setupPeripheral(EulerAngles const&,
                            Angle _deltaYaw = 0.0,
                            qreal _distanceCenter = 0.4,
                            qreal _towerHeight = 0.2,
                            qreal _shift = 0.0
                          );

        /// Aspect ratio of screen
        qreal             aspectRatio() const;

        /// Set aspect ratio of screen
        void              setAspectRatio(qreal);

        /// Return throw ratio of projector ( = 1/2 * tan(fov / 2))
        qreal             throwRatio() const;

        /// Set new throw ratio
        void setThrowRatio(qreal);

        /// Return field of view
        Angle             fov() const;

        /// Set new field of view
        void              setFov(Angle _fov);

        /// Return keystone correction value for projector
        qreal             keystone() const;

        /// Set new keystone correction value (0.0 = default)
        void              setKeystone(qreal);

        /// Return transformation matrix
        QMatrix4x4 const& matrix() const;

        /// Set transformation matrix
        void              setMatrix(QMatrix4x4 const&);

        /// Return projector matrix
        QMatrix4x4        projectionMatrix() const;

        /// Return projector position (3rd column of projector matrix)
        QVector3D const&  pos() const;

        /// Returns true if projector roll angle is > 90deg
        bool flipped() const;

        /// Delta yaw angle from rotating yaw angle
        Angle deltaYaw() const;

        /// Delta yaw angle from rotating yaw angle
        void setDeltaYaw(Angle);

        /// Return distance from center
        qreal distanceCenter() const;

        /// Set distance to center
        void setDistanceCenter(qreal);

        /// Return tower height (z position)
        qreal towerHeight() const;

        /// Set tower height (z position of projector)
        void setTowerHeight(qreal);

        /// Distance to centric line
        qreal shift() const;

        /// Set shift (distance to centric line)
        void setShift(qreal);

        /// Scale projector setup by factor
        void scale(qreal);

        /// Set position from QVector3D
        void      setPos(QVector3D const&);

        /// Set position from x,y,z position values
        void      setPos(qreal _x,
                         qreal _y,
                         qreal _z);

        /// Write projector to stream
        void              toStream(QDataStream&) const;

        /// Read projector from stream
        void              fromStream(QDataStream&);


        /// Test for equality
        friend bool       operator==(Projector const&,
                                     Projector const&);

      private:
        void      update();
        Setup setup_ = PERIPHERAL;
        QMatrix4x4 matrix_;
        qreal aspectRatio_ = 1.5;
        Angle fov_;
        qreal keystone_ = 0.0;

        QVector3D pos_;
        Angle deltaYaw_ = 0.0;
        qreal distanceCenter_ = 0.4;
        qreal towerHeight_ = 0.2;
        qreal shift_ = 0.0;
    };
  }

  using proj::Projector;
}

OMNI_DECL_STREAM_OPERATORS(omni::proj::Projector)
OMNI_DECL_ENUM_STREAM_OPERATORS(omni::proj::Projector::Setup)

#endif /* OMNI_PROJ_PROJECTOR_H_ */
