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

#ifndef OMNIC_GL_SHADEROBJECT_H_
#define OMNIC_GL_SHADEROBJECT_H_

#include <omnic/gl/shader.h>

namespace omnic {
  namespace gl {
    /// Struct that encapsulates GLSL calibration shader
    struct ShaderObject OMNIC_GL_INHERIT_QT_OPENGLFUNCTIONS {
    public:

      void initialize() {
        if (programId_ != 0) return;
#if OMNIC_USE_QT_GL
        initializeOpenGLFunctions();
#endif
        programId_ = glCreateProgram();
        GLint _vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
        GLint _fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

        const char* _vertexSrc = getVertexShaderSource();
        const char* _fragmentSrc = getFragmentShaderSource();
        glShaderSource(_vertexShaderId,1,&_vertexSrc,NULL);
        glShaderSource(_fragmentShaderId,1,&_fragmentSrc,NULL);

        glCompileShader(_vertexShaderId);
        shaderLog(_vertexShaderId);

        glCompileShader(_fragmentShaderId);
        shaderLog(_fragmentShaderId);

        glAttachShader(programId_, _vertexShaderId);
        glAttachShader(programId_, _fragmentShaderId);
          
        glLinkProgram(programId_);

        glDeleteShader(_vertexShaderId);
        glDeleteShader(_fragmentShaderId);

        // Get uniforms
        colorCorrectionLoc_ = glGetUniformLocation(programId_,"colorcorrection");

        pixeldataLoc_ = glGetUniformLocation(programId_,"pixeldata");
        pixeldataWidthLoc_ = glGetUniformLocation(programId_,"pixeldata_width");
        pixeldataHeightLoc_ = glGetUniformLocation(programId_,"pixeldata_height");

        input2DLoc_ = glGetUniformLocation(programId_,"input_2d");
        inputUseRectLoc_ = glGetUniformLocation(programId_,"input_use_rect");

        inputRectLoc_ = glGetUniformLocation(programId_,"input_rect");
        inputWidthLoc_ = glGetUniformLocation(programId_,"input_width");
        inputHeightLoc_ = glGetUniformLocation(programId_,"input_height");

        blendmaskAlphaLoc_ = glGetUniformLocation(programId_,"blendmask_alpha");
      }

      inline void bind() {
        glUseProgram(programId_);
        
      }

      inline void release() {  
        glUseProgram(0);
      }

      inline GLint colorCorrectionLoc() const {
        return colorCorrectionLoc_;
      }
      
      inline GLint pixeldataLoc() const {
        return pixeldataLoc_;
      }
      
      inline GLint pixeldataWidthLoc() const {
        return pixeldataWidthLoc_;
      }
      
      inline GLint pixeldataHeightLoc() const {
        return pixeldataHeightLoc_;
      }
      
      inline GLint input2DLoc() const {
        return input2DLoc_;
      }
      
      inline GLint inputRectLoc() const {
        return inputRectLoc_;
      }
      
      inline GLint inputUseRectLoc() const {
        return inputUseRectLoc_;
      }
      
      inline GLint inputWidthLoc() const {
        return inputWidthLoc_;
      }
      
      inline GLint inputHeightLoc() const {
        return inputHeightLoc_;
      }

      inline bool isInitialized() const {
        return programId_ != 0;
      }

      inline GLint programId() const {
        return programId_;
      }

    private:
      void shaderLog(int id) {
          GLint maxLength = 0;
          glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);
          std::vector<char> errorLog(maxLength);
          glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);
          std::string log;
          for (int i = 0; i < errorLog.size(); i++) {
            log += errorLog[i];
          }
          std::cout << log;
      }

      GLint colorCorrectionLoc_;
      GLint pixeldataLoc_;
      GLint pixeldataWidthLoc_;
      GLint pixeldataHeightLoc_;

      GLint blendmaskAlphaLoc_;

      GLint input2DLoc_;
      GLint inputRectLoc_;
      GLint inputUseRectLoc_;
      GLint inputWidthLoc_;
      GLint inputHeightLoc_;

      GLint programId_ = 0;
    };
  }
}

#endif /* OMNIC_GL_SHADEROBJECT_H_ */

