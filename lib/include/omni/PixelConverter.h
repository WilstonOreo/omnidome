/* Copyright (c) 2014-2015 "Omnidome" by Michael Winkelmann
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

#undef IN
#undef OUT

namespace omni {
  /// RGBA float pixel type
  struct RGBAFloat
  {
    RGBAFloat() {}

    RGBAFloat(float r, float g, float b, float a = 1.0) :
      r(r), g(g), b(b), a(a) {}

    float r, g, b, a;
  };

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


  /// Convert RGBA to QColor
  template<>
  struct PixelConverter<RGBAFloat, QColor>{
    template<typename T>
    void operator()(T const& _in, QColor& _out) {
      _out = QColor(
        qBound(0.0,_in.r*255.0,255.0),
        qBound(0.0,_in.g*255.0,255.0),
        qBound(0.0,_in.b*255.0,255.0),
        qBound(0.0,_in.a*255.0,255.0));
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
