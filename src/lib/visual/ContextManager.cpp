
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
      return !!primaryContext_;
    }

    QOpenGLContext* ContextManager::primaryContext() {
      instance()->makePrimaryContext();
      return instance()->primaryContext_.get();
    }

    void ContextManager::makePrimaryContext() {
      if (instance()->primaryContext_.get()) {
        return;
      }

      primaryContext_.reset(new QOpenGLContext());
      primaryContext_->setFormat(QOpenGLContext::globalShareContext()->format());
      primaryContext_->setShareContext(QOpenGLContext::globalShareContext());

      surface_.reset(new QOffscreenSurface());
      surface_->setFormat(primaryContext_->format());
      surface_->create();

      /// Create primary context
      primaryContext_->create();
    }

    void ContextManager::add(QOpenGLContext* _ctx) {
      makePrimaryContext();

      _ctx->connect(_ctx,&QOpenGLContext::aboutToBeDestroyed,[&]() {
        this->remove(_ctx);
      });

      OMNI_DEBUG << "New context:" << _ctx << contextCount();

      contexts_.insert(_ctx);
      if (_ctx != primaryContext_.get()) {
        _ctx->setShareContext(primaryContext_.get());
      }
    }

    void ContextManager::remove(QOpenGLContext* _ctx) {
      contexts_.erase(_ctx);
      OMNI_DEBUG << "Context removed:" << contextCount();
    }

    int ContextManager::contextCount() const {
      return contexts_.size();
    }
  }
}
