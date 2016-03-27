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

#ifndef OMNI_UI_FULLSCREEN_H_
#define OMNI_UI_FULLSCREEN_H_

#include <QWidget>
#include <QScreen>
#include <omni/util.h>
#include "ScreenSetupDragWidget.h"

namespace omni {
  namespace ui {
    /// Widget for fullscreen display
    class FullScreen : public QWidget {
      Q_OBJECT

      public:
        FullScreen(const QScreen *_screen);
        ~FullScreen();

        void hideDragWidget();
        void showDragWidget(int _subIndex,
                            QColor const& _color);

        void attach(int _subIndex,
                    QWidget *_widget);
        void detach(int _subIndex);
        void detachAll();

      protected:
        void closeEvent(QCloseEvent *_event);

      private:
        void setGeometry(QWidget *,
                         int _subIndex) const;

        QScreen const *screen_;
        std::map<int, QWidget *> widgets_;
        QUniquePtr<ScreenSetupDragWidget> dragWidget_;
    };
  }
}


#endif /* OMNI_UI_FULLSCREEN_H_ */
