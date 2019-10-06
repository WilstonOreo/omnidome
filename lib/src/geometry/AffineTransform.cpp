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
    AffineTransform::AffineTransform() :
        scale_(1.0, 1.0, 1.0),
        translation_(0.0, 0.0, 0.0) {}

    EulerAngles const& AffineTransform::rotation() const {
        return rotation_;
    }

    EulerAngles& AffineTransform::rotation() {
        return rotation_;
    }

    void AffineTransform::setRotation(EulerAngles const& _rotation) {
        rotation_ = _rotation;
    }

    QVector3D AffineTransform::scale() const {
        float _s = uniformScale();
        return uniformScaleEnabled_ ? QVector3D(_s,_s,_s) : scale_;
    }

    /// Return scale factor
    float AffineTransform::uniformScale()  const {
      return uniformScale_;
    }

    void AffineTransform::setScale(QVector3D const& _scale) {
        scale_ = _scale;
    }

    void AffineTransform::setScale(float _uniformScale) {
        uniformScale_ = _uniformScale;
        if (!uniformScaleEnabled_) {
          scale_ = QVector3D(_uniformScale,_uniformScale,_uniformScale);
        }
    }

    QVector3D const& AffineTransform::translation() const {
        return translation_;
    }

    QVector3D& AffineTransform::translation() {
        return translation_;
    }

    void AffineTransform::setTranslation(QVector3D const& _translation) {
        translation_ = _translation;
    }

    QMatrix4x4 AffineTransform::matrix() const {
        return matrix(QVector3D(0.0, 0.0, 0.0));
    }

    QMatrix4x4 AffineTransform::matrix(QVector3D const& _center) const {
        QMatrix4x4 _m;

        _m.translate(_center);
        if (translationEnabled()) _m.translate(translation_);
        if (rotationEnabled()) _m *= rotation_.matrix();
        if (scaleEnabled()) {
          _m.scale(scale());

        }
        _m.translate(-_center);
        return _m;
    }

    /// Return true if rotation is enabled
    bool AffineTransform::rotationEnabled() const {
        return rotationEnabled_;
    }

    /// Enable or disable rotation
    void AffineTransform::setRotationEnabled(bool _enabled) {
        rotationEnabled_ = _enabled;
    }

    /// Return true if scaling is enabled
    bool AffineTransform::uniformScaleEnabled() const {
        return uniformScaleEnabled_;
    }

    /// Enable or disable scale
    void AffineTransform::setUniformScaleEnabled(bool _enabled) {
        uniformScaleEnabled_ = _enabled;
    }

    /// Return true if scaling is enabled
    bool AffineTransform::scaleEnabled() const {
        return scaleEnabled_;
    }

    /// Enable or disable scale
    void AffineTransform::setScaleEnabled(bool _enabled) {
        scaleEnabled_ = _enabled;
    }

    /// Return true if translation is enabled
    bool AffineTransform::translationEnabled() const {
        return translationEnabled_;
    }

    /// Enable or disable translation
    void AffineTransform::setTranslationEnabled(bool _enabled) {
        translationEnabled_ = _enabled;
    }

    /// Write transformation to stream
    void AffineTransform::toStream(QDataStream& _os) const {
        PropertyMap _map;
        _map("rotationEnabled",rotationEnabled_)
            ("rotation",rotation_)
            ("scaleEnabled",scaleEnabled_)
            ("scale",scale_)
            ("uniformScale",uniformScale_)
            ("uniformScaleEnabled",uniformScaleEnabled_)
            ("translationEnabled",translationEnabled_)
            ("translation",translation_);
        _os << _map;
    }

    /// Read transformation from stream
    void AffineTransform::fromStream(QDataStream& _is) {
        PropertyMap _map;
        _is >> _map;
        _map.get("rotationEnabled",rotationEnabled_);
        _map.get("rotation",rotation_);
        _map.get("scaleEnabled",scaleEnabled_);
        _map.get("scale",scale_);
        _map.get("uniformScale",uniformScale_);
        _map.get("uniformScaleEnabled",uniformScaleEnabled_);
        _map.get("translationEnabled",translationEnabled_);
        _map.get("translation",translation_);

        OMNI_DEBUG << translation_;
    }

    bool operator==(AffineTransform const& _lhs, AffineTransform const& _rhs) {
        return
            OMNI_TEST_MEMBER_EQUAL(rotationEnabled_) &&
            OMNI_TEST_MEMBER_EQUAL(rotation_) &&
            OMNI_TEST_MEMBER_EQUAL(scaleEnabled_) &&
            OMNI_TEST_MEMBER_EQUAL(scale_) &&
            OMNI_TEST_MEMBER_EQUAL(uniformScaleEnabled_) &&
            OMNI_TEST_MEMBER_EQUAL(translationEnabled_) &&
            OMNI_TEST_MEMBER_EQUAL(translation_);
    }
  }
}
