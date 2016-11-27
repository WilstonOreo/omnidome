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

#include <omni/visual/VBO.h>

#include <omni/visual/util.h>

namespace omni
{
  namespace visual
  {
    VBO::VBO()
    {
      gen();
    }

    VBO::~VBO()
    {
      free();
    }

    void VBO::gen()
    {
      withCurrentContext([this](QOpenGLFunctions& _)
      {
        if (!id_)
          _.glGenBuffers(1, &id_);
      });
    }

    void VBO::freeAndGen()
    {
      withCurrentContext([this](QOpenGLFunctions& _)
      {
        if (id_) free();
        _.glGenBuffers(1,&id_);
      });
    }

    void VBO::free()
    {
      withCurrentContext([this](QOpenGLFunctions& _)
      {
        if (id_)
          _.glDeleteBuffers(1,&id_);
      });
    }

    GLuint VBO::id() const
    {
      return id_;
    }
  }
}
