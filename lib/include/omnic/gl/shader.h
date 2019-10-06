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

#ifndef OMNIC_GL_SHADER_H_
#define OMNIC_GL_SHADER_H_

#include <sstream>
#include <omnic/util.h>

#ifndef OMNIC_GL_SHADERVERSION
  #define OMNIC_GL_SHADERVERSION 120
#endif

namespace omnic {
  namespace gl {
     const char CalibrationVertexShader_120[] = OMNIC_STRINGIZE_SOURCE(
        void main() {
          gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
          gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
        } 
     );

     const char CalibrationFragmentShader_120[] =  OMNIC_STRINGIZE_SOURCE(
      uniform sampler2DRect pixeldata;
      uniform int pixeldata_width;
      uniform int pixeldata_height;
      uniform sampler1D colorcorrection;

      uniform sampler2D input_2d;
      uniform sampler2DRect input_rect;
      uniform bool input_use_rect;
      uniform bool blendmask_alpha;
      uniform int input_width;
      uniform int input_height;

      vec3 apply_colorcorrection(vec3 color) {
        return vec3(
          texture1D(colorcorrection,texture1D(colorcorrection,color.r).r).a,
          texture1D(colorcorrection,texture1D(colorcorrection,color.g).g).a,
          texture1D(colorcorrection,texture1D(colorcorrection,color.b).b).a
        );
      }

      void main() {
        vec2 uv = gl_TexCoord[0].xy; 
        vec4 pixeldata_pixel = texture2DRect(pixeldata,uv * vec2(pixeldata_width,pixeldata_height));

        vec4 color = vec4(1.0);
          color = texture2DRect(input_rect,pixeldata_pixel.st * vec2(input_width,input_height));

        if (blendmask_alpha) {
          gl_FragColor = vec4(apply_colorcorrection(color.rgb),pixeldata_pixel.a);
        } else {
          gl_FragColor = vec4(apply_colorcorrection(color.rgb)*pixeldata_pixel.a,1.0);
        }
      }
    );

    /// Get shader source from const char and attach version pragma
    template<typename SOURCE>
    inline static std::string getShaderSource(int _version, SOURCE const& _src) {
      std::stringstream _ss;
      _ss << "#version " << _version << std::endl;
      std::string _versionString(_ss.str());
      return std::string(_versionString + _src);
    }

    /// Get calibration vertex shader source with version pragma
    inline static const char* getVertexShaderSource(int _version = OMNIC_GL_SHADERVERSION) {
      static std::string _vertexSrc = 
        getShaderSource(_version,CalibrationVertexShader_120);
      return _vertexSrc.c_str();
    }
    
    /// Get calibration vertex shader source with version pragma
    inline static const char* getFragmentShaderSource(int _version = OMNIC_GL_SHADERVERSION) {
      static std::string _fragmentSrc = 
        getShaderSource(_version,CalibrationFragmentShader_120);
      return _fragmentSrc.c_str();
    }
  }
}

#endif /* OMNIC_GL_SHADER_H_ */
