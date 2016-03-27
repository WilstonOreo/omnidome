/* Copyright (c) 2014-2015 "Omnidome" by cr8tr
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
#ifndef OMNI_PIXELCONVERTER_H_
#define OMNI_PIXELCONVERTER_H_

#include <QColor>

namespace omni {
  /// Template for converting IN pixel type to OUT pixel type
  template<typename IN, typename OUT>
  struct PixelConverter {
    void operator()(IN const& _in, OUT& _out) {}
  };

  /// Convert to QColor
  template<typename IN>
  struct PixelConverter<IN, QColor>{
    template<typename T>
    void operator()(T const& _in, QColor& _out) {
      int _v = qBound(0, int(255.0 * _in), 255);

      _out = QColor(_v, _v, _v);
    }
  };

  /// Convert byte value to grayscale QColor
  template<>
  struct PixelConverter<uint8_t, QColor>{
    template<typename T>
    void operator()(T const& _in, QColor& _out) {
      _out = QColor(_in, _in, _in);
    }
  };

  /// Convert int value to grayscale QColor
  template<>
  struct PixelConverter<int, QColor>: PixelConverter<uint8_t, QColor>{};

  /// Convert int value to grayscale QColor
  template<>
  struct PixelConverter<unsigned int, QColor>: PixelConverter<uint8_t, QColor>{};


  /// Convert IN to OUT pixel
  template<typename IN, typename OUT>
  void convertPixel(const IN& _in, OUT& _out) {
    PixelConverter<IN, OUT>()(_in, _out);
  }

  /// Convert pixel IN to OUT pixel and return OUT
  template<typename OUT, typename IN>
  OUT convertPixel(const IN& _in) {
    OUT _out;

    convertPixel(_in, _out);
    return _out;
  }
}

#endif /* OMNI_PIXELCONVERTER_H_ */
