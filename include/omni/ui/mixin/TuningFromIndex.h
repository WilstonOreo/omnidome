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

#ifndef OMNI_UI_MIXIN_TUNINGFROMINDEX_H_
#define OMNI_UI_MIXIN_TUNINGFROMINDEX_H_

#include <omni/Session.h>
#include <omni/ui/mixin/DataModel.h>

namespace omni {
  namespace ui {
    namespace mixin {
      template<typename WIDGET>
      class TuningFromIndex {
        public:
          TuningFromIndex(WIDGET& _widget) :
            widget_(_widget) {}

          int index() const {
            if (index_ >= 0) return index_;

            if (!widget_.dataModel()) return -1;

            return widget_.dataModel()->tunings().currentIndex();
          }

          /// Set index of tuning
          void setIndex(int _index) {
            index_ = _index;
          }

          /// Set index to current index
          void setIndexToCurrent() {
            index_ = -1;
          }

          /// Return pointer to tuning
          omni::proj::Tuning* tuning() const {
            return widget_.dataModel() ?
                   widget_.dataModel()->tunings()[index()] :
                   nullptr;
          }

          /// Return pointer to blend mask
          omni::BlendMask* blendMask() const
          {
            if (!tuning()) return nullptr;

            return tuning() ? &tuning()->blendMask() : nullptr;
          }

          /// Return pointer to warp grid
          omni::WarpGrid* warpGrid() const
          {
            if (!tuning()) return nullptr;

            return tuning() ? &tuning()->warpGrid() : nullptr;
          }

        private:
          int index_ = -1;
          WIDGET& widget_;
      };
    }
  }
}

#endif /* OMNI_UI_MIXIN_TUNINGFROMINDEX_H_ */
