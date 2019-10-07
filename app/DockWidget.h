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
#ifndef OMNI_UI_DOCKWIDGET_H_
#define OMNI_UI_DOCKWIDGET_H_

#include <QScrollArea>

namespace omni {
  namespace ui {
    /// DockWidget is scroll area with an embedded widget
    class DockWidget : public QScrollArea {
      Q_OBJECT

      public:
        DockWidget(QWidget * = nullptr);

        template<typename UI>
        DockWidget(UI&, QWidget *_parent = nullptr) :
          QScrollArea(_parent) {}

        virtual ~DockWidget() {}

      protected:
        /**@brief Setup ui form
           @detail UI parameter is smart pointer (e.g. unique_ptr) to UI form
         **/
        template<typename UI>
        void setup(UI& _ui) {
          _ui.reset(new typename UI::element_type);
          auto *_widget = new QWidget(this);
          this->setWidget(_widget);
          this->setWidgetResizable(true);
          _ui->setupUi(this->widget());
        }
    };
  }
}

#endif /* OMNI_UI_DOCKWIDGET_H_ */
