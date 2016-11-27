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

#include <omni/ui/RangedInt.h>

#include <QPainter>
#include <QStylePainter>
#include <QStyleOption>
#include <QMouseEvent>
#include <QWheelEvent>

namespace omni {
    namespace ui {
        RangedInt::RangedInt(QWidget *_parent) :
            AbstractInputWidget(_parent)
        {
            init();
        }

        RangedInt::RangedInt(QString const& _label,
                             int _value, int _min, int _max, QWidget *_parent) :
            AbstractInputWidget(_parent),
            mixin_range_type(_value, _min, _max)
        {
            setLabel(_label);
            init();
        }

        RangedInt::~RangedInt()
        {}

        void RangedInt::setValue(int _value)
        {
            mixin_range_type::setValue(_value);
            emit valueChanged();

            update();
        }

        void RangedInt::setMinimum(int _min)
        {
            mixin_range_type::setMinimum(_min);
        }

        void RangedInt::setMaximum(int _max)
        {
            mixin_range_type::setMaximum(_max);
        }

        void RangedInt::setRange(int _min, int _max)
        {
            mixin_range_type::setRange(_min, _max);
        }

        bool RangedInt::useDefaultValue() const
        {
            return useDefaultValue_;
        }

        void RangedInt::setUseDefaultValue(bool _defaultValue)
        {
            useDefaultValue_ = _defaultValue;
            update();
        }

        bool RangedInt::drawTicks() const
        {
            return drawTicks_;
        }

        void RangedInt::setDrawTicks(bool _ticks)
        {
            drawTicks_ = _ticks;
            update();
        }

        void RangedInt::valueChangedEvent()
        {
            mixin_range_type::apply(editorAs<editor_type>());
            AbstractInputWidget::valueLabel_->setText(QString("%1").arg(value()));

            update();
            emit valueChanged();
        }

        void RangedInt::rangeChangedEvent()
        {
            mixin_range_type::apply(editorAs<editor_type>());
            emit rangeChanged();
        }

        int RangedInt::valueFromPos(double _pos) const
        {
            auto && _rect = rect();
            return (_pos - _rect.left()) / _rect.width() * range() + minimum();
        }

        int RangedInt::valueToPos() const
        {
            auto && _rect = rect();
            return _rect.left() + ratio() * float(_rect.width());
        }

        double RangedInt::valueToPos(int _value) const
        {
            auto && _rect = rect();
            return _rect.left() + mixin::Range<int>::ratio(_value) *
                   double(_rect.width());
        }

        void RangedInt::paintEvent(QPaintEvent *_paintEvent)
        {
            QStylePainter _p(this);

            _p.setRenderHint(QPainter::Antialiasing);
            QStyleOption _option;
            _option.initFrom(this);

            double _pos = valueToPos();

            if (useDefaultValue()) AbstractInputWidget::drawHandle(_p,
                                                                   valueToPos(
                                                                       defaultValue()),
                                                                   0.5);


            AbstractInputWidget::drawTrack(_p, _pos);

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

        void RangedInt::keyPressEvent(QKeyEvent* _event) {
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

        void RangedInt::mouseMoveEvent(QMouseEvent *e)
        {
            if (moving_)
            {
                setValue(valueFromPos(e->pos().x()));
            }
        }

        void RangedInt::mousePressEvent(QMouseEvent *e)
        {
            AbstractInputWidget::mousePressEvent(e);

            if (abs(e->pos().x() - valueToPos()) <= 4)
            {
                moving_ = true;
            }
        }

        void RangedInt::mouseReleaseEvent(QMouseEvent *e)
        {
            moving_ = false;
        }

        void RangedInt::wheelEvent(QWheelEvent *e)
        {
            if (hasFocus()) setValue(
                    value() + e->delta() / 15.0 / 8.0 * singleStep());
        }

        void RangedInt::setEditorVisible(bool _visible)
        {
            AbstractInputWidget::setEditorVisible(_visible);
            mixin_range_type::apply(editorAs<editor_type>());
        }

        void RangedInt::init()
        {
            setSingleStep(1.0);
            AbstractInputWidget::createEditor<editor_type>();
        }

        void RangedInt::editorSetup() {
            auto _editor = this->editorAs<editor_type>();

            connect(_editor, SIGNAL(valueChanged(int)), this,
                    SLOT(setValue(int)));
            _editor->setFrame(false);
            _editor->setAlignment(Qt::AlignRight);
            _editor->setButtonSymbols(QAbstractSpinBox::PlusMinus);
            mixin_range_type::apply(_editor);
            valueChangedEvent();
        }
    }
}
