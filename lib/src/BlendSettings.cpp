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

#include <omni/BlendSettings.h>

#include <omni/util.h>
#include <omni/serialization/PropertyMap.h>

namespace omni {
    BlendSettings::ColorMode BlendSettings::colorMode() const
    {
        return colorMode_;
    }

    void BlendSettings::setColorMode(ColorMode _colorMode)
    {
        colorMode_ = _colorMode;
    }

    float BlendSettings::inputOpacity() const
    {
        return inputOpacity_;
    }

    void BlendSettings::setInputOpacity(float _inputOpacity)
    {
        inputOpacity_ = _inputOpacity;
    }

    /// Show blend mask in warp colorMode
    bool BlendSettings::showInWarpMode() const {
        return showInWarpMode_;
    }

    /// Set flag if blend mask is visible in warp colorMode
    void BlendSettings::setShowInWarpMode(bool _showInWarpMode) {
        showInWarpMode_ = _showInWarpMode;
    }

    /// Deserialize from stream
    void BlendSettings::fromStream(QDataStream& _is) {
        PropertyMap _map;
        _is >> _map;

        colorMode_ = util::intToEnum<ColorMode>(_map.getValue<int>("colorMode"));
        _map.get("showInWarpMode",showInWarpMode_);
        _map.get("inputOpacity",inputOpacity_);
    }

    /// Serialize to stream
    void BlendSettings::toStream(QDataStream& _os) const {
        PropertyMap _map;
        _map("colorMode",util::enumToInt(colorMode_));
        _map("showInWarpMode",showInWarpMode_);
        _map("inputOpacity",inputOpacity_);
        _os << _map;
    }

    bool operator==(BlendSettings const& _lhs, BlendSettings const& _rhs) {
        return
            OMNI_TEST_MEMBER_EQUAL(colorMode_) &&
            OMNI_TEST_MEMBER_EQUAL(showInWarpMode_) &&
            OMNI_TEST_MEMBER_EQUAL(inputOpacity_);
    }
}
