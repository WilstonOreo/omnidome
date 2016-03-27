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

#ifndef OMNI_BLENDMASK_H_
#define OMNI_BLENDMASK_H_

#include <vector>
#include <QRectF>
#include <omni/BlendBrush.h>

namespace omni {
  namespace proj {
    class Tuning;
  }

  /**@brief A blend mask consists of an edge mask and stroke mask with a brush
   * @detail Edge mask and stroke mask are stored in different buffers
   */
  class BlendMask {
    public:

      typedef Buffer<uint8_t>Buffer;

      BlendMask(proj::Tuning const& _tuning);

      /// Clears stroke buffer
      void clear();

      /**@brief Sets rect of blends
       *@detail Clamps rect to borders 0,0,1,1 if its overlapping
       **/
      void setRect(QRectF const&,
                   bool _update = true);

      /// Returns rectangle
      QRectF            rect() const;

      /// Returns distance to top edge
      float             topWidth() const;

      /// Returns distance to bottom edge
      float             bottomWidth() const;

      /// Returns distance to left edge
      float             leftWidth() const;

      /// Returns distance to right edge
      float             rightWidth() const;

      /// Sets gamma value for edge mask
      void              setGamma(float _gamma);

      /// Return gamma value
      float             gamma() const;

      /// Return reference blend brush
      BlendBrush      & brush();

      /// Return reference blend brush (const version)
      BlendBrush const& brush() const;

      /// Stamp on stroke buffer with current brush at position x y
      void              stamp(const QPointF& _pos);

      /**@brief Draws a line on stroke buffer
       * @returns new left over value
       */
      float             drawLine(QPointF const& _p0,
                                 QPointF const& _p1,
                                 float          _leftOver = 0.0);

      /// Returns stroke buffer (read only)
      Buffer const& strokeBuffer() const;

      void          resize(int width,
                           int height);

      /// Return void* pointer to strokebuffer data, used for OpenGL Texture
      void      * strokeBufferData() const;

      /// Write blend mask to stream
      void        toStream(QDataStream&) const;

      /// Read blend mask from stream
      void        fromStream(QDataStream&);

      /// Test for equality, buffers are ignored
      friend bool operator==(BlendMask const&,
                             BlendMask const&);

    private:

      /// Returns blend value for mask buffer at position x y
      float borderValue(float _x,
                        float _y) const;

      void  autoResizeStrokeBuffer();

      proj::Tuning const& tuning_;
      QRectF rect_;
      float  gamma_;
      BlendBrush brush_;
      Buffer     strokeBuffer_;
  };
}

OMNI_DECL_STREAM_OPERATORS(omni::BlendMask)

#endif /* OMNI_BLEND_MASK_H_ */
