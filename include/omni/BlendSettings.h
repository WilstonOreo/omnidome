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

#ifndef OMNI_BLENDSETTINGS_H_
#define OMNI_BLENDSETTINGS_H_

#include <omni/Serializer.h>
#include <omni/property.h>

namespace omni {
  /// Common blend mask settings for the all tunings in a session
  class BlendSettings : public QObject {
      Q_OBJECT
  public:
      /// Blend Mask display colorColorMode
      enum ColorMode
      {
        CM_COLORED, // Displays blend mask with color of tuning
        CM_WHITE    // Displays white blend mask
      };
      Q_ENUM(ColorMode)
  private:
      OMNI_PROPERTY_RW_DEFAULT(ColorMode,colorMode,setColorMode,CM_COLORED)
      OMNI_PROPERTY_RW_DEFAULT(qreal,inputOpacity,setInputOpacity,0.0)
      OMNI_PROPERTY_RW_DEFAULT(bool,showInWarpMode,setShowInWarpMode,false)
  public:
      BlendSettings(QObject* = nullptr);
      ~BlendSettings();

    signals:
      void colorModeChanged();
      void inputOpacityChanged();
      void showInWarpModeChanged();
  };
}

#endif /* OMNI_BLENDSETTINGS_H_ */
