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
#ifndef OMNI_GEOMETRY_AFFINETRANSFORM_H_
#define OMNI_GEOMETRY_AFFINETRANSFORM_H_

#include <QVector3D>
#include <QMatrix4x4>
#include <omni/serialization/Serializer.h>
#include "EulerAngles.h"

namespace omni {
  namespace geometry {
    /**@brief An affine linear transform with rotation, scale and translation
    **/
    class AffineTransform : public QObject, public Serializer<AffineTransform> {
        Q_OBJECT
        Q_PROPERTY(bool rotationEnabled READ rotationEnabled WRITE setRotationEnabled NOTIFY rotationEnabledChanged)
        Q_PROPERTY(EulerAngles* rotation READ rotation CONSTANT)
        Q_PROPERTY(ScaleMode scaleMode READ scaleMode WRITE setScaleMode NOTIFY scaleModeChanged)
        Q_PROPERTY(QVector3D scale READ scale WRITE setScale NOTIFY scaleChanged)
        Q_PROPERTY(qreal uniformScale READ uniformScale WRITE setUniformScale NOTIFY uniformScaleChanged)
        Q_PROPERTY(bool translationEnabled READ translationEnabled WRITE setTranslationEnabled NOTIFY translationEnabledChanged)
        Q_PROPERTY(QVector3D translation READ translation WRITE setTranslation NOTIFY translationChanged)
      public:
        enum ScaleMode {
          SCALE_NONE,
          SCALE_UNIFORM,
          SCALE_NON_UNIFORM
        };
        Q_ENUM(ScaleMode)

        AffineTransform(QObject* = nullptr);


        /// Return true if rotation is enabled
        bool             rotationEnabled() const;

        /// Enable or disable rotation
        void             setRotationEnabled(bool);

        /// Return ref to rotation angles
        EulerAngles      * rotation() const;

        /// Return scale mode
        ScaleMode        scaleMode() const;

        /// Set scale mode
        void             setScaleMode(ScaleMode);

        /// Return scale vector
        QVector3D        scale()  const;

        /// Set new scale vector
        void             setScale(QVector3D);

        /// Return scale factor
        qreal            uniformScale()  const;

        /// Set uniform value for scale
        void             setUniformScale(qreal);

        /// Return true if translation is enabled
        bool             translationEnabled() const;

        /// Enable or disable translation
        void             setTranslationEnabled(bool);

        /// Return translation vector
        QVector3D        translation() const;

        /// Set new translation vector
        void             setTranslation(QVector3D);

        /// Calculate transformation matrix
        QMatrix4x4       matrix() const;

        /// Calculate matrix for transformation around a center point
        QMatrix4x4       matrix(QVector3D const& _center) const;

      signals:
        void rotationEnabledChanged();
        void scaleModeChanged();
        void scaleChanged();
        void uniformScaleChanged();
        void translationEnabledChanged();
        void translationChanged();

      private:
        bool rotationEnabled_ = true;
        EulerAngles* rotation_;

        ScaleMode scaleMode_ = SCALE_UNIFORM;
        QVector3D scale_;
        qreal uniformScale_ = 1.0;

        bool translationEnabled_ = true;
        QVector3D translation_;
    };
  }

  using geometry::AffineTransform;
}

#endif /* OMNI_GEOMETRY_AFFINETRANSFORM_H_ */
