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

#ifndef OMNI_UI_SCREENSETUPDRAGWIDGET_H_
#define OMNI_UI_SCREENSETUPDRAGWIDGET_H_

#include <QWidget>
#include <QColor>

namespace omni {
  namespace ui {
    /// Widget which is shown when dragging a projector to a screen
    class ScreenSetupDragWidget : public QWidget {
      Q_OBJECT

      public:
        ScreenSetupDragWidget(QWidget * = nullptr);
        ~ScreenSetupDragWidget();

        /// Return widget color
        QColor color() const;

        /// Set widget color
        void   setColor(QColor const&);

        /// Set rectangle for widget
        void   setRect(QRectF const&);

      protected:
        /// Paint widget
        void   paintEvent(QPaintEvent *);

      private:
        QColor color_;
    };
  }
}


#endif /* OMNI_UI_SCREENSETUPDRAGWIDGET_H_ */
