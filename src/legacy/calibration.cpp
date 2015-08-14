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

#include <odm/calibration.h>

namespace odm
{
  namespace calibration
  {
    static void normalmap(const Frustum& _frustum, canvas::Interface const* _canvas, Image4f& _out)
    {
      using namespace gex::raster;
      for_each(_out,strategy::for_each::Pixel(),[&](Color4f& _pix, size_t _x, size_t _y)
      {
        auto&& _ray = _frustum.ray(Vec2(Scalar(_x) / _out.width(),Scalar(_y) / _out.height())); 
        Point3 _iPoint;
        Vec3 _normal;
        if (_canvas->intersect(_ray,_iPoint,&_normal))
        {
          _pix(_normal.x(),_normal.y(),_normal.z());
        } else
        {
          _pix = Color4f(0,0,0,0);
        }
      });
    }

    static void normalmap_png(const Image4f& _image, const std::string& _filename)
    {
      using namespace gex::raster;
      typedef png::rgb_pixel_16 pixel_type;

      png::image< pixel_type > _png(_image.width(),_image.height());
      for_each(_image,strategy::for_each::Pixel(),[&](const Color4f& _pix, size_t _x, size_t _y)
      {
        // Function to convert a 32-bit float (in range [-1;1]) 
        // to a 16-bit unsigned integer (range [0:65535])
        auto conv = [](const Scalar& _v)
        {
          int i = (1 << 15) + _v*(1 << 15);  
          if (i < 0) return 0;
          if (i > 65535) return 65535;
          return i;
        };
        _png[_y][_x] = pixel_type(conv(_pix.r()),conv(_pix.g()),conv(_pix.b())); 
      });
      _png.write(_filename);
    }
    
    static void normalmap_png_sep(const Image4f& _image, const std::string& _filename)
    {
      using namespace gex::raster;
      typedef png::rgb_pixel pixel_type;

      png::image< pixel_type > _png(_image.width()*2,_image.height());
      for_each(_image,strategy::for_each::Pixel(),[&](const Color4f& _pix, size_t _x, size_t _y)
      {
        // Function to convert a 32-bit float (in range [-1;1]) 
        // to a 16-bit unsigned integer (range [0:65535])
        auto convUpper = [](const Scalar& _v)
        {
          int i = (1 << 7) + _v*(1 << 7);  
          if (i < 0) return 0;
          if (i > 255) return 255;
          return i;
        };
        auto convLower = [](const Scalar& _v)
        {
          int i = (1 << 15) + _v*(1 << 15);  
          return i & 255;
        };
        _png[_y][_x] = pixel_type(convUpper(_pix.r()),convUpper(_pix.g()),convUpper(_pix.b())); 
        _png[_y][_x+_image.width()] = pixel_type(convLower(_pix.r()),convLower(_pix.g()),convLower(_pix.b())); 
      });
      _png.write(_filename);
    }
  }
}

