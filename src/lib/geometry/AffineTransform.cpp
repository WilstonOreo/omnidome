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

#include <omni/geometry/AffineTransform.h>

#include <omni/util.h>
#include <omni/serialization/PropertyMap.h>

namespace omni {
  namespace geometry {
    AffineTransform::AffineTransform(QObject* parent) :
        QObject(parent),
        rotation_(new EulerAngles(this)),
        scale_(1.0, 1.0, 1.0),
        translation_(0.0, 0.0, 0.0) {}

    EulerAngles* AffineTransform::rotation() const {
        return rotation_;
    }

    OMNI_PROPERTY_RW_IMPL(AffineTransform,bool,rotationEnabled,setRotationEnabled)
    OMNI_PROPERTY_RW_IMPL(AffineTransform,AffineTransform::ScaleMode,scaleMode,setScaleMode)
    OMNI_PROPERTY_RW_IMPL(AffineTransform,QVector3D,scale,setScale)
    OMNI_PROPERTY_RW_IMPL(AffineTransform,qreal,uniformScale,setUniformScale)
    OMNI_PROPERTY_RW_IMPL(AffineTransform,bool,translationEnabled,setTranslationEnabled)
    OMNI_PROPERTY_RW_IMPL(AffineTransform,QVector3D,translation,setTranslation)

    QMatrix4x4 AffineTransform::matrix() const {
        return matrix(QVector3D(0.0, 0.0, 0.0));
    }

    QMatrix4x4 AffineTransform::matrix(QVector3D const& _center) const {
        QMatrix4x4 _m;

        _m.translate(_center);
        if (translationEnabled()) _m.translate(translation_);
        if (rotationEnabled()) _m *= rotation_->matrix();

        switch(scaleMode()) {
        case SCALE_UNIFORM: _m.scale(scale()); break;
        case SCALE_NON_UNIFORM: _m.scale(uniformScale()); break;
        default: case SCALE_NONE: break;
        }
        _m.translate(-_center);
        return _m;
    }

  }
}
