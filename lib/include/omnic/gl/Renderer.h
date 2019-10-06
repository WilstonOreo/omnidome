/* Copyright (c) 2014-2016 "OmniCalibration" by Michael Winkelmann
 * Calibration Format for Omnidome (http://omnido.me).
 * Created by Michael Winkelmann aka Wilston Oreo (@WilstonOreo)
 *
 * This file is part of Omnidome.
 *
 * Simplified BSD license
 * Copyright (c) 2016
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef OMNIC_GL_RENDERER_H_
#define OMNIC_GL_RENDERER_H_

#include <iostream>
#include <omnic/gl/Texture.h>
#include <omnic/gl/TextureRef.h>
#include <omnic/gl/ShaderObject.h>
#include <omnic/CalibratedProjector.h>

namespace omnic
{
  namespace gl
  {
    class Renderer OMNIC_GL_INHERIT_QT_OPENGLFUNCTIONS
    {
    public:
      Renderer() {}
      Renderer(CalibratedProjector const& _proj)
      {
        initialize(_proj);
      }

      inline void initialize(CalibratedProjector const& _proj)
      {
        if (isInitialized()) return;

#if OMNIC_USE_QT_GL
        initializeOpenGLFunctions();
#endif
        destroy();

        if (!shader().isInitialized()) {
          shader().initialize();
        }

        contentGeometry_ = _proj.contentGeometry();

        auto& _colorCorrectionLookUp = _proj.colorCorrection();
        colorCorrectionTex_.initialize(_colorCorrectionLookUp.data(),GL_TEXTURE_1D);
        pixeldata_.initialize(_proj.pixelData(),GL_TEXTURE_RECTANGLE);
      }

      inline bool isInitialized() const
      {
        return pixeldata_.textureId() && colorCorrectionTex_.textureId();
      }

      inline void bindCalibration(
        GLuint _inputId,
        GLuint _inputWidth,
        GLuint _inputHeight,
        GLuint _target = GL_TEXTURE_RECTANGLE) {
        bindCalibration(gl::TextureRef(_inputId,_inputWidth,_inputHeight,_target));
      }

      inline void bindCalibration(gl::TextureRef const& _input)
      {
        shader().bind();

        /// Different uniform location for different texture target
        glUniform1i(shader().inputUseRectLoc(),
            _input.target() == GL_TEXTURE_2D ? GL_FALSE : GL_TRUE);
        glUniform1i(shader().inputWidthLoc(),_input.width());
        glUniform1i(shader().inputHeightLoc(),_input.height());
        
        glUniform1i(shader().pixeldataWidthLoc(),pixeldata_.width());
        glUniform1i(shader().pixeldataHeightLoc(),pixeldata_.height());

        /// Bind input texture
        //glUniform1i(shader().input2DLoc(),0);
        glUniform1i(shader().inputRectLoc(),0);
        glActiveTexture(GL_TEXTURE0 + 0);
        glBindTexture(_input.target(), _input.id());

        /// Bind pixeldata texture
        
        glUniform1i(shader().pixeldataLoc(),1);
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_RECTANGLE, pixeldata_.textureId());  
        
        /// Bind colorcorrection lookup texture;
        glUniform1i(shader().colorCorrectionLoc(),2);
        glActiveTexture(GL_TEXTURE0 + 2);
        glBindTexture(GL_TEXTURE_1D, colorCorrectionTex_.textureId());
      }

      /// Release calibration 
      inline void releaseCalibration() 
      {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_1D,0);
        glBindTexture(GL_TEXTURE_2D,0);
        glBindTexture(GL_TEXTURE_RECTANGLE,0);
        glUseProgram(0);
      }

      /// Render with given texture ref 
      inline void render(gl::TextureRef const& _tex) {
        bindCalibration(_tex);

        auto& _r = contentGeometry_;

        glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2f(0.0,0.0);
        glVertex2i(_r.offsetX()             ,_r.offsetY());
        glTexCoord2f(1.0,0.0);
        glVertex2i(_r.offsetX() + _r.width(),_r.offsetY());
        
        glTexCoord2f(0.0,1.0);
        glVertex2i(_r.offsetX()             ,_r.offsetY() + _r.height());
        glTexCoord2f(1.0,1.0);
        glVertex2i(_r.offsetX() + _r.width(),_r.offsetY() + _r.height());
        glEnd();

        releaseCalibration();
      }

      /// Render with given texture id, size and target 
      inline void render(
        GLuint _inputId,
        GLuint _inputWidth,
        GLuint _inputHeight,
        GLuint _target = GL_TEXTURE_RECTANGLE) 
      {
        render(gl::TextureRef(_inputId,_inputWidth,_inputHeight,_target));
      }

      inline void destroy() {
        colorCorrectionTex_.destroy();
        pixeldata_.destroy();
      } 

      inline Rect const& contentGeometry() const {
        return contentGeometry_;
      }

      
      inline bool blendmaskAlpha() const {
        return blendmaskAlpha_;
      }
      
      inline static ShaderObject& shader() {
        static ShaderObject shader_;
        return shader_;
      }

    private:
      Rect contentGeometry_;

      bool blendmaskAlpha_ = false;


      TextureRGBA32F colorCorrectionTex_;
      TextureRGBA16 pixeldata_;

    };
  }
}

#endif /* OMNIC_GL_RENDERER_H_ */
