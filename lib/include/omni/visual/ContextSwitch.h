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

#ifndef OMNI_VISUAL_CONTEXTSWITCH_H_
#define OMNI_VISUAL_CONTEXTSWITCH_H_

#include <functional>
#include <QOpenGLContext>
#include <omni/global.h>

namespace omni {
  namespace visual {
    using ContextFunctor = std::function<void (QOpenGLFunctions&)>;

    /// Do OpenGL operations with current context, if it exists
    void OMNI_EXPORT withCurrentContext(ContextFunctor);

    void OMNI_EXPORT contextSwitch(QOpenGLContext*, ContextFunctor);

    /// Switch to primary context to create OpenGL objects like textures etc
    void OMNI_EXPORT primaryContextSwitch(ContextFunctor);
  }

  using visual::withCurrentContext;
  using visual::contextSwitch;
  using visual::primaryContextSwitch;
}

#endif /* OMNI_VISUAL_CONTEXTSWITCH_H_ */
