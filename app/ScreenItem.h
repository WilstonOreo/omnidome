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
#ifndef OMNI_UI_SCREENITEM_H_
#define OMNI_UI_SCREENITEM_H_

#include <QColor>
#include <QScreen>
#include <omni/util.h>
#include "FullscreenTuningGLView.h"

namespace omni {

  namespace ui {
    namespace proj {
      class Tuning;
    }

    class ScreenItem;
    class ScreenSetup;

    /// Subscreen Item for which a tuning can be assigned
    class SubScreenItem {
      public:
        SubScreenItem();
        SubScreenItem(ScreenItem* _parent,
                      int _index);

        /// Currently assigned tuning widget
        omni::ui::proj::Tuning      * tuningWidget();

        /// Currently assigned tuning widget (const version)
        omni::ui::proj::Tuning const* tuningWidget() const;

        /// Attach tuning widget to this subscreen
        void                      attachTuningWidget(
          omni::ui::proj::Tuning *_tuning);

        /// Detach current tuning widget from this subscreen
        void                      detachTuningWidget();

        /// Return rectangle of this subscreen item
        QRect                     rect() const;

        /// Return size of this subscreen
        QSize                     size() const;

        /// Pointer to parent screen
        ScreenItem*              parent();

        /// Const pointer to parent screen
        ScreenItem const*        parent() const;


        /// Paint this item with painter
        void                      paint(bool _hover,
                                        bool _drop,
                                        QColor _dropColor,
                                        QPainter&);

      private:
        ScreenItem *parent_   = nullptr;
        int index_                  = -1;
        omni::ui::proj::Tuning *tuningWidget_ = nullptr;
    };

    /// A screen rectangle item representing a QScreen with drawing functionality
    class ScreenItem {
      public:
        friend class SubScreenItem;

        ScreenItem(ScreenSetup &,
             QScreen const *_screen);
        ~ScreenItem();

        /// Detach tuning widget
        void           detachTuningWidget(omni::ui::proj::Tuning*);

        /// Screen this screen item is assigned to
        QScreen const* screen() const;

        void           paint(QPainter&);

        /// Detach all tunings/projectors
        void           detachTunings();

        SubScreenItem* item(int _index);

        SubScreenItem const* item(int _index) const;

        /// Return pointer to currently hovered item
        SubScreenItem* hoveredItem();

        /// Return pointer to currently hovered item (const)
        SubScreenItem const* hoveredItem() const;

        /// Returns flag whether mouse is currently over this item
        int            hoverIndex() const;

        /// Set index of current hovered subscreen
        void           setHoverIndex(int);

        /// Set hover index from mouse position
        void           setHoverIndex(QPoint const&);

        /// Flag whether a drop is supposed to happen on this item
        bool           drop() const;

        /// Drop and color of drop
        void           startDrop(QColor const& _color = QColor("#FFFFFF"));

        /// Hides fullscreen widget if no tunings are attached
        void  endDrop();

        QRect rect() const;

        FullscreenTuningGLView*  fullscreen();

        inline int numSubScreens() const {
          return int(subScreens_.size());
        }

      private:
        int hoverIndex_ = -1;
        bool   drop_    = false;
        QColor dropColor_;
        ScreenSetup  & screenSetup_;
        QScreen const *screen_ = nullptr;
        QUniquePtr<FullscreenTuningGLView>  fullscreen_;
        std::vector<SubScreenItem> subScreens_;
    };
  }
}

#endif /* OMNI_UI_SCREENITEM_H_ */
