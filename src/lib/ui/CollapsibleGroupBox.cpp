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

#include <omni/ui/CollapsibleGroupBox.h>
#include <QSizePolicy>
#include <QVBoxLayout>

namespace omni {
    namespace ui {
        CollapsibleGroupBox::CollapsibleGroupBox(QWidget* _parent) :
            QWidget(_parent)
        {
            auto* _layout = new QVBoxLayout(this);
            _layout->setAlignment(Qt::AlignTop);
            this->setLayout(_layout);
            button_ = new QToolButton(this);
            button_->setCheckable(true);
            button_->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            button_->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
            button_->setAutoRaise(true);
            widget_ = new QFrame(this);

            setSizePolicy(QSizePolicy::Expanding,QSizePolicy::MinimumExpanding);
            widget_->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
            widget_->setFrameShape(QFrame::StyledPanel);
            widget_->setMidLineWidth(2);
            widget_->setFrameShadow(QFrame::Sunken);

            _layout->addWidget(button_);
            _layout->addWidget(widget_);

            connect(button_,SIGNAL(clicked(bool)),this,SLOT(collapse(bool)));
        }

        CollapsibleGroupBox::~CollapsibleGroupBox() {
        }

        bool CollapsibleGroupBox::isCollapsed() const {
            return widget_->isVisible();
        }

        void CollapsibleGroupBox::collapse(bool _collapsed) {
            widget_->setVisible(!_collapsed);
        }

        QWidget* CollapsibleGroupBox::widget() {
            return widget_;
        }

        QWidget const* CollapsibleGroupBox::widget() const {
            return widget_;
        }

        QString CollapsibleGroupBox::title() {
            return button_->text();
        }

        void CollapsibleGroupBox::setTitle(QString const& _title) {
            button_->setText(_title);
        }
    }

}
