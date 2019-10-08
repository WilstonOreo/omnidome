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

#include <omni/ui/Dial.h>

#include <cmath>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QVector2D>

namespace omni {
    namespace ui {
        Dial::Dial(QWidget *_parent) :
            QWidget(_parent),
            mixin_range_type(0, 0, 100),
            mixin_editor_type(this),
            colorSet_(this)
        {
            init();
        }

        Dial::Dial(value_type _value,
                   value_type _minimum,
                   value_type _maximum,
                   QWidget   *_parent) :
            QWidget(_parent),
            mixin_range_type(_value, _minimum, _maximum),
            mixin_editor_type(this),
            colorSet_(this)
        {
            init();
        }

        Dial::~Dial()
        {}

        bool Dial::showTicks() const
        {
            return showTicks_;
        }

        bool Dial::isMoving() const
        {
            return isMoving_;
        }

        QString const& Dial::suffix() const
        {
            return suffix_;
        }

        void Dial::setShowTicks(bool _showTicks)
        {
            showTicks_ = _showTicks;
            update();
        }

        void Dial::init()
        {
            setMinimumSize(32 / devicePixelRatio(), 96 / devicePixelRatio());
            setMaximumSize(16000, 320 / devicePixelRatio());

            valueLabel_ = new QLabel(this);
            valueLabel_->setAlignment(Qt::AlignCenter);
            valueLabel_->setStyleSheet("background : transparent; color: #808080;");

            mixin_editor_type::createEditor<>();

            valueChangedEvent();
        }

        void Dial::setEditorVisible(bool _visible)
        {
            mixin_editor_type::setEditorVisible(_visible);

            valueLabel_->setVisible(!_visible);

            if (!_visible)
            {
                setFocus();
                return;
            }
        }

        void Dial::editorSetup() {
            auto _editor = this->editorAs<editor_type>();

            connect(_editor, SIGNAL(valueChanged(double)), this,
                    SLOT(setValue(double)));
            _editor->setFrame(false);
            _editor->setAlignment(Qt::AlignRight);
            _editor->setButtonSymbols(QAbstractSpinBox::PlusMinus);
            mixin_range_type::apply(_editor);
            valueChangedEvent();
        }

        void Dial::setLetter(QChar c)
        {
            letter_ = c;
            update();
        }

        void Dial::setSuffix(QString const& _s)
        {
            suffix_ = _s;
            valueLabel_->setText(QString("%1%2").arg(int(value())).arg(suffix()));
            update();
        }

        void Dial::setValue(double _value)
        {
            mixin_range_type::setValue(_value);
        }

        void Dial::setValue(int _value)
        {
            mixin_range_type::setValue(_value);
        }

        void Dial::setMinimum(double _min)
        {
            mixin_range_type::setMinimum(_min);
        }

        void Dial::setMaximum(double _max)
        {
            mixin_range_type::setMaximum(_max);
        }

        void Dial::setRange(double _min, double _max)
        {
            mixin_range_type::setRange(_min, _max);
        }

        void Dial::setSingleStep(double _singleStep)
        {
            mixin_range_type::setSingleStep(_singleStep);

            update();
        }

        void Dial::setPageStep(double _pageStep)
        {
            mixin_range_type::setPageStep(_pageStep);

            update();
        }

        ColorSet const& Dial::colorSet() const
        {
            return colorSet_;
        }

        void Dial::valueChangedEvent()
        {
            valueLabel_->setText(QString("%1%2").arg(int(value())).arg(suffix()));
            emit valueChanged();
            update();
        }

        void Dial::rangeChangedEvent()
        {
            emit rangeChanged();

            update();
        }

        void Dial::resizeEvent(QResizeEvent *e)
        {
            valueLabel_->setGeometry(rect());
            if (editor()) {
                editor()->move(rect().center() -
                    QPoint(editor()->width()*0.5,editor()->height()*0.5));
            }
        }

        void Dial::mousePressEvent(QMouseEvent *e)
        {
            setFocus();

            float _d = QVector2D(rect().center() - e->pos()).length();
            isMoving_ = (_d < radius()) && (_d > radius()*0.6);

            if (isMoving_)
            {
                pos_ = e->pos();
                setValue(getValue(e->pos()));
            }
        }

        void Dial::mouseReleaseEvent(QMouseEvent *e)
        {
            if (isMoving_)
            {
                pos_ = e->pos();
                setValue(getValue(e->pos()));
            }
            isMoving_ = false;
        }

        void Dial::mouseMoveEvent(QMouseEvent *e)
        {
            if (isMoving_)
            {
                if (!minimumUsed() || !maximumUsed())
                {
                    auto _center = rect().center();

                    if ((pos_.x() > _center.x()) &&
                        (e->pos().x() <= _center.x()) &&
                        (e->pos().y() > _center.y()))
                    {
                        mixin_range_type::setValue(
                            value() + mixin_range_type::range());
                    }

                    if ((pos_.x() <= _center.x()) &&
                        (e->pos().x() > _center.x()) &&
                        (e->pos().y() > _center.y()))
                    {
                        mixin_range_type::setValue(
                            value() - mixin_range_type::range());
                    }
                }

                pos_ = e->pos();
                setValue(getValue(e->pos()));
            }
        }

        void Dial::mouseDoubleClickEvent(QMouseEvent *event)
        {
            float _d = QVector2D(rect().center() - event->pos()).length();

            if ((_d < radius()) && (_d > radius()*0.6)) {
                setValue(minimum());
            }
            // Show editor when click in the middle
            if (_d < radius()*0.4) {
                setEditorVisible(!editor()->isVisible());
            } else {
                setEditorVisible(false);
            }
        }

        void Dial::keyPressEvent(QKeyEvent* _event) {
            if (((_event->key() == Qt::Key_Enter) ||
                 (_event->key() == Qt::Key_Return)))
            {
                setEditorVisible(!editor()->hasFocus());
            }

            float _step = singleStep();

            if (_event->modifiers() & Qt::ShiftModifier) {
                _step = 1.0;
            }
            if (_event->modifiers() & Qt::ControlModifier) {
                _step = pageStep();
            }

            if (_event->key() == Qt::Key_Left) {
                setValue(value() - _step);
            }
            if (_event->key() == Qt::Key_Right) {
                setValue(value() + _step);
            }
        }

        double Dial::getValue(QPoint const& _point)
        {
            auto  _v     = QVector2D(_point - rect().center()).normalized();
            float _angle = atan2(_v.x(), -_v.y()) / M_PI * 0.5;

            _angle = (_angle + 0.5) * (maximum() - minimum()) + minimum();
            return _angle;
        }

        void Dial::paintEvent(QPaintEvent *e)
        {
            auto && _rect = rect();
            QPointF _center = _rect.center();

            double   _r = radius() - radius() / 50.0;
            QPainter _p(this);
            _p.setRenderHint(QPainter::Antialiasing);
            _p.setBrush(colorSet().button());
            _p.setPen(hasFocus() ? QPen(colorSet().dark(),
                                        radius() / 50.0) : Qt::NoPen);

            _p.drawEllipse(_center, _r, _r);

            QPainterPath _ellipsePath, _piePath;
            _ellipsePath.addEllipse(_center, _r,        _r);
            _ellipsePath.addEllipse(_center, _r * 0.75, _r * 0.75);
            auto && _pieRect = _ellipsePath.boundingRect().adjusted(-2, -2, 2, 2);

            _p.setBrush(
                hasFocus() ?
                colorSet().highlight() :
                colorSet().dark());

            QPainterPath _arc(_center);
            _arc.arcTo(_pieRect, -90, -ratio() * 360);
            _arc.lineTo(_center);
            _arc.closeSubpath();

            _piePath = _arc & _ellipsePath;
            _p.drawPath(_piePath);

            if (showTicks())
            {
                mixin_range_type::for_each_step([&](int _step, double i,
                                                    bool _isPage)
                {
                    _p.setPen(QPen((i >= mixin_range_type::value() ?
                                   colorSet().light() :
                                   colorSet().windowText()).lighter(150),
                                   1.5));
                    paintTick(_p, i, _isPage ? 0.25 : 0.125);
                });
            }

            _p.setPen(QPen(colorSet().shadow(), _r / 50.0));
            paintTick(_p, mixin_range_type::value(), 0.333);
            paintTick(_p, 0.0,                       0.25);


            // Draw big letter in the middle
            _p.setPen(QPen(QColor("#3d3d3d"), 2));
            QFont _font = font();
            _font.setPixelSize(_r);
            _p.setFont(_font);
            _p.drawText(_rect, Qt::AlignCenter, letter_);
        }

        double Dial::radius() const
        {
            double _r = width() > height() ? height() : width();

            return _r * 0.5;
        }

        void Dial::paintTick(QPainter& _p, double _value, double _length)
        {
            auto && _rect = rect();
            auto _center = QVector2D(_rect.center());

            auto _theta = mixin::Range<value_type>::ratio(_value) * M_PI * 2.0;

            QVector2D _a(-sin(_theta), cos(_theta));
            _a *= radius() - radius() / 50.0;
            QVector2D _b = _a * (1.0 - _length);
            _a += _center;
            _b += _center;

            _p.drawLine(_a.toPointF(), _b.toPointF());
        }
    }
}
