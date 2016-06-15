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

#include <omni/proj/ScreenMultiplex.h>

namespace omni {
  namespace proj {
    ScreenMultiplex::ScreenMultiplex(
        Tiling const& _tiling) :
      tiling_(_tiling) {}

    ScreenMultiplex::Tiling ScreenMultiplex::tiling() const {
      return tiling_;
    }

    void ScreenMultiplex::setTiling(Tiling const& _tiling) {
      tiling_ = _tiling;
    }

    QRect ScreenMultiplex::contentGeometry(QSize const& _size, int _index) const { 
      int _w = _size.width();
      int _h = _size.height();

      switch (tiling_) {
      default:
      case TILING_1X1:
      break;
      case TILING_2X1:
        return QRect(_w * _index / 2,0,_w/2,_h);
      case TILING_3X1:
        return QRect(_w * _index / 3,0,_w/3,_h);
      case TILING_4X1:
        return QRect(_w * _index / 4,0,_w/4,_h);
      case TILING_2X2:
        return QRect(_w * (_index % 2) / 2,_h * (_index / 2) / 2,_w/2,_h/2);
      case TILING_3X3:
        return QRect(_w * (_index % 3) / 3,_h * (_index / 3) / 3,_w/3,_h/3);
      case TILING_4X4:
        return QRect(_w * (_index % 4) / 4,_h * (_index / 4) / 4,_w/4,_h/4);
      }

      return QRect(0,0,_w,_h);
    }
      
    int ScreenMultiplex::numberOfSubScreens() const {
      switch (tiling_) {
      default:
      case TILING_1X1:
      break;
      case TILING_2X1: return 2;
      case TILING_3X1: return 3;
      case TILING_4X1: return 4;
      case TILING_2X2: return 4;
      case TILING_3X3: return 9;
      case TILING_4X4: return 16;
      }
      return 1;
    }
  }
}
