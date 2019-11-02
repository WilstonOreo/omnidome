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

#ifndef OMNI_VISUAL_CAMERAINTERFACE_H_
#define OMNI_VISUAL_CAMERAINTERFACE_H_

#include <QOpenGLFunctions>
#include "Tracker.h"

#if defined(Q_OS_WIN)
#undef near
#undef far
#endif

namespace omni {
  namespace visual {
    /// A generic camera interface
    class OMNI_EXPORT CameraInterface : public Tracker
    {
      public:
        CameraInterface();
        CameraInterface(const Tracker& _tracker,
               qreal _near = 0.1,
               qreal _far = 1000.0,
               const QVector3D& _up = QVector3D(0.0, 0.0, 1.0));
        ~CameraInterface() override = default;

        /// Right vector
        QVector3D right() const;

        /// Move camera to the right (or to the left if value is negative)
        void      strafe(qreal _right);

        /// Move camera upwards (or downwards if value is negative)
        void      lift(qreal _up);

        /// Camera orientation
        QVector3D up() const;
        void      setUp(QVector3D const&);

        /// Camera's near plane
        qreal     near() const;
        void setNear(qreal);

        /// Camera's far plane
        qreal far() const;
        void setFar(qreal);

        /// Setup perspective camera in OpenGL
        virtual void setup(qreal _aspect) const = 0;

        /// Deserialize from stream
        void        fromStream(QDataStream&) override;

        /// Serialize to stream
        void        toStream(QDataStream&) const override;

        /// Test for equality. ScreenSetup is ignored
        friend bool operator==(CameraInterface const&,
                    CameraInterface const&);

        /// Expose field of field
        inline virtual qreal fov() const {
          return 0.0;
        }

        inline virtual void setFov(qreal) {
        }

      private:
        /// camera orientation
        QVector3D up_;

        /// Near plane
        qreal near_ = 0.1;

        /// Far plane
        qreal far_ = 1000.0;
    };
  }
}

OMNI_DECL_STREAM_OPERATORS(omni::visual::CameraInterface)

#endif /* OMNI_VISUAL_CAMERAINTERFACE_H_ */
