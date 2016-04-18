
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

#include <omni/visual/ContextManager.h>

namespace omni {
  namespace visual {
    ContextManager* ContextManager::instance_ = nullptr;

    ContextManager::ContextManager() { makePrimaryContext(); }
    ContextManager::~ContextManager() {}

    ContextManager * ContextManager::instance() {
      if (!instance_) {
        instance_ = new ContextManager();
      }
      return instance_;
    }

    bool ContextManager::hasPrimaryContext() const {
      return primaryContext_ && surface_;
    }

    void ContextManager::makePrimaryContext() {
      if (!primaryContext_) {
        primaryContext_.reset(new QOpenGLContext);
        if (!primaryContext_->create()) {
          qDebug() << "Could not create primary context!";
          return;
        }
        if (!primaryContext_->isValid()) {
          qDebug() << "Primary context is not valid!";
        }
      }

      if (!this->surface_) {
        surface_.reset(new QOffscreenSurface());
        surface_->setFormat(this->primaryContext_->format());
        surface_->create();
      }
    }

    QOpenGLContext* ContextManager::primaryContext() {
      if (!hasPrimaryContext()) {
        makePrimaryContext();
      }
      return primaryContext_.get();
    }

    void ContextManager::add(QOpenGLContext* _ctx) {

      contexts_.insert(_ctx);
      auto _primaryContext = primaryContext();
      if (_primaryContext) {
        _ctx->setShareContext(_primaryContext);
      }
    }

    void ContextManager::remove(QOpenGLContext* _ctx) {
      contexts_.erase(_ctx);
    }

    int ContextManager::contextCount() const {
      return contexts_.size();
    }
  }
}
