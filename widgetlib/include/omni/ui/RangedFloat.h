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

#ifndef OMNI_UI_RANGEDFLOAT_H_
#define OMNI_UI_RANGEDFLOAT_H_

#include <QDoubleSpinBox>
#include "mixin/RangedValue.h"
#include "AbstractInputWidget.h"

namespace omni {
  namespace ui {
    /// A slider with float value within a minimum and maximum
    class OMNIWIDGET_EXPORT RangedFloat :
      public AbstractInputWidget,
      private mixin::RangedValue<RangedFloat, double>{
        Q_OBJECT
      public:
        enum class Scale {
          LINEAR,
          RECIPROCAL
        };

        typedef mixin::RangedValue<RangedFloat, double>mixin_range_type;
        typedef QDoubleSpinBox                         editor_type;
        using mixin_range_type::minimum;
        using mixin_range_type::maximum;
        using mixin_range_type::pageStep;
        using mixin_range_type::singleStep;

        /// Construct from parent widget
        RangedFloat(QWidget * = nullptr);

        /**@brief Construct with input parameters
         * @param _label Label of widget
         * @param _value Initial value
         * @param _min   Minimal value
         * @param _max   Maximal value
         **/
        RangedFloat(QString const& _label,
                    double _value,
                    double _min,
                    double _max,
                    QWidget * = nullptr);
        virtual ~RangedFloat();

        using mixin_range_type::value;
        using mixin_range_type::defaultValue;
        using mixin_range_type::snap;
        using mixin_range_type::pivot;

        /// Return suffix
        QString suffix() const;

        /// Return precision
        int     precision() const;

        /// Return grip size
        int     gripSize() const;

        /// Return flag if default value is used
        bool    useDefaultValue() const;

        bool    drawTicks() const;

        Scale   scale() const;

      public slots:
        /// Set value, valueChanged() signal will be emitted
        void setValue(double);

        /// Set minimal value, rangeChanged() signal will be emitted
        void setMinimum(double);

        /// Set maximum value, rangeChanged() signal will be emitted
        void setMaximum(double);

        /// Set range (minimum/maximum), rangeChanged() signel will be emitted
        void setRange(double,
                      double);

        /// Show/hide editor
        void setEditorVisible(bool);

        /// Toggle value snap on/off
        void setSnap(bool);

        void setGripSize(int);

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

        void setScale(Scale);

      signals:
        void   rangeChanged();

      protected:
        void   paintEvent(QPaintEvent *);
        void   keyPressEvent(QKeyEvent *);
        void   mouseMoveEvent(QMouseEvent *);
        void   mousePressEvent(QMouseEvent *);
        void   mouseReleaseEvent(QMouseEvent *);
        void   mouseDoubleClickEvent(QMouseEvent *);
        void   wheelEvent(QWheelEvent *);

        void   valueChangedEvent();
        void   rangeChangedEvent();

      private:
        void   init();
        void   editorSetup();

        /// Get value from x mouse position
        double valueFromPos(double) const;

        /// Calculate position from value
        double valueToPos() const;

        /// Calculate position from value
        double valueToPos(double) const;

        Scale   scale_           = Scale::LINEAR;
        bool    moving_          = false;
        bool    drawTicks_       = false;
        bool    useDefaultValue_ = false;
        QString suffix_;
        int     gripSize_  = 0;
        int     precision_ = 2;
    };
  }
}

#endif /* OMNI_UI_RANGEDFLOAT_H_ */
