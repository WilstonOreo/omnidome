
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

#include <omni/visual/util.h>

namespace omni {
  namespace visual {
    ContextManager* ContextManager::instance_ = nullptr;

    ContextManager::ContextManager() {}
    ContextManager::~ContextManager() {}

    ContextManager * ContextManager::instance() {
      if (!instance_) {
        instance_ = new ContextManager();
      }
      return instance_;
    }

    bool ContextManager::hasPrimaryContext() const {
      return primaryContext_;
    }

    QOpenGLContext* ContextManager::primaryContext() {
      return primaryContext_;
    }

    void ContextManager::add(QOpenGLContext* _ctx) {
      if (!primaryContext_) {
        primaryContext_ = _ctx;/*
        primaryContext_ = QOpenGLContext::globalShareContext();
        primaryContext_->setFormat(_ctx->format());
        primaryContext_->setScreen(_ctx->screen());
        primaryContext_->create();*/
      }
      contexts_.insert(_ctx);
      if (_ctx != primaryContext_) {
        _ctx->setShareContext(primaryContext_);
      }
    }

    void ContextManager::remove(QOpenGLContext* _ctx) {
      contexts_.erase(_ctx);
      if (_ctx == primaryContext_) {
        primaryContext_ = nullptr;
      }
    }

    int ContextManager::contextCount() const {
      return contexts_.size();
    }
  }
}
