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

#include <omni/ui/Widget.h>

#include <QHBoxLayout>
#include <QLabel>
#include <QStyle>
#include <QStyleOption>
#include <QPainter>

namespace omni {
    namespace ui {
        Widget::Widget(QWidget *_parent) :
            QWidget(_parent),
            colorSet_(this)
        {
            init();
        }

        Widget::Widget(QString const& _label, QWidget *_parent) :
            QWidget(_parent),
            colorSet_(this)
        {
            init();
            setLabel(_label);
        }

        QString Widget::label() const
        {
            return label_->text();
        }

        void Widget::setLabel(QString const& _label)
        {
            label_->setText(_label);
            update();
        }

        ColorSet const& Widget::colorSet() const
        {
            return colorSet_;
        }

        void Widget::init()
        {
            setWindowFlags(Qt::FramelessWindowHint);
            setMinimumSize(0, 24 / devicePixelRatio());
            setMaximumSize(16000, 48 / devicePixelRatio());
            setFocusPolicy(Qt::TabFocus);
            label_ = new QLabel("", this);
            label_->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            label_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            label_->setStyleSheet(
              "* { font-family: \"Dosis\"; font: light \"Dosis\"; } "
              "QLabel { background : transparent; color: #808080; } ");
            label_->setPalette(this->palette());

            QLayout *_layout = new QHBoxLayout(this);
            _layout->setContentsMargins(4, 1, 4, 1);
            _layout->addWidget(label_);
            setLayout(_layout);
        }

        void Widget::drawBorder(QPainter& _p)
        {
            drawBorder(_p, hasFocus());
        }

        void Widget::drawBorder(QPainter& _p, bool _focus)
        {
            auto && _rect = rect();

            QStyleOption _option;
            _option.initFrom(this);

            _p.setPen(_focus ? QPen(QBrush(_option.palette.color(colorSet().
                                                                 colorGroup(),
                                                                 QPalette::Dark)),
                                    2) : Qt::NoPen);
            _p.setBrush(Qt::NoBrush);
            _p.drawRect(_rect);
        }
    }
}
