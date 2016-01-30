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

#include "proj/TitleBar.h"
#include "proj/Tuning.h"

#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QHBoxLayout>
#include <QColorDialog>

namespace omni
{
  namespace ui
  {
    namespace proj
    {
      TitleBar::TitleBar(Tuning* _tuning) :
        slim::Widget(_tuning),
        tuningWidget_(_tuning)
      {
        setup();
      }

      TitleBar::TitleBar(QString const& _label, Tuning* _tuning) :
        slim::Widget(_label,_tuning),
        tuningWidget_(_tuning)
      {
        setup();
      }

      TitleBar::~TitleBar()
      {
      }

      void TitleBar::setColor(QColor const& _color)
      {
        color_=_color;
        emit colorChanged(_color);
        update();
      }

      void TitleBar::selectColor()
      {
        QColorDialog _colorDialog;
        _colorDialog.setCurrentColor(color());
        _colorDialog.setOptions(
                /* do not use native dialog */
                QColorDialog::DontUseNativeDialog);

        if (_colorDialog.exec())
          setColor(_colorDialog.selectedColor());

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

      void TitleBar::paintEvent(QPaintEvent* event)
      {
        QPainter _p(this);

        auto _rect = rect().adjusted(1,1,-1,-1);
        _p.setPen(Qt::NoPen);
        _p.setBrush(color());
        _p.drawRect(_rect);
        QWidget::paintEvent(event);
      }

      void TitleBar::setup()
      {
        this->label_->setAlignment(Qt::AlignCenter);

        layout()->removeWidget(this->label_);

        ///////////////////// Setup buttons
        auto setupToolButton = [this](QUniquePtr<QToolButton>& _btn)
        {
          _btn.reset(new QToolButton());
          _btn->setAutoRaise(true);
          _btn->setStyleSheet("background : transparent");
          _btn->installEventFilter(this->parent());
          _btn->installEventFilter(this);
        };

        setupToolButton(menuButton_);

        menuButton_->setArrowType(Qt::DownArrow);

        menu_.reset(new QMenu);

        /// Generate and add popup menu
        menuButton_->setMenu(menu_.get());
        menuButton_->setPopupMode(QToolButton::InstantPopup);
        auto* _changeColor = menu_->addAction("Change color...");
        connect(_changeColor,SIGNAL(triggered()),this,SLOT(selectColor()));

        menu_->addSeparator();
        auto* _peripheral = menu_->addAction("Peripheral Setup");
        _peripheral->setCheckable(true);

        auto* _free = menu_->addAction("Free Setup");
        _free->setCheckable(true);

        setupToolButton(displayButton_);
        displayButton_->setCheckable(true);
        displayButton_->setIcon(QIcon(":/qss_icons/undock.png"));

        connect(displayButton_.get(),SIGNAL(clicked(bool)),tuningWidget(),SLOT(fullscreenToggle(bool)));

        setupToolButton(maximizeButton_);
        maximizeButton_->setIcon(QIcon(":/qss_icons/up_arrow.png"));
        connect(maximizeButton_.get(),SIGNAL(clicked()),tuningWidget(),SLOT(setNextWindowState()));

        setupToolButton(closeButton_);
        closeButton_->setIcon(QIcon(":/qss_icons/close.png"));
        connect(closeButton_.get(),SIGNAL(clicked()),this,SIGNAL(closeButtonClicked()));

        ///////////////////// END Setup buttons

        /// Add widget in this left-to-right order to layout
        layout()->addWidget(menuButton_.get());
        layout()->addWidget(displayButton_.get());
        layout()->addWidget(this->label_);
        layout()->addWidget(maximizeButton_.get());
        layout()->addWidget(closeButton_.get());
        layout()->setSpacing(0);
        layout()->setContentsMargins(0,0,0,0);

        if (tuningWidget_ && tuningWidget_->tuning())
          setColor(tuningWidget_->tuning()->color());
      }

      Tuning* TitleBar::tuningWidget()
      {
        return tuningWidget_;
      }

    }
  }
}
