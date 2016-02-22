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

#ifndef OMNI_UI_DIAL_H_
#define OMNI_UI_DIAL_H_

#include <QWidget>
#include <QLabel>
#include <omni/ui/ColorSet.h>
#include <omni/ui/mixin/RangedValue.h>

namespace omni
{
        namespace ui
        {
                class Dial :
                public QWidget,
                public mixin::RangedValue<Dial,double>
                {
                        Q_OBJECT
                    public:
                        typedef mixin::RangedValue<Dial,double> mixin_range_type;
                        typedef typename mixin_range_type::value_type value_type;

                        Dial(QWidget* = nullptr);
                        Dial(value_type _value, value_type _minimum, value_type _maximum,QWidget* = nullptr);
                        ~Dial();

                        QString const& suffix() const;

                        QChar letter() const;

                        ColorSet const& colorSet() const;

                        bool isMoving() const;
                        bool showTicks() const;

                    public slots:
                        void setShowTicks(bool);
                        void setSuffix(QString const&);
                        void setLetter(QChar);

                        void setSingleStep(double);
                        void setPageStep(double);
                        void setMinimum(double);
                        void setMaximum(double);
                        void setRange(double,double);
                        void setValue(int);
                        void setValue(double);

                    signals:
                        void valueChanged();
                        void rangeChanged();

                    protected:
                        void resizeEvent(QResizeEvent*);
                        void paintEvent(QPaintEvent*);
                        void mousePressEvent(QMouseEvent*);
                        void mouseReleaseEvent(QMouseEvent*);
                        void mouseMoveEvent(QMouseEvent*);
                        void mouseDoubleClickEvent(QMouseEvent*);

                        void valueChangedEvent();
                        void rangeChangedEvent();

                    private:
                        bool isMoving_ = false;
                        bool showTicks_ = true;
                        QChar letter_ = 'X';
                        QString suffix_;
                        QLabel* valueLabel_;
                        ColorSet colorSet_;

                        void init();
                        QPointF pos_;

                        double radius() const;
                        double getValue(QPoint const&);
                        void paintTick(QPainter&, double _value, double _length);
                };
        }
}

#endif /* OMNI_UI_DIAL_H_ */
