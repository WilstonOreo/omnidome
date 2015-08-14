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

#ifndef SLIM_GRADIENTSLIDER_H_
#define SLIM_GRADIENTSLIDER_H_

#include <QLinearGradient>
#include "mixin/RangedValue.hpp"
#include "Widget.h"

namespace slim
{
  /** A slider with a value and a gradient **/
  class GradientSlider : 
    public Widget,
    private mixin::RangedValue<GradientSlider,double>
  {
    Q_OBJECT
  public:
    typedef mixin::RangedValue<GradientSlider,double> mixin_range_type;

    GradientSlider(QWidget* = nullptr);
    GradientSlider(QString const& _label, QWidget* = nullptr);
    ~GradientSlider();

    QLinearGradient& gradient();
    QLinearGradient const& gradient() const;
    void setGradient(QLinearGradient const&);
 
    using mixin_range_type::value;

    QColor color() const;
    bool showColorOnly() const;

  public slots:
    void setValue(double);
    void setShowColorOnly(bool);
  signals:
    void valueChanged();


  protected:
    void paintEvent(QPaintEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void wheelEvent(QWheelEvent*);

  private:    
    bool showColorOnly_ = false;
    bool moving_ = false;
    
    /// Calculate position from value
    double valueToPos() const;

    /// Calculate position from value
    double valueToPos(double) const;

    /// Get value from x mouse position
    double valueFromPos(double) const;
    
    QLinearGradient gradient_;
  };
}

#endif /* SLIM_GRADIENTSLIDER_H_ */
