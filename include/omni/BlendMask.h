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

#ifndef OMNI_BLENDMASK_H_
#define OMNI_BLENDMASK_H_

#include <vector>
#include <QRectF>
#include <omni/BlendBrush.h>

namespace omni {
  namespace proj {
    class Tuning;
  }

  class BlendBrush;

  /**@brief A blend mask consists of an edge mask and stroke mask with a brush
   * @detail Edge mask and stroke mask are stored in different buffers
   */
  class BlendMask : public QObject, public Serializer<BlendMask> {
      Q_OBJECT
      Q_PROPERTY(QRectF rect READ rect WRITE setRect NOTIFY rectChanged)
      Q_PROPERTY(qreal topWidth READ topWidth WRITE setTopWidth NOTIFY topWidthChanged STORED false)
      Q_PROPERTY(qreal bottomWidth READ bottomWidth WRITE setBottomWidth NOTIFY bottomWidthChanged STORED false)
      Q_PROPERTY(qreal leftWidth READ leftWidth WRITE setLeftWidth NOTIFY leftWidthChanged STORED false)
      Q_PROPERTY(qreal rightWidth READ rightWidth WRITE setRightWidth NOTIFY rightWidthChanged STORED false)
      Q_PROPERTY(qreal gamma READ gamma WRITE setGamma NOTIFY gammaChanged)
      Q_PROPERTY(BlendBrush* brush READ brush CONSTANT)
    public:
      /// Resolution of blend mask
      inline static constexpr int resolution() {
        return 1024;
      }

      typedef Buffer<uint8_t> buffer_type;

      BlendMask(QObject* parent = nullptr);

      /// Clears stroke buffer
      Q_INVOKABLE void clear();

      /**@brief Sets rect of blends
       *@detail Clamps rect to borders 0,0,1,1 if its overlapping
       **/
      void setRect(QRectF const&);

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

      /// Return blend brush
      BlendBrush*       brush() const;

      /// Stamp on stroke buffer with current brush at position x y
      void              stamp(const QPointF& _pos);

      /**@brief Draws a line on stroke buffer
       * @returns new left over value
       */
      float             drawLine(QPointF const& _p0,
                                 QPointF const& _p1,
                                 float          _leftOver = 0.0);

    signals:
      void rectChanged();
      void topWidthChanged();
      void bottomWidthChanged();
      void leftWidthChanged();
      void rightWidthChanged();
      void gammaChanged();

    private:
      /// Transform point from tuning coordinates to stroke buffer coordinates
      QPointF transformedPoint(QPointF const&) const;

      /// Returns blend value for mask buffer at position x y
      float borderValue(float _x,
                        float _y) const;

      QRectF rect_;
      float  gamma_;
      BlendBrush* brush_ = nullptr;
      buffer_type     strokeBuffer_;
  };
}

#endif /* OMNI_BLEND_MASK_H_ */
