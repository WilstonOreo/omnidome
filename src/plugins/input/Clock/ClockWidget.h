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

#ifndef OMNI_UI_INPUT_CLOCK_H_
#define OMNI_UI_INPUT_CLOCK_H_

#include <omni/ui/ParameterWidget.h>
#include <omni/ui/InputPreview.h>
#include <QCheckBox>
#include <QToolButton>
#include <QTimeEdit>

namespace omni {
  namespace input {
    class Clock;
  }

  namespace ui {
    namespace input {
      class Clock : public ParameterWidget {
        Q_OBJECT
      public:
        Clock(omni::input::Clock*, QWidget* = nullptr);
        ~Clock();

      public slots:
        void resetCountDown();
        void setCountDown(QTime const&);
        void triggerUpdate();
        void useSystemTime(bool);
        void pauseCountDown(bool);

      signals:
        void inputChanged();

      private:
        void setup();

        omni::input::Clock* input_ = nullptr;

        QUniquePtr<QCheckBox> chkUseSystem_;
        QUniquePtr<QTimeEdit> editCountDownTime_;
        QUniquePtr<QToolButton> btnResetCountDown_;
        QUniquePtr<QToolButton> btnPauseCountDown_;
        QUniquePtr<InputPreview> preview_;
      };
    }
  }
}

#endif /* OMNI_INPUT_CLOCK_H_ */
