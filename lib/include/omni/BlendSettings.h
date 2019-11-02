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

#include <omni/serialization/Interface.h>

namespace omni {
  /// Common blend mask settings for the all tunings in a session
  class OMNI_EXPORT BlendSettings {
    public:
      /// Blend Mask display colorColorMode
      enum class ColorMode
      {
        COLORED, // Displays blend mask with color of tuning
        WHITE    // Displays white blend mask
      };

      /// Return colorColorMode of blend mask colorColorMode
      ColorMode colorMode() const;

      /// Set blend mask colorColorMode
      void setColorMode(ColorMode);

      /// Return input opacity of blend mask
      float       inputOpacity() const;

      /// Opacity of input when in blend mask colorColorMode
      void        setInputOpacity(float _input);

      /// Show blend mask in warp colorColorMode
      bool        showInWarpMode() const;

      /// Set flag if blend mask is visible in warp colorColorMode
      void        setShowInWarpMode(bool);

      /// Deserialize from stream
      void        fromStream(QDataStream&);

      /// Serialize to stream
      void        toStream(QDataStream&) const;

      /// Test for equality.
      friend bool operator==(BlendSettings const&,
                             BlendSettings const&);

    private:
      ColorMode colorMode_  = ColorMode::COLORED;
      bool  showInWarpMode_ = false;
      float inputOpacity_   = 0.0;
  };
}

OMNI_DECL_STREAM_OPERATORS(omni::BlendSettings)

#endif /* OMNI_BLENDSETTINGS_H_ */
