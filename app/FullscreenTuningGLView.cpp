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

#include "FullscreenTuningGLView.h"
#include <omni/proj/ScreenSetup.h>
#include <omni/visual/util.h>
#include <omni/visual/Tuning.h>
#include <omni/visual/Rectangle.h>

#include <QGuiApplication>

namespace omni {
  namespace ui {
    FullscreenTuningGLView::FullscreenTuningGLView (QScreen const* _screen, QWidget* _parent) : 
      GLView(_parent),
      screen_(_screen) {
      setStyleSheet("* { background:black; }");
      setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

#ifdef Q_OS_LINUX
      setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
#endif

      QWidget::setGeometry(_screen->geometry());
      this->hide();
    }

    FullscreenTuningGLView::~FullscreenTuningGLView() {
    }

    void FullscreenTuningGLView::attachTuning(proj::Tuning const* _tuning) {
      if (_tuning->screen() != screen_) return;

      tunings_.insert(_tuning);
      setGeometry(screen_->geometry());
      show();

      update();
    }
    
    void FullscreenTuningGLView::detachTuning(proj::Tuning const* _tuning) {
      tunings_.erase(_tuning);
      if (tunings_.empty()) {
        this->hide();
      }

      update();
    }
    
    void FullscreenTuningGLView::showDragWidget(int _index, QColor _color) {
      dragWidgetIndex_ = _index;
      dragWidgetColor_ = _color;
      setGeometry(screen_->geometry());
      show();
      update();
    }

    void FullscreenTuningGLView::hideDragWidget() {
      dragWidgetIndex_ = -1;
      update();
    }
 
    void FullscreenTuningGLView::paint() {
      int d = this->devicePixelRatio();
      glViewport(0,0,width()*d,height()*d);

      if (showDragWidget()) {
        glClearColor(0.0,0.0,0.0,1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        QMatrix4x4 _m;

        _m.ortho(0.0, width(), height(), 0, -1.0,
                  1.0);
        glMultMatrixf(_m.constData());
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        QRectF && _rect = proj::ScreenSetup::subScreenRectForScreen(dragWidgetIndex_,screen_);
        
        glColor4f(
            dragWidgetColor_.red(),
            dragWidgetColor_.green(),
            dragWidgetColor_.blue(),1.0);
        visual::Rectangle::draw(_rect);
      }

      for (auto& _tuning : tunings_) {
        if (!_tuning->visualizer()) return;

        auto _rect = _tuning->contentGeometry();
        glViewport(
            _rect.left() * d, (height() - _rect.height() - _rect.top()) * d , 
            _rect.width() * d, _rect.height() * d);
        _tuning->visualizer()->drawFullScreenOutput();
      }
    }
    
    bool FullscreenTuningGLView::initialize() {
      return context() != nullptr;
    }
  }
}
