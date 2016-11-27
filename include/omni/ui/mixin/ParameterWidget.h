/* Copyright (c) 2014-2016 "Omnidome" by Michael Winkelmann
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
         @tparam Base class of parameter widget
         @detail Interface can be a mapping, canvas or input interface
       **/
      template<typename WIDGET = QWidget>
      class ParameterWidget {
        public:
          typedef WIDGET widget_type;

          /// Remove parameter widget from given widget
          template<typename PARENT_WIDGET>
          inline void removeParameterWidget(PARENT_WIDGET *_widget) {
            if (parameterWidget_) {
              _widget->layout()->removeWidget(parameterWidget_);
              parameterWidget_->hide();
              parameterWidget_->deleteLater();
              parameterWidget_ = nullptr;
            }
          }

          /// Setup widget for interface and place it onto widget
          template<typename PARENT_WIDGET, typename INTERFACE>
          bool setupParameterWidget(PARENT_WIDGET *_widget, INTERFACE *_interface) {
            if (!_widget || !_widget->layout()) return false;

            removeParameterWidget(_widget);

            if (!_interface) return false;

            /// Make a new parameter widget
            parameterWidget_ = _interface->widget();

            if (parameterWidget_) {
              parameterWidgetSetupOptions(parameterWidget_);

              _widget->layout()->addWidget(parameterWidget_);
              parameterWidget_->show();
              return true;
            }
            return false;
          }

          /// Return pointer to parameterWidget
          inline widget_type* parameterWidget() {
            return parameterWidget_;
          }

        private:
          /// Additional options. Is called during setup process
          inline virtual void parameterWidgetSetupOptions(widget_type *_paramWidget)
          const {}

          widget_type *parameterWidget_ = nullptr;
      };
    }
  }
}

#endif /* OMNI_UI_MIXIN_PARAMETERWIDGET_H_ */
