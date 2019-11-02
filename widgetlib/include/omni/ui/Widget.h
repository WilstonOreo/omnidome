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

#ifndef OMNI_UI_WIDGET_H_
#define OMNI_UI_WIDGET_H_

#include <QWidget>
#include <QLabel>
#include <QPalette>
#include <omni/ui/ColorSet.h>

namespace omni {
  namespace ui {
    /// A single parameter widget with a label and color set
    class OMNIWIDGET_EXPORT Widget : public QWidget {
      Q_OBJECT

      public:
        Widget(QWidget * = nullptr);
        Widget(QString const& _label,
               QWidget * = nullptr);
        virtual ~Widget() {}

        /// Return label text of widget
        QString         label() const;

        /// Set label text of widget
        void            setLabel(QString const&);

        ColorSet const& colorSet() const;

        /// Zoom factor of widget
        double          zoomFactor() const;

      protected:
        void            drawBorder(QPainter&);
        void            drawBorder(QPainter&,
                                   bool _focus);
        QLabel *label_;

      private:
        void init();

        ColorSet colorSet_;
        double   zoomFactor_ = 1.0;
    };
  }
}

#endif /* OMNI_UI_WIDGET_H_ */
