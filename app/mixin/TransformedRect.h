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

#ifndef OMNI_UI_MIXIN_TRANSFORMEDRECT_H
#define OMNI_UI_MIXIN_TRANSFORMEDRECT_H

#include <QRectF>
#include <QDebug>

namespace omni {
  namespace ui {
    namespace mixin {
      /**@brief Mixin which calculates transformed screen rectangles
         @detail Used in export output preview and screen setup widgets
       **/
      template<typename WIDGET>
      class TransformedRect {
        public:
          typedef WIDGET widget_type;
          TransformedRect(widget_type *_widget) : widget_(_widget) {}

          /// Pure virtual method which returns overall desktop rect
          virtual QRect desktopRect() const = 0;

          /// Returns the scaling factor which is needed so that desktopRect_
          // fits into window
          float scalingFactor() const
          {
            if (!widget_) return 0.0;

            auto _windowRect = widget_->rect();

            if ((_windowRect.height() == 0) ||
                (_windowRect.width() == 0)) return 0.0;

            QRect _desktopRect = this->desktopRect();

            if ((_desktopRect.height() == 0) ||
                (_desktopRect.width() == 0)) return 0.0;

            float _rectAspect = float(_desktopRect.width()) /
                                float(_desktopRect.height());
            float _windowAspect = float(_windowRect.width()) /
                                  float(_windowRect.height());

            float _factor = 1.0f;

            if (_windowAspect < _rectAspect) {
              _factor = float(_windowRect.width()) / _desktopRect.width();
            } else {
              _factor = float(_windowRect.height()) / _desktopRect.height();
            }

            return _factor * zoom();
          }

          /// Returns transformed bounding rect which fits into window and keeps
          // aspect ratio
          inline QRectF transformedRect() const
          {
            auto  _windowRect  = widget_->rect();
            auto  _zoom        = scalingFactor();
            QRect _desktopRect = this->desktopRect();

            return QRectF(
              0.5 * (_windowRect.width() - _zoom * (_desktopRect.width())),
              0.5 * (_windowRect.height() - _zoom * (_desktopRect.height())),
              _zoom * _desktopRect.width(),
              _zoom * _desktopRect.height());
          }

          /// Returns Transformed copy of the rect, based on transformed desktop
          // rect
          inline QRectF transformedRect(QRectF const& _rect) const
          {
            auto _zoom = scalingFactor();

            /// Transformed desktop rect
            auto _desktopRect = transformedRect();

            return QRectF(
              _desktopRect.x() + _rect.x() * _zoom,
              _desktopRect.y() + _rect.y() * _zoom,
              _rect.width() * _zoom,
              _rect.height() * _zoom);
          }

          /// Return zoom factor
          float zoom() const {
            return zoom_;
          }

          /// Set zoom factor
          void setZoom(float _zoom)
          {
            zoom_ = _zoom;
            widget_->update();
          }

        private:
          widget_type *widget_ = nullptr;
          float zoom_          = 0.9;
      };
    }
  }
}

#endif /* OMNI_UI_MIXIN_TRANSFORMEDRECT_H */
