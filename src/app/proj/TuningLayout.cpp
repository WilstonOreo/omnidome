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

#include "proj/TuningLayout.h"

#include <QDebug>
#include "proj/Tuning.h"

namespace omni {
  namespace ui {
    namespace proj {
      TuningLayout::TuningLayout(Tuning *_parent) :
        QLayout(_parent)
      {}

      TuningLayout::~TuningLayout()
      {
        clear();
      }

      void TuningLayout::addItem(QLayoutItem *_item)
      {
        add(_item, Role::PARAMETER);
      }

      void TuningLayout::addWidget(QWidget *widget)
      {
        add(new QWidgetItem(widget), Role::PARAMETER);
      }

      void TuningLayout::addWidget(QWidget *widget, Role _role)
      {
        add(new QWidgetItem(widget), _role);
      }

      int TuningLayout::count() const
      {
        return items_.size();
      }

      int TuningLayout::indexOf(QWidget *widget) const
      {
        int _index = -1;

        for (auto& _item : items_)
        {
          if (_item.widget() == widget) return _index;

          ++_index;
        }
        return _index;
      }

      QLayoutItem * TuningLayout::itemAt(int index) const
      {
        return (index >= 0 &&
                index < items_.size()) ? items_[index].item_ : nullptr;
      }

      QLayoutItem * TuningLayout::takeAt(int index)
      {
        QLayoutItem *_item = itemAt(index);

        if (_item)
        {
          items_.erase(items_.begin() + index);
          return _item;
        }
        return nullptr;
      }

      QSize TuningLayout::calculateSize(SizeType sizeType) const
      {
        if (!tuning()) return QSize(0, 0);


        int _border = 2;
        int _width  = 0;
        int _height = _border;

        auto _parent = static_cast<Tuning *>(parent());

        for (auto& _item : items_)
        {
          switch (_item.role_)
          {
          case Role::PARAMETER:
          case Role::TITLE:
            _height += 36 / _parent->devicePixelRatio();
            break;

          case Role::PREVIEW:

            if (!tuning()) continue;

            /// Increase height by aspect ratio
            _height += _parent->width() /
                       float(tuning()->width()) * tuning()->height();
            break;
          }
        }
        _height += _border;

        return QSize(_width, _height);
      }

      omni::proj::Tuning * TuningLayout::tuning()
      {
        return static_cast<Tuning *>(parent())->tuning();
      }

      omni::proj::Tuning const * TuningLayout::tuning() const
      {
        return static_cast<Tuning const *>(parent())->tuning();
      }

      void TuningLayout::setWidgets(widgetgroup_type const& _widgets)
      {
        clear();

        for (auto& _widgetRole : _widgets)
        {
          auto& _widget = _widgetRole.first;
          auto& _role   = _widgetRole.second;
          addWidget(_widget, _role);
        }
        update();
      }

      void TuningLayout::clear()
      {
        QLayoutItem *l;

        while ((l = takeAt(0))) delete l;
      }

      QSize TuningLayout::minimumSize() const
      {
        return calculateSize(MINIMUMSIZE);
      }

      QSize TuningLayout::sizeHint() const
      {
        return calculateSize(SIZEHINT);
      }

      void TuningLayout::setGeometry(QRect const& _rect)
      {
        if (!tuning()) return;

        const int _border = 2;
        int _height       = _border;

        auto _parent = static_cast<Tuning *>(parent());

        /// Adjust geometry for each widget
        for (auto& _item : items_)
        {
          auto _widget = _item.widget();

          /// Widget height is constant except for preview
          int _widgetHeight = _item.role_ == Role::PREVIEW ? _parent->width() /
                              float(tuning()->width()) * tuning()->height() :
                              36 / _parent->devicePixelRatio();
          _widget->setGeometry(_border, _height,
                               _parent->width() - _border * 2, _widgetHeight);
          _widget->show();

          /// Increase height
          _height += _widgetHeight;
        }
      }

      QRect TuningLayout::geometry() const {
        auto _rect = QLayout::geometry();

        _rect.setWidth(_rect.width() - 16);
        return _rect;
      }

      void TuningLayout::add(QLayoutItem *_item, Role _role)
      {
        items_.emplace_back(_item, _role);
        invalidate();
      }
    }
  }
}
