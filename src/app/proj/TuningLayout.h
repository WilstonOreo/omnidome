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

#ifndef OMNI_UI_PROJ_TUNINGLAYOUT_H_
#define OMNI_UI_PROJ_TUNINGLAYOUT_H_

#include <QLayout>
#include <omni/proj/Tuning.h>

namespace omni {
  namespace ui {
    namespace proj {
      class Tuning;

      /**@brief Tuning Layout
       * @detail Similar to VBoxLayout, except that widgets with a PREVIEW Role
       *         have a size with aspect ratio of associated tuning
       **/
      class TuningLayout : public QLayout {
        Q_OBJECT

        public:
          /// Role of a widget defines resize behaviour
          enum class Role {
            TITLE,     // Fixed size
            PARAMETER, // Fixed size
            PREVIEW    // Size with aspect ratio of tuning
          };

          typedef std::vector<std::pair<QWidget *, Role> >widgetgroup_type;

          TuningLayout(Tuning *parent);
          ~TuningLayout();

          /// Add item, with PARAMETER Role
          void addItem(QLayoutItem *item);

          /// Add widget with PARAMETER Role
          void addWidget(QWidget *widget);

          /// Add widget with Role
          void addWidget(QWidget * widget, Role);

          /// Return number of widgets
          int                       count() const;

          /// Get index of given widget
          int                       indexOf(QWidget *widget) const;

          /// Return item at index
          QLayoutItem             * itemAt(int index) const;

          /// Remove widget at index
          QLayoutItem             * takeAt(int index);

          /// Set geometry of all widgets
          void                      setGeometry(const QRect& rect);

          QRect                     geometry() const;

          /// Clears layout and inserts the given widgets
          void                      setWidgets(widgetgroup_type const& _widgets);

          /// Clear layout
          void                      clear();

          /// Return minimum size
          QSize                     minimumSize() const;

          /// Return size hint, is equal to minimum size
          QSize                     sizeHint() const;

        private:
          omni::proj::Tuning      * tuning();
          omni::proj::Tuning const* tuning() const;

          struct ItemWrapper
          {
            ItemWrapper(QLayoutItem *_item = nullptr,
                        Role _role = Role::PARAMETER) :
              item_(_item),
              role_(_role)
            {}

            QLayoutItem *item_;
            Role         role_;

            QWidget* widget() const
            {
              return item_->widget();
            }
          };

          /// Add a layout item with Role
          void add(QLayoutItem *_item,
                   Role _role);

          enum SizeType { MINIMUMSIZE, SIZEHINT };
          QSize calculateSize(SizeType sizeType) const;

          std::vector<ItemWrapper> items_;
      };
    }
  }
}

#endif /* OMNI_UI_PROJ_TUNINGLAYOUT_H_ */
