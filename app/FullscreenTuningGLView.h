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

#ifndef OMNI_UI_FULLSCREENTUNINGGLVIEW_H_
#define OMNI_UI_FULLSCREENTUNINGGLVIEW_H_

#include <omni/proj/Tuning.h>
#include <omni/ui/GLView.h>

namespace omni {
  namespace ui {

    class FullscreenTuningGLView : public GLView {
      Q_OBJECT
    public:
      FullscreenTuningGLView(QScreen const* _screen, QWidget * = nullptr);
      ~FullscreenTuningGLView();

      void attachTuning(proj::Tuning const* _tuning);
      void detachTuning(proj::Tuning const* _tuning);

      void showDragWidget(int _index, QColor _color);
      void hideDragWidget();

    private:
      void paint();
      bool initialize();
      std::set<proj::Tuning const*> tunings_;

      QScreen const* screen_ = nullptr;
  
      inline bool showDragWidget() const {
        return dragWidgetColor_ != -1;
      }

      int dragWidgetIndex_ = -1;
      QColor dragWidgetColor_;
    };
  }
}

#endif /* OMNI_UI_FULLSCREENTUNINGGLVIEW_H_ */
