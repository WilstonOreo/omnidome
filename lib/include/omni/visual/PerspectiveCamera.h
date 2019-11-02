
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

#ifndef OMNI_VISUAL_PERSPECTIVECAMERA_H_
#define OMNI_VISUAL_PERSPECTIVECAMERA_H_

#include <omni/visual/CameraInterface.h>

namespace omni {
  namespace visual {
    /// A perspective camera
    class OMNI_EXPORT PerspectiveCamera: public CameraInterface
    {
      public:
        PerspectiveCamera(qreal _fov = 30.0);
        PerspectiveCamera(const Tracker& _tracker,
               qreal _fov = 30.0,
               qreal _near = 0.1,
               qreal _far = 1000.0,
               const QVector3D& _up = QVector3D(0.0, 0.0, 1.0));

        /// Setup perspective camera in OpenGL
        void setup(qreal _aspect) const;

        /// Return field of view value
        qreal fov() const;

        /// Set new field of view value
        void setFov(qreal _fov);

        /// Deserialize from stream
        void        fromStream(QDataStream&);

        /// Serialize to stream
        void        toStream(QDataStream&) const;

        /// Test for equality. ScreenSetup is ignored
        friend bool operator==(PerspectiveCamera const&,
                               PerspectiveCamera const&);

      private:
        /// Field of View Angle
        qreal fov_ = 30.0;
    };
  }
}

OMNI_DECL_STREAM_OPERATORS(omni::visual::PerspectiveCamera)

#endif /* OMNI_VISUAL_PERSPECTIVECAMERA_H_ */
