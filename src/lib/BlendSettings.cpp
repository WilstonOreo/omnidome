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

#include <omni/BlendSettings.h>

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
}
