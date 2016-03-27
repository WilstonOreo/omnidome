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

#ifndef OMNI_PROJ_PROJECTOR_H_
#define OMNI_PROJ_PROJECTOR_H_

#include <memory>
#include <QMatrix4x4>
#include <QScreen>
#include <omni/Angle.h>
#include "Setup.h"

namespace omni {
  namespace proj {
    /// A projector with a transformation matrix and setup
    class Projector {
      public:
        Projector();
        Projector(
          QScreen const *_screen,
          int _subScreenIndex,
          Angle _fov = 45.0);

        /**@brief Make a new projector setup with a certain id and delete old
           one
         * @return Pointer to new projector setup
         **/
        Setup           * setup(Id const& _setupId);

        /// Update projector matrix by current setup and return setup
        Setup           * setup();

        /// Returns projector setup (const version)
        Setup const     * setup() const;

        /// Aspect ratio of screen
        qreal             aspectRatio() const;

        /// Assign screen and subscreen to projector
        void              setScreen(QScreen const *_screen,
                                    int _subScreenIndex);

        /// Return pointer to screen
        QScreen const   * screen() const;

        /**@brief Return subscreen index
           @detail Used to determine screen position when screen is a triple head
         **/
        int               subScreenIndex() const;

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
        void setKeystone(qreal);

        /// Return transformation matrix
        QMatrix4x4 const& matrix() const;

        /// Set transformation matrix
        void              setMatrix(QMatrix4x4 const&);

        /// Return projector matrix
        QMatrix4x4        projectionMatrix() const;

        /// Return projector position (3rd column of projector matrix)
        QVector3D         pos() const;

        /// Write projector to stream
        void              toStream(QDataStream&) const;

        /// Read projector from stream
        void              fromStream(QDataStream&);

        /// Test for equality
        friend bool       operator==(Projector const&,
                                     Projector const&);

      private:
        QMatrix4x4 matrix_;
        QScreen const *screen_ = nullptr;
        int   subScreenIndex_  = -1;
        Angle fov_;
        qreal keystone_ = 0.0;
        std::unique_ptr<Setup> setup_;
    };
  }

  using proj::Projector;
}

OMNI_DECL_STREAM_OPERATORS(omni::proj::Projector)

#endif /* OMNI_PROJ_PROJECTOR_H_ */
