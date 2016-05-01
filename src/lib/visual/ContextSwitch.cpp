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

#include <omni/visual/ContextSwitch.h>
#include <omni/visual/ContextManager.h>

namespace omni {
  namespace visual {
    void contextSwitch(QOpenGLContext *_context, ContextFunctor f) {
      auto *_current = QOpenGLContext::currentContext();
      if (!_current) return;

      if (_context != _current) {
        _context->makeCurrent(_current->surface());
      }

      withCurrentContext(f);

      if (_context != _current) {
        _current->makeCurrent(_current->surface());
      }
    }

    void primaryContextSwitch(ContextFunctor f) {
      if (!ContextManager::primaryContext()) return;
      contextSwitch(ContextManager::primaryContext(), f);
    }

    void withCurrentContext(ContextFunctor f)
    {
      auto _currentContext = QOpenGLContext::currentContext();
      if (!_currentContext) return;

      if (!_currentContext->isValid()) {
        qDebug() << "Context is not valid!";
        return;
      }
      QOpenGLFunctions glFuncs(_currentContext);
      f(glFuncs);
      glFuncs.glFinish();
    }
  }
}
