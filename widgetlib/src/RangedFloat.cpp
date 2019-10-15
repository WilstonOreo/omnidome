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

#include <omni/ui/RangedFloat.h>

#include <QDebug>
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>

namespace omni {
    namespace ui {
        RangedFloat::RangedFloat(QWidget *_parent) :
            AbstractInputWidget(_parent)
        {
            init();
        }

        RangedFloat::RangedFloat(QString const& _label,
                                 double         _value,
                                 double         _min,
                                 double         _max,
                                 QWidget       *_parent) :
            AbstractInputWidget(_parent),
            mixin_range_type(_value, _min, _max)
        {
            init();
            setLabel(_label);
        }

        RangedFloat::~RangedFloat()
        {}

        QString RangedFloat::suffix() const
        {
            return suffix_;
        }

        int RangedFloat::precision() const
        {
            return precision_;
        }

        void RangedFloat::valueChangedEvent()
        {
            mixin_range_type::apply(editorAs<editor_type>());
            AbstractInputWidget::valueLabel_->setText(QString("%1%2")
                                                      .arg(QString::number(
                                                               value(),
                                                               'f',
                                                               precision_))
                                                      .arg(suffix()));

            update();
            emit valueChanged();
        }

        void RangedFloat::rangeChangedEvent()
        {
            mixin_range_type::apply(editorAs<editor_type>());
            emit rangeChanged();
        }

        void RangedFloat::setValue(double _value)
        {
            mixin_range_type::setValue(_value);
        }

        void RangedFloat::setMinimum(double _min)
        {
            mixin_range_type::setMinimum(_min);
        }

        void RangedFloat::setMaximum(double _max)
        {
            mixin_range_type::setMaximum(_max);
        }

        void RangedFloat::setRange(double _min, double _max)
        {
            mixin_range_type::setRange(_min, _max);
        }

        void RangedFloat::setSnap(bool _snap)
        {
            mixin_range_type::setSnap(_snap);
        }

        void RangedFloat::setPivot(double _pivot)
        {
            mixin_range_type::setPivot(_pivot);

            update();
        }

        void RangedFloat::setDefaultValue(double _defaultValue)
        {
            mixin_range_type::setDefaultValue(_defaultValue);

            update();
        }

        void RangedFloat::setSingleStep(double _singleStep)
        {
            mixin_range_type::setSingleStep(_singleStep);

            update();
        }

        void RangedFloat::setPageStep(double _pageStep)
        {
            mixin_range_type::setPageStep(_pageStep);

            update();
        }

        bool RangedFloat::useDefaultValue() const
        {
            return useDefaultValue_;
        }

        void RangedFloat::setUseDefaultValue(bool _defaultValue)
        {
            useDefaultValue_ = _defaultValue;
            update();
        }

        bool RangedFloat::drawTicks() const
        {
            return drawTicks_;
        }

        void RangedFloat::setDrawTicks(bool _ticks)
        {
            drawTicks_ = _ticks;
            update();
        }

        void RangedFloat::setPrecision(int _precision)
        {
            precision_ = _precision;
            editorAs<editor_type>()->setDecimals(_precision);
            valueChangedEvent();
        }

        void RangedFloat::setSuffix(QString const& _suffix)
        {
            suffix_ = _suffix;
            editorAs<editor_type>()->setSuffix(_suffix);
            valueChangedEvent();
        }

        double RangedFloat::valueFromPos(double _pos) const
        {
            auto && _rect = rect();
            if (scale_ == Scale::RECIPROCAL) {
                if (minimum() <= 1.0 && maximum() >= 1.0 && minimum() > 0.0) {

                    double _recMinimum = - 1.0 / minimum();
                    double _max = maximum();

                    double _v = (_pos - _rect.left()) / _rect.width() *
                            (maximum() - _recMinimum) + _recMinimum;

                    if (_v < 0.0) {
                        _v = (_v - minimum()) * (1.0 / minimum() + maximum() /
                             (1.0 / minimum() - 1.0) )  - 1.0 / minimum();
                    }  else {
                        _v = (_v + 1.0) / (_max - minimum() + 1.0) * (_max - minimum());
                    }
                    return _v;
                }
            }

            return (_pos - _rect.left()) / _rect.width() *
                (maximum() - minimum()) + minimum();
        }

        double RangedFloat::valueToPos() const
        {
            return valueToPos(value());
        }

        /// Calculate position from value
        double RangedFloat::valueToPos(double _value) const
        {
            auto && _rect = rect();
            double _ratio = mixin::Range<double>::ratio(_value);

            if (scale_ == Scale::RECIPROCAL) {
                if (minimum() <= 1.0 && maximum() >= 1.0 && minimum() > 0.0) {
                    double _recMinimum = - 1.0 / minimum();
                    double _max = maximum();
                    double _v = (_value - 1) / (_max -1) * _max;
                    if (_value <= 1.0) {
                        _v *= 1.0 / minimum();
                    }
                    _ratio = (_v - _recMinimum) / (_max - _recMinimum);
                }
            }

            return _rect.left() + _ratio *
                    double(_rect.width());
        }

        void RangedFloat::paintEvent(QPaintEvent *_paintEvent)
        {
            if (!isVisible()) return;

            QPainter _p(this);
            _p.setRenderHint(QPainter::Antialiasing);
            double _pos = valueToPos();

            AbstractInputWidget::drawTrack(_p, valueToPos(pivot()), _pos);

            if (useDefaultValue()) AbstractInputWidget::drawHandle(_p,
                                                                   valueToPos(
                                                                       defaultValue()),
                                                                   0.5);

            if (drawTicks())
            {
                mixin_range_type::for_each_step([&](int _step, double i,
                                                    bool _isPage)
                {
                    _p.setPen(QPen(i >= mixin_range_type::value() ?
                                   colorSet().shadow() :
                                   colorSet().windowText(), 1));

                    double _pos = valueToPos(i);
                    double _sizeFactor = _isPage ? 0.3 : 0.15;
                    _p.drawLine(
                        QPointF(_pos,
                                rect().bottom() - rect().height() * _sizeFactor),
                        QPointF(_pos, rect().bottom()));
                });
            }
            AbstractInputWidget::drawBorder(_p,
                                            hasFocus() || editor()->hasFocus());
        }

        void RangedFloat::keyPressEvent(QKeyEvent* _event) {
            AbstractInputWidget::keyPressEvent(_event);
            float _step = pageStep();

            if (_event->modifiers() & Qt::ShiftModifier) {
                _step = singleStep();
            }
            if (_event->modifiers() & Qt::ControlModifier) {
                _step *= 10.0;
            }

            if (_event->key() == Qt::Key_Left) {
                setValue(value() - _step);
            }
            if (_event->key() == Qt::Key_Right) {
                setValue(value() + _step);
            }
        }

        void RangedFloat::mouseMoveEvent(QMouseEvent *e)
        {
            if (moving_)
            {
                setValue(valueFromPos(e->pos().x()));
            }
        }

        void RangedFloat::mousePressEvent(QMouseEvent *e)
        {
            AbstractInputWidget::mousePressEvent(e);

            if (std::abs(e->pos().x() - valueToPos()) <= gripSize_ || !gripSize_)
            {
                moving_ = true;
                setValue(valueFromPos(e->pos().x()));
            }
        }

        void RangedFloat::mouseReleaseEvent(QMouseEvent *e)
        {
            moving_ = false;
        }

        void RangedFloat::mouseDoubleClickEvent(QMouseEvent *e)
        {
            if (useDefaultValue())
            {
                if (std::abs(e->pos().x() - valueToPos(defaultValue())) <= 4)
                {
                    mixin_range_type::reset();
                    return;
                }
            }

            AbstractInputWidget::mouseDoubleClickEvent(e);
        }

        void RangedFloat::wheelEvent(QWheelEvent *e)
        {
            if (hasFocus()) setValue(
                    value() + e->delta() / 15.0 / 8.0 * singleStep());
        }

        void RangedFloat::setEditorVisible(bool _visible)
        {
            AbstractInputWidget::setEditorVisible(_visible);
            mixin_range_type::apply(editorAs<editor_type>());
        }

        void RangedFloat::init()
        {
            setSingleStep(1.0);
            AbstractInputWidget::createEditor<editor_type>();
        }

        RangedFloat::Scale RangedFloat::scale() const {
            return scale_;
        }

        void RangedFloat::setScale(Scale _scale) {
            scale_ = _scale;
            update();
        }

        void RangedFloat::editorSetup() {
            auto _editor = this->editorAs<editor_type>();

            connect(_editor, SIGNAL(valueChanged(double)), this,
                    SLOT(setValue(double)));
            _editor->setFrame(false);
            _editor->setAlignment(Qt::AlignRight);
            _editor->setButtonSymbols(QAbstractSpinBox::PlusMinus);
            mixin_range_type::apply(_editor);
            valueChangedEvent();
        }

        int RangedFloat::gripSize() const {
            return gripSize_;
        }

        void RangedFloat::setGripSize(int _gripSize) {
            gripSize_ = _gripSize;
        }
    }
}
