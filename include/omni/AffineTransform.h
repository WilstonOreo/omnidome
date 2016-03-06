/* Copyright (c) 2014-2016 "Omnidome" by cr8tr
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
#ifndef OMNI_AFFINETRANSFORM_H_
#define OMNI_AFFINETRANSFORM_H_

#include <QVector3D>
#include <QMatrix4x4>
#include "EulerAngles.h"

namespace omni {
    /**@brief An affine linear transform with rotation, scale and translation
     **/
    class AffineTransform {
    public:
        AffineTransform();

        /// Return ref to rotation angles
        EulerAngles& rotation();
        /// Return const ref to rotation angles
        EulerAngles const& rotation() const;

        /// Set new rotation angles
        void setRotation(EulerAngles const&);

        /// Return reference to scale vector
        QVector3D& scale();

        /// Return const reference to scale vector
        QVector3D const& scale() const;

        /// Set new scale vector
        void setScale(QVector3D const&);

        /// Return reference to translation vector
        QVector3D& translation();

        /// Return reference to translation vector (const)
        QVector3D const& translation() const;

        /// Set new translation vector
        void setTranslation(QVector3D const&);

        /// Calculate transformation matrix
        QMatrix4x4 matrix() const;

        /// Calculate matrix for transformation around a center point
        QMatrix4x4 matrix(QVector3D const& _center) const;

        /// Return true if rotation is enabled
        bool rotationEnabled() const;

        /// Enable or disable rotation
        void setRotationEnabled(bool);

        /// Return true if scaling is enabled
        bool scaleEnabled() const;

        /// Enable or disable scale
        void setScaleEnabled(bool);

        /// Return true if translation is enabled
        bool translationEnabled() const;

        /// Enable or disable translation
        void setTranslationEnabled(bool);

    private:
        bool rotationEnabled_ = true;
        EulerAngles rotation_;

        bool scaleEnabled_ = true;
        QVector3D scale_;

        bool translationEnabled_ = true;
        QVector3D translation_;
    };
}

#endif /* OMNI_AFFINETRANSFORM_H_ */
