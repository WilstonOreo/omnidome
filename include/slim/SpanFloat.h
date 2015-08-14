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

#ifndef SLIM_SPANFLOAT_H_
#define SLIM_SPANFLOAT_H_

#include <QSpinBox>
#include "AbstractInputWidget.h"
#include "mixin/Span.hpp"

namespace slim
{
  class SpanFloat :
    public AbstractInputWidget,
    public mixin::Span<SpanFloat,double>
  {
    Q_OBJECT
  public:
    typedef QSpinBox editor_type;
    typedef mixin::Span<SpanFloat,double> mixin_span_type;

    SpanFloat(QWidget* = nullptr);
    SpanFloat(QString const& _label, double _lower, double _upper, double _min, double _max, QWidget* = nullptr);
    ~SpanFloat();

  public slots:
    void setLowerValue(double);
    void setUpperValue(double);
    void setMinimum(double);
    void setMaximum(double);
    void setRange(double,double);
    void setLowerEditorVisible(bool);
    void setUpperEditorVisible(bool);
  signals:
    void lowerChanged();
    void upperChanged();
    void rangeChanged();

  protected:
    void paintEvent(QPaintEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);

    void lowerChangedEvent();
    void upperChangedEvent();
    void rangeChangedEvent();

  private:
    void init();
    double fromPos(double) const;
    double lowerToPos() const;
    double upperToPos() const;

    enum MoveState
    {
      NONE,
      LOWER_MOVING,
      UPPER_MOVING
    } moveState_;
  };
}

#endif /* SLIM_SPANFLOAT_H_ */
