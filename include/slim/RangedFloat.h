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

#ifndef SLIM_RANGEDFLOAT_H_
#define SLIM_RANGEDFLOAT_H_

#include <QDoubleSpinBox>
#include "mixin/RangedValue.hpp"
#include "AbstractInputWidget.h"

namespace slim
{
  /// A slider with float value within a minimum and maximum
  class RangedFloat : 
    public AbstractInputWidget,
    private mixin::RangedValue<RangedFloat,double>
  {
    Q_OBJECT
  public:
    typedef mixin::RangedValue<RangedFloat,double> mixin_range_type;
    typedef QDoubleSpinBox editor_type;

    /// Construct from parent widget
    RangedFloat(QWidget* = nullptr);

    /**@brief Construct with input parameters
     * @param _label Label of widget
     * @param _value Initial value
     * @param _min   Minimal value
     * @param _max   Maximal value
     **/
    RangedFloat(QString const& _label, double _value, double _min, double _max, QWidget* = nullptr);
    virtual ~RangedFloat();

    using mixin_range_type::value;
    using mixin_range_type::defaultValue;
    using mixin_range_type::snap;
    using mixin_range_type::pivot;

    /// Return suffix
    QString suffix() const;

    /// Return precision
    int precision() const;

    /// Return flag if default value is used
    bool useDefaultValue() const;

    bool drawTicks() const;

  public slots:
    /// Set value, valueChanged() signal will be emitted
    void setValue(double);
   
    /// Set minimal value, rangeChanged() signal will be emitted
    void setMinimum(double);
    
    /// Set maximum value, rangeChanged() signal will be emitted
    void setMaximum(double);
    
    /// Set range (minimum/maximum), rangeChanged() signel will be emitted
    void setRange(double,double);
    
    /// Show/hide editor
    void setEditorVisible(bool);

    /// Toggle value snap on/off
    void setSnap(bool);

    /// Use Default Value and show it
    void setUseDefaultValue(bool);

    void setDrawTicks(bool);

    /// Set precision
    void setPrecision(int);

    /// Set Suffix
    void setSuffix(QString const&);

    /// Set pivot
    void setPivot(double);

    void setDefaultValue(double);

    void setSingleStep(double);

    void setPageStep(double);

  signals:
    void valueChanged();
    void rangeChanged();

  protected:
    void paintEvent(QPaintEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseDoubleClickEvent(QMouseEvent*);
    void wheelEvent(QWheelEvent*);
    
    void valueChangedEvent();
    void rangeChangedEvent();

  private:
    void init();
    /// Get value from x mouse position
    double valueFromPos(double) const;

    /// Calculate position from value
    double valueToPos() const;

    /// Calculate position from value
    double valueToPos(double) const;
    
    bool moving_ = false;
    bool drawTicks_ = true;
    bool useDefaultValue_ = false;
    QString suffix_;
    int precision_ = 2;
  };
}

#endif /* SLIM_RANGEDFLOAT_H_ */
