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

#include <omni/visual/ContextSwitch.h>
#include <QOpenGLFunctions>

namespace omni {
  namespace visual {
    void contextSwitch(QOpenGLContext *_context, ContextFunctor f) {
      auto *_current = QOpenGLContext::currentContext();
      if (!_current || !_current->isValid()) return;
#ifndef OMNI_DISABLE_CONTEXT_SWITCH
      if (_context != _current) {
        _context->makeCurrent(_current->surface());
      }
#endif 

      withCurrentContext(f);

#ifndef OMNI_DISABLE_CONTEXT_SWITCH
      if (_context != _current) {
       _current->makeCurrent(_current->surface());
      }
#endif
    }

    void primaryContextSwitch(ContextFunctor f) {
      if (!QOpenGLContext::globalShareContext() || !QOpenGLContext::globalShareContext()->isValid() || 
          !QOpenGLContext::currentContext()) return;
#ifdef OMNI_DISABLE_CONTEXT_SWITCH
      QOpenGLFunctions glFuncs(QOpenGLContext::globalShareContext());
      f(glFuncs);
      glFuncs.glFinish();
#else
      contextSwitch(QOpenGLContext::globalShareContext(), f);
#endif
    }

    void withCurrentContext(ContextFunctor f)
    {
      auto _currentContext = QOpenGLContext::currentContext();
      if (!_currentContext || !_currentContext->isValid()) return;

      QOpenGLFunctions glFuncs(_currentContext);
      f(glFuncs);
    }
  }
}
