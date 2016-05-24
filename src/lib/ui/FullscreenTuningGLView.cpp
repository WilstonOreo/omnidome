/* Copyright (c) 2014-2015 "Omnidome" by cr8tr
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

#include <omni/ui/FullscreenTuningGLView.h>
#include <omni/visual/util.h>

namespace omni {
  namespace ui {
    FullscreenTuningGLView::FullscreenTuningGLView (QScreen const* _screen, QWidget* _parent) : 
      GLView(_parent),
      screen_(_screen) {
    }

    FullscreenTuningGLView::~FullscreenTuningGLView() {
    }

    void FullscreenTuningGLView::attachTuning(proj::Tuning const* _tuning) {
      if (_tuning->screen() != screen_) return;

      tunings_.insert(_tuning);
    }
    
    void FullscreenTuningGLView::detachTuning(proj::Tuning const* _tuning) {
      tunings_.erase(_tuning);
    }
 
    void FullscreenTuningGLView::paint() {
      visual::withCurrentContext([&](QOpenGLFunctions& _) {
        _.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        for (auto& _tuning : tunings_) {
          auto _rect = _tuning->contentGeometry();
          int d = this->devicePixelRatio();
          _.glViewport(
            _rect.left() * d, _rect.top() * d, 
            _rect.width() * d, _rect.height() * d);
         // _tuning->visualizer()->drawFullScreenOutput();
        }
      });
    }
  }
}
