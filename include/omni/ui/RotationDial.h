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

#ifndef OMNI_UI_ROTATIONDIAL_H_
#define OMNI_UI_ROTATIONDIAL_H_

#include <QWidget>
#include <QLabel>
#include <omni/ui/ColorSet.h>
#include <omni/ui/mixin/RangedValue.h>

namespace omni
{
    namespace ui {
  class RotationDial :
    public QWidget,
    public mixin::Range<double>,
    public mixin::Step<double>
  {
    Q_OBJECT
  public:
    typedef mixin::Range<double> mixin_range_type;
    typedef mixin::Step<double> mixin_step_type;

    RotationDial(QWidget* = nullptr);
    RotationDial(double _x, double _y, double _z, QWidget* = nullptr);
    ~RotationDial();

    ColorSet const& colorSet() const;

    double x() const;
    double y() const;
    double z() const;

    QColor xColor() const;
    QColor yColor() const;
    QColor zColor() const;

    bool isMoving() const;
    bool showTicks() const;

  public slots:
    void setShowTicks(bool);

    void setX(double);
    void setY(double);
    void setZ(double);

    void setXColor(QColor);
    void setYColor(QColor);
    void setZColor(QColor);

  signals:
    void xChanged();
    void yChanged();
    void zChanged();

  protected:
    void resizeEvent(QResizeEvent*);
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);

    void valueChangedEvent();

  private:
    enum MoveState
    {
      MOVE_NONE,
      MOVE_X,
      MOVE_Y,
      MOVE_Z
    } moveState_ = MOVE_NONE;

    bool showTicks_ = true;
    QLabel* valueLabel_;
    ColorSet colorSet_;

    void init(double = 0.0,double = 0.0,double = 0.0);
    QPointF pos_;

    double radius() const;

    /// Set value according to move state
    void setValue(double);

    /// Get angle from point
    double getValue(QPoint const&);
    void paintTick(QPainter&, double _inner, double _outer, double _value);

    void paintPie(QPainter&, double _innerRadius, double _outerRadius, double _value, QColor _color);
    void paintPies(QPainter&);

    static QString formatDegrees(double);
    static QString moveStateToText(MoveState);


    double x_, y_, z_;
    QColor xColor_ = "#2A76AC";
    QColor yColor_ = "#89CBF9";
    QColor zColor_ = "#E8F0F5";
  };
}
}

#endif /* FUTON_UI_DIAL_H_ */
