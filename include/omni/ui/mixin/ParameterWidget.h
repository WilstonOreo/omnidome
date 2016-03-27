/* Copyright (c) 2014-2016 "Omnidome" by cr8tr
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
#ifndef OMNI_UI_MIXIN_PARAMETERWIDGET_H_
#define OMNI_UI_MIXIN_PARAMETERWIDGET_H_

#include <QWidget>
#include <QLayout>

namespace omni {
  namespace ui {
    namespace mixin {
      /**@brief Setup and remove a parameter widget for a certain plugin
         interface
         @detail Interface can be a mapping, canvas or input interface
       **/
      class ParameterWidget {
        public:
          /// Remove parameter widget from given widget
          inline void removeParameterWidget(QWidget *_widget) {
            if (parameterWidget_) {
              _widget->layout()->removeWidget(parameterWidget_);
              parameterWidget_->hide();
              parameterWidget_->deleteLater();
              parameterWidget_ = nullptr;
            }
          }

          /// Setup widget for interface and place it onto widget
          template<typename INTERFACE>
          void setupParameterWidget(QWidget *_widget, INTERFACE *_interface) {
            if (!_widget) return;

            if (!_widget->layout()) return;

            removeParameterWidget(_widget);

            if (!_interface) return;

            /// Make a new parameter widget
            parameterWidget_ = _interface->widget();

            if (parameterWidget_) {
              parameterWidgetSetupOptions(parameterWidget_);

              _widget->layout()->addWidget(parameterWidget_);
              parameterWidget_->show();
            }
          }

          /// Return pointer to parameterWidget
          inline QWidget* parameterWidget() {
            return parameterWidget_;
          }

        private:
          /// Additional options. Is called during setup process
          inline virtual void parameterWidgetSetupOptions(QWidget *_paramWidget)
          const {}

          QWidget *parameterWidget_ = nullptr;
      };
    }
  }
}

#endif /* OMNI_UI_MIXIN_PARAMETERWIDGET_H_ */
