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

#ifndef OMNI_UI_GLCONTEXT_H
#define OMNI_UI_GLCONTEXT_H

#include <tbd/property.h>
#include <omni/gl.hpp>
#include <QtOpenGL/QGLContext>
#include <memory>

namespace omni
{
  namespace ui
  {
    class GLContext : public QGLContext
    {
    public:
      typedef Texture<Image4ub,GL_TEXTURE_2D> texture_type;
      typedef std::unique_ptr<texture_type> texture_ptr_type;
      typedef std::unique_ptr<Shader> shader_ptr_type;

      GLContext();
      ~GLContext();

      void initialize();/*
      void useTexture(std::string const&, MapMode);
      void useTestImage();
      void useProjImage(int _number);*/
      void free();

      TBD_PROPERTY_REF_RO(shader_ptr_type,shader)
      TBD_PROPERTY_REF_RO(shader_ptr_type,blendShader)
      TBD_PROPERTY_REF_RO(shader_ptr_type,testImageShader)
      TBD_PROPERTY_REF_RO(shader_ptr_type,projImageShader) 
    };
  }
}

#endif /* OMNI_UI_GLCONTEXT_H */
