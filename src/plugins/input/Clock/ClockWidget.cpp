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

#include "ClockWidget.h"
#include "Clock.h"

namespace omni {
  namespace ui {
    namespace input {
      Clock::Clock(omni::input::Clock* _input, QWidget* _parent) :
        ParameterWidget(_parent),
        input_(_input) {
        setup();
      }

      Clock::~Clock() {

      }

      void Clock::setCountDown(QTime const& _time) {
        input_->setCountDown(_time.hour(),_time.minute(),_time.second());
        triggerUpdate();
      }

      void Clock::triggerUpdate() {
        if (!preview_) return;

        preview_->triggerUpdate();
      }

      void Clock::resetCountDown() {
        input_->resetCountDown();
        triggerUpdate();
      }

      void Clock::useSystemTime(bool _use) {
        input_->setUseSystemTime(_use);
        editCountDownTime_->setVisible(!_use);
        btnResetCountDown_->setVisible(!_use);
        btnPauseCountDown_->setVisible(!_use);
        triggerUpdate();
      }

      void Clock::pauseCountDown(bool _paused) {
        input_->pauseCountDown(_paused);
        triggerUpdate();
      }

      void Clock::setup() {
        QLayout *_layout = new QVBoxLayout;
        _layout->setSpacing(2);
        _layout->setContentsMargins(0, 0, 0, 0);
        _layout->setSizeConstraint(QLayout::SetMaximumSize);
        setLayout(_layout);

        preview_.reset(new InputPreview(input_));
        _layout->addWidget(preview_.get());

        chkUseSystem_.reset(new QCheckBox("Use system time"));

        _layout->addWidget(chkUseSystem_.get());

        editCountDownTime_.reset(new QTimeEdit());
        editCountDownTime_->setDisplayFormat("hh:mm:ss");
        editCountDownTime_->setTime(input_->countdownTime());

        connect(editCountDownTime_.get(),SIGNAL(timeChanged(QTime const&)),
        this,SLOT(setCountDown(QTime const&)));
        _layout->addWidget(editCountDownTime_.get());

        btnResetCountDown_.reset(new QToolButton());
        btnResetCountDown_->setText("Reset countdown");
        btnResetCountDown_->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Maximum);
        connect(btnResetCountDown_.get(),SIGNAL(clicked()),this,SLOT(resetCountDown()));
        _layout->addWidget(btnResetCountDown_.get());

        btnPauseCountDown_.reset(new QToolButton());
        btnPauseCountDown_->setText("Pause countdown");
        btnPauseCountDown_->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Maximum);
        btnPauseCountDown_->setCheckable(true);
        btnPauseCountDown_->setChecked(input_->countDownPaused());
        connect(btnPauseCountDown_.get(),SIGNAL(clicked(bool)),this,SLOT(pauseCountDown(bool)));
        _layout->addWidget(btnPauseCountDown_.get());

        bool _systemTimeUsed = input_->useSystemTime();
        chkUseSystem_->setChecked(_systemTimeUsed);
        connect(chkUseSystem_.get(),SIGNAL(clicked(bool)),this,SLOT(useSystemTime(bool)));
        editCountDownTime_->setVisible(!_systemTimeUsed);
        btnResetCountDown_->setVisible(!_systemTimeUsed);
        btnPauseCountDown_->setVisible(!_systemTimeUsed);
      }
    }
  }
}
