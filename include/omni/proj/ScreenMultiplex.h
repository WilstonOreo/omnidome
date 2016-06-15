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

#ifndef OMNI_PROJ_SCREENMULTIPLEX_H_
#define OMNI_PROJ_SCREENMULTIPLEX_H_

#include <QScreen>
#include <QRect>

namespace omni {
  namespace proj {
    /// Screen Multiplexer for using sub screens
    class ScreenMultiplex {
    public:
      enum Tiling {
        TILING_1X1,
        TILING_2X1,
        TILING_3X1,
        TILING_4X1,    
        TILING_2X2,    
        TILING_3X3,    
        TILING_4X4,
        NUM_TILINGS
      };

      ScreenMultiplex(Tiling const& = TILING_2X2);

      /// Return screen tile mode
      Tiling tiling() const;

      /// Set screen tiling mode
      void setTiling(Tiling const&);

      /// Return number of sub screens for current tile mode
      int numberOfSubScreens() const;

      /// Return content geometry rectangle for given multiplex setup
      QRect contentGeometry(QSize const&, int _index) const;

    private:
      Tiling tiling_ = TILING_1X1;
    };
  }
}



#endif /* OMNI_PROJ_SCREENMULTIPLEX_H_ */
