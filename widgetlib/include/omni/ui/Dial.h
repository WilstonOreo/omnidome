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

#ifndef OMNI_UI_DIAL_H_
#define OMNI_UI_DIAL_H_

#include <QWidget>
#include <QDoubleSpinBox>
#include <QLabel>
#include <omni/ui/ColorSet.h>
#include <omni/ui/mixin/RangedValue.h>
#include <omni/ui/mixin/Editor.h>

namespace omni {
  namespace ui {
    /// Circular dial widget with range value and editor
    class OMNIWIDGET_EXPORT Dial :
      public QWidget,
      public mixin::RangedValue<Dial, double>,
      protected mixin::Editor<Dial, QDoubleSpinBox>{
        Q_OBJECT

      public:
        /// Editor mixin type
        typedef mixin::Editor<Dial, QDoubleSpinBox>   mixin_editor_type;

        /// Ranged value mixin type
        typedef mixin::RangedValue<Dial, double>      mixin_range_type;
        typedef typename mixin_range_type::value_type value_type;

        /// Construct with optional parent widget
        Dial(QWidget * = nullptr);

        /// Construct with value and minimum and maximum values
        Dial(value_type _value,
             value_type _minimum,
             value_type _maximum,
             QWidget * = nullptr);

        /// Destructor
        ~Dial();

        /// Return suffix string
        QString const & suffix() const;

        /// Return letter shown in background
        QChar           letter() const;

        /// Return color set
        ColorSet const& colorSet() const;

        /// Is moving when mouse button is pressed
        bool            isMoving() const;

        /// Show ticks
        bool            showTicks() const;

      public slots:
        /// Set show ticks
        void            setShowTicks(bool);

        /// Set suffix string
        void            setSuffix(QString const&);

        /// Set letter
        void            setLetter(QChar);

        /// Set single step value
        void            setSingleStep(double);

        /// Set page step value
        void            setPageStep(double);

        /// Set minimum value
        void            setMinimum(double);

        /// Set maximum value
        void            setMaximum(double);

        /// Set range
        void            setRange(double,
                                 double);

        /// Set value from integer
        void            setValue(int);

        /// Set value
        void            setValue(double);

        /// Set visibility of editor
        void            setEditorVisible(bool);

      signals:
        void            valueChanged();
        void            rangeChanged();

      protected:
        /// Re-paint on resize
        void            resizeEvent(QResizeEvent *);
        void            paintEvent(QPaintEvent *);
        void            mousePressEvent(QMouseEvent *);
        void            mouseReleaseEvent(QMouseEvent *);
        void            mouseMoveEvent(QMouseEvent *);

        /**@brief Double click in center shows editor, double click on border reset to default value
         **/
        void            mouseDoubleClickEvent(QMouseEvent *);

        /// Show editor when enter key is pressed
        void            keyPressEvent(QKeyEvent *);

        void            valueChangedEvent();
        void            rangeChangedEvent();

      private:
        bool     isMoving_  = false;
        bool     showTicks_ = true;
        QChar    letter_    = 'X';
        QString  suffix_;
        QLabel  *valueLabel_;
        ColorSet colorSet_;

        void   init();
        void   editorSetup();
        QPointF pos_;

        double radius() const;

        /// Get value from point on widget
        double getValue(QPoint const&);

        /// Draw a tick with value and length
        void   paintTick(QPainter&,
                         double _value,
                         double _length);
    };
  }
}

#endif /* OMNI_UI_DIAL_H_ */
