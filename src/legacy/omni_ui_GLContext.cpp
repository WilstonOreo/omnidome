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

#include <omni/ui/GLContext.h>

namespace omni
{
  namespace ui
  {
    GLContext::GLContext() : QGLContext(QGLFormat::defaultFormat())
    {
    }

    GLContext::~GLContext()
    {
    }
      
    void GLContext::initialize()
    {
      if (isValid()) return;
      if (!create()) return; 

// No glew required for apple
#ifndef __APPLE__ 
      glewExperimental=GL_TRUE; // Enable experimental extension for OpenGL 3.3 and later  
      static GLenum err = -1;
      if (err == -1)
      {
        err = glewInit(); 
        if (GLEW_OK != err)
        {
          std::cerr << "GLEW could not be initialized. Error: " << glewGetErrorString(err) << std::endl;
        } else
        {
          std::cerr << "GLEW was initialized!" << std::endl;
        }
      }
#endif

      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LEQUAL);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_LINE_SMOOTH);
      glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
      glEnable(GL_POINT_SMOOTH);
      glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
      glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
      glPolygonMode(GL_FRONT,GL_FILL);
      glPolygonMode(GL_BACK,GL_FILL);
      glEnable(GL_NORMALIZE);
      // fix outlines z-fighting with quads
      glPolygonOffset(1, 1);
   /* FIXME 
      static std::string _vertSrc = qfileToStr(":/shaders/frustum.vert");
      static std::string _fragmentSrc = qfileToStr(":/shaders/frustum.frag");
      static std::string _blendVertSrc = qfileToStr(":/shaders/frustum.vert");
      static std::string _blendFragmentSrc = qfileToStr(":/shaders/blend.frag");

      if (!shader_)
        shader_.reset(new Shader(_vertSrc,_fragmentSrc));
      if (!blendShader_)
        blendShader_.reset(new Shader(_blendVertSrc,_blendFragmentSrc));
*/
      setInitialized(true);
    }

    void GLContext::free()
    {
      shader_.reset(nullptr);
      blendShader_.reset(nullptr);    
    }
  }
}
