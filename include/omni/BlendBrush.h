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

#ifndef OMNI_BLENDBRUSH_H_
#define OMNI_BLENDBRUSH_H_

#include <QVector2D>
#include <omni/Buffer.h>
#include <omni/Serializer.h>
#include <omni/property.h>

class QPointF;
class QDataStream;

namespace omni {
  /**@brief BlendBrush for drawing on the blendmask
   * @detail Holds an internal pixel buffer
   **/
  class BlendBrush : public QObject, public Serializer<BlendBrush> {
      Q_OBJECT
      OMNI_PROPERTY_CLAMPED(qreal,feather,setFeather,1.0,0.0,10.0)
      OMNI_PROPERTY_CLAMPED(qreal,opacity,setOpacity,1.0,0.0,1.0)
      OMNI_PROPERTY_RW_DEFAULT(bool,inverted,setInverted,false)
      Q_PROPERTY(QVector2D size READ size WRITE setSize NOTIFY sizeChanged)
    public:
      /// Default constructor
      BlendBrush(QObject* = nullptr);

      /// Returns brush size
      QVector2D const& size() const;

      /**@brief Sets size of the brush.
       * @detail Minimum size is 2 pixels, maximum size is 512 pixels
       **/
      void  setSize(QVector2D const& _size);

      /**@brief Increase/Decrease size of the brush with a certain delta value
       * @detail Minimum size is 2 pixels, maximum size is 512 pixels
       **/
      void  changeSize(QVector2D const& _delta);


      /// Draws internal pixel buffer in given blend buffer
      void stamp(const QPointF& _pos,
                 Buffer<uint8_t>& _buf) const;

      /**@brief Draws a lot from point _p0 to _p1 on given pixel buffer
       *@param _p0 Start point
       *@param _p1 End point
       *@param _buf Buffer to be drawn
       *@param _leftOver Left over value to compensate positioning errors from
       *previous moves
       *@returns New left over value
       **/
      float drawLine(const QPointF& _p0,
                     const QPointF& _p1,
                     Buffer<uint8_t>& _buf,
                     float _leftOver = 0.0);

      Buffer<float>const& buffer() const;

    signals:
      void sizeChanged();
      void opacityChanged();
      void featherChanged();
      void invertedChanged();

    private:
      /// Generate internal pixel buffer with given size and feather parameters
      void generate();

      /// Keep brush size within certain limits
      static QVector2D clampBrushSize(QVector2D const&);

      /// Internal pixel buffer
      Buffer<float> buffer_;

      QVector2D size_;
  };
}

#endif /* OMNI_BLENDBRUSH_H_ */
