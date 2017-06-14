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
    BlendSettings::BlendSettings(QObject* parent) : QObject(parent) {}
    BlendSettings::~BlendSettings() {
    }

    OMNI_PROPERTY_RW_IMPL(BlendSettings,BlendSettings::ColorMode,colorMode,setColorMode)
    OMNI_PROPERTY_REAL_BOUND_IMPL(BlendSettings,inputOpacity,setInputOpacity,0.0,1.0)
    OMNI_PROPERTY_RW_IMPL(BlendSettings,bool,showInWarpMode,setShowInWarpMode)
}
