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

#ifndef OMNI_UI_RANGEDINT_H_
#define OMNI_UI_RANGEDINT_H_

#include <QSpinBox>
#include "AbstractInputWidget.h"
#include "mixin/RangedValue.h"

namespace omni {
  namespace ui {
    /// Integer slider
    class OMNIWIDGET_EXPORT RangedInt :
      public AbstractInputWidget,
      public mixin::RangedValue<RangedInt, int>{
        Q_OBJECT
      public:
        typedef QSpinBox                          editor_type;
        typedef mixin::RangedValue<RangedInt, int>mixin_range_type;

        RangedInt(QWidget * = nullptr);
        RangedInt(QString const& _label,
                  int _value,
                  int _min,
                  int _max,
                  QWidget * = nullptr);
        ~RangedInt();

        /// Return flag if default value is used
        bool   useDefaultValue() const;

        bool   drawTicks() const;

      public slots:
        void   setValue(int);
        void   setMinimum(int);
        void   setMaximum(int);
        void   setRange(int,
                        int);
        void   setEditorVisible(bool);

        /// Use Default Value and show it
        void   setUseDefaultValue(bool);

        void   setDrawTicks(bool);

      signals:
        void   rangeChanged();

      protected:
        void   paintEvent(QPaintEvent *);
        void   keyPressEvent(QKeyEvent *);
        void   mouseMoveEvent(QMouseEvent *);
        void   mousePressEvent(QMouseEvent *);
        void   mouseReleaseEvent(QMouseEvent *);
        void   wheelEvent(QWheelEvent *);

        void   valueChangedEvent();
        void   rangeChangedEvent();

      private:
        void   init();
        void   editorSetup();
        int    valueFromPos(double) const;
        int    valueToPos() const;

        /// Calculate position from value
        double valueToPos(int) const;

        bool drawTicks_       = true;
        bool useDefaultValue_ = false;
        bool moving_          = false;
    };
  }
}

#endif /* OMNI_UI_RANGEDINT_H_ */
