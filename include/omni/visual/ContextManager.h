
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

#ifndef OMNI_VISUAL_CONTEXTMANAGER_H_
#define OMNI_VISUAL_CONTEXTMANAGER_H_

#include <set>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOffscreenSurface>
#include <omni/util.h>

namespace omni {
  namespace visual {
    class ContextManager {
    public:
      static ContextManager* instance();

      bool hasPrimaryContext() const;

      QOpenGLContext* primaryContext();

      template<typename F>
      void withPrimaryContext(F f) {
        if (!hasPrimaryContext()) {
          makePrimaryContext();
        }
      /*  QSurface* _surface = nullptr;
        auto* _previousContext = QOpenGLContext::currentContext();
        if (_previousContext) {
          _surface = _previousContext->surface();
        }*/

        primaryContext_->makeCurrent(surface_.get());
        QOpenGLFunctions _glFuncs;
        _glFuncs.initializeOpenGLFunctions();
        f(_glFuncs);
        primaryContext_->doneCurrent();

    /*    if (_surface) {
          _previousContext->makeCurrent(_surface);
        }*/
      }

      void add(QOpenGLContext*);
      void remove(QOpenGLContext*);

      int contextCount() const;

    private:
      ContextManager();
      ~ContextManager();

      /**@brief Make a new primary if does not exist
         @detail All framebuffers are stored in the primary context
       **/
      void makePrimaryContext();

      static ContextManager *instance_;
      QUniquePtr<QOffscreenSurface> surface_;
      QUniquePtr<QOpenGLContext> primaryContext_;
      std::set<QOpenGLContext*> contexts_;
    };
  }
}

#endif /* OMNI_VISUAL_CONTEXTMANAGER_H_ */
