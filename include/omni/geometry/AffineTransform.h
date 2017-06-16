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
#include <omni/Serializer.h>
#include <omni/property.h>
#include "EulerAngles.h"

namespace omni {
  namespace geometry {
    /**@brief An affine linear transform with rotation, scale and translation
    **/
    class AffineTransform : public QObject, public Serializer<AffineTransform> {
        Q_OBJECT
    public:
        enum ScaleMode {
          SCALE_NONE,
          SCALE_UNIFORM,
          SCALE_NON_UNIFORM
        };
        Q_ENUM(ScaleMode)
    private:
        OMNI_PROPERTY_RW_DEFAULT(bool,rotationEnabled,setRotationEnabled,true)
        OMNI_PROPERTY_OBJ(EulerAngles,rotation)
        OMNI_PROPERTY_RW_DEFAULT(ScaleMode,scaleMode,setScaleMode,SCALE_UNIFORM)
        OMNI_PROPERTY_RW(QVector3D,scale,setScale)
        OMNI_PROPERTY_RW_DEFAULT(qreal,uniformScale,setUniformScale,1.0)
        OMNI_PROPERTY_RW_DEFAULT(bool,translationEnabled,setTranslationEnabled,true)
        OMNI_PROPERTY_RW(QVector3D,translation,setTranslation)
      public:

        AffineTransform(QObject* = nullptr);

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
    };
  }

  using geometry::AffineTransform;
}

#endif /* OMNI_GEOMETRY_AFFINETRANSFORM_H_ */
