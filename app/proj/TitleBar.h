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

#ifndef OMNI_UI_PROJ_TITLEBAR_H
#define OMNI_UI_PROJ_TITLEBAR_H

#include <QToolButton>
#include <QMenu>
#include "Widget.h"
#include "mixin/RangedValue.h"
#include <omni/util.h>

namespace omni {
  namespace ui {
    namespace proj {
      class Tuning;

      /**@brief TitleBar for projector tuning widget
       * @detail Has four buttons:
       *         * projector setup selection and resetting
       *         * Fullscreen display toggle options
       *         * Minimizing/Maximizing tuning widget
       *         * Close tuning widget for deleting a projector tuning
       **/
      class TitleBar :
        public omni::ui::Widget {
        Q_OBJECT

        public:
          /// Construct with tuning widget as parent (is mandatory)
          TitleBar(Tuning *);

          /// Construct with label and tuning widget as parent
          TitleBar(QString const& _label,
                   Tuning *);
          ~TitleBar();

          /// Return color
          QColor color() const;

          /// Returns true if close button is visible
          bool   closeButtonVisible() const;

        public slots:
          /// Set color from gradient pos (between 0.0 and 1.0)
          void    setColor(QColor const&);

          /// Select color from color picker
          void    selectColor();

          /// Toggle visibility of close button
          void    setCloseButtonVisible(bool);

          /// Update State for display button when fullscreen has been toggled
          void    fullscreenToggle(bool);

        signals:
          void    closeButtonClicked();
          void    maximizeButtonClicked();
          void    colorChanged(QColor const&);
          void    freeSetupSelected();
          void    peripheralSetupSelected();

        protected:
          void    paintEvent(QPaintEvent *);

        private:
          void    setup();

          /// Set moving flag to value and handle visibility of buttons
          Tuning* tuningWidget();

          /// Button for resetting and projector setup selection
          QWidgetPtr<QToolButton> menuButton_;

          /// Button for toggling fullscreen display
          QWidgetPtr<QToolButton> displayButton_;

          /// Button for Minimizing/Maximizing widget
          QWidgetPtr<QToolButton> maximizeButton_;

          /// Button for closing widget
          QWidgetPtr<QToolButton> closeButton_;

          /// Context menu widget
          QWidgetPtr<QMenu> menu_;

          /// Parent tuning widget
          Tuning *tuningWidget_ = nullptr;

          QColor color_;
      };
    }
  }
}

#endif /* OMNI_UI_PROJ_TITLEBAR_H */
