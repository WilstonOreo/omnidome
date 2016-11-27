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

#include "proj/TitleBar.h"
#include "proj/Tuning.h"

#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QHBoxLayout>
#include <QColorDialog>

namespace omni {
  namespace ui {
    namespace proj {
      TitleBar::TitleBar(Tuning *_tuning) :
        Widget(_tuning),
        tuningWidget_(_tuning)
      {
        setup();
      }

      TitleBar::TitleBar(QString const& _label, Tuning *_tuning) :
        Widget(_label, _tuning),
        tuningWidget_(_tuning)
      {
        setup();
      }

      TitleBar::~TitleBar()
      {}

      void TitleBar::setColor(QColor const& _color)
      {
        color_ = _color;
        emit colorChanged(_color);
        update();
      }

      void TitleBar::selectColor()
      {
        QColorDialog _colorDialog(color());

        if (_colorDialog.exec()) setColor(_colorDialog.selectedColor());
      }

      QColor TitleBar::color() const
      {
        return color_;
      }

      bool TitleBar::closeButtonVisible() const
      {
        return closeButton_ ? closeButton_->isVisible() : false;
      }

      void TitleBar::setCloseButtonVisible(bool _visible)
      {
        if (!closeButton_) return;

        closeButton_->setVisible(_visible);
      }

      void TitleBar::fullscreenToggle(bool _checked) {
        if (_checked) {
          displayButton_->setIcon(QIcon(":/icons/eye.png"));
        } else {
          displayButton_->setIcon(QIcon(":/icons/eye_crossed.png"));
        }
      }

      void TitleBar::paintEvent(QPaintEvent *event)
      {
        QPainter _p(this);

        auto _rect = rect().adjusted(1, 1, -1, -1);

        _p.setPen(Qt::NoPen);
        _p.setBrush(color());
        _p.drawRect(_rect);
        QWidget::paintEvent(event);
      }

      void TitleBar::setup()
      {
        this->label_->setAlignment(Qt::AlignCenter);
        this->label_->setStyleSheet("QLabel {"
                                    "  background: transparent; "
                                    "  color : #0e0e0e; "
                                    "  font-size : 10pt; "
                                    "}");

        layout()->removeWidget(this->label_);

        ///////////////////// Setup buttons
        auto setupToolButton = [this](QWidgetPtr<QToolButton>& _btn)
                               {
                                 _btn.reset(new QToolButton());
                                 _btn->setAutoRaise(true);
                                 _btn->setStyleSheet("QToolButton { "
                                                     "     background : transparent; "
                                                     "     border: 0px; "
                                                     " } "
                                                     "QToolButton::menu-indicator { image: none; }");
                                 _btn->installEventFilter(this->parent());
                                 _btn->installEventFilter(this);
                               };

        setupToolButton(menuButton_);

        menuButton_->setArrowType(Qt::DownArrow);

        menu_.reset(new QMenu);

        /// Generate and add popup menu
        menuButton_->setMenu(menu_.get());
        menuButton_->setPopupMode(QToolButton::InstantPopup);
        menuButton_->setArrowType(Qt::NoArrow);
        menuButton_->setIcon(QIcon(":/arrows/212121_90.png"));
        auto *_changeColor = menu_->addAction("Change color...");
        connect(_changeColor, SIGNAL(triggered()), this, SLOT(selectColor()));

        menu_->addSeparator();
        auto *_peripheral = menu_->addAction("Peripheral Setup");
        connect(_peripheral, SIGNAL(triggered()), this,
                SIGNAL(peripheralSetupSelected()));

        auto *_free = menu_->addAction("Free Setup");
        connect(_free, SIGNAL(triggered()), this, SIGNAL(freeSetupSelected()));

        setupToolButton(displayButton_);
        displayButton_->setCheckable(true);
        displayButton_->setChecked(true);
        displayButton_->setIcon(QIcon(":/icons/eye.png"));
        connect(displayButton_.get(), SIGNAL(clicked(bool)), tuningWidget(),
                SLOT(fullscreenToggle(bool)));

        setupToolButton(maximizeButton_);
        maximizeButton_->setIcon(QIcon(":/icons/maximize.png"));
        connect(maximizeButton_.get(), SIGNAL(clicked()), tuningWidget(),
                SLOT(setNextWindowState()));

        setupToolButton(closeButton_);
        closeButton_->setIcon(QIcon(":/icons/close.png"));
        connect(closeButton_.get(), SIGNAL(clicked()), this,
                SIGNAL(closeButtonClicked()));

        ///////////////////// END Setup buttons

        /// Add widget in this left-to-right order to layout
        layout()->addWidget(menuButton_.get());
        layout()->addWidget(displayButton_.get());
        layout()->addWidget(this->label_);
        layout()->addWidget(maximizeButton_.get());
        layout()->addWidget(closeButton_.get());
        layout()->setSpacing(0);
        layout()->setContentsMargins(0, 0, 0, 0);

        if (tuningWidget_ && tuningWidget_->tuning()) setColor(
            tuningWidget_->tuning()->color());
      }

      Tuning * TitleBar::tuningWidget()
      {
        return tuningWidget_;
      }
    }
  }
}
