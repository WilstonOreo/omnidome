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

#ifndef OMNIC_GL_FUNCTIONS_H_
#define OMNIC_GL_FUNCTIONS_H_

#include <omnic/gl/types.h>

namespace omnic
{
  namespace gl
  {
    template<GLuint FORMAT, typename T>
    void texImage(GLuint _target, int _width, int _height, void const* _ptr = nullptr)  
    {
      switch (_target) {
        case GL_TEXTURE_1D:
          glTexImage1D(_target,0, gl::PixelTypeId<FORMAT,T>::typeId(),
              _width,0,FORMAT,gl::TypeId<T>::typeId(), _ptr);
        break; 
        case GL_TEXTURE_2D:
        case GL_TEXTURE_RECTANGLE:
          glTexImage2D(_target, 0, gl::PixelTypeId<FORMAT,T>::typeId(),
                    _width,
                    _height, 0,
              FORMAT, gl::TypeId<T>::typeId(), _ptr);
        break;
      }
    }
    
    template<GLuint FORMAT, typename T, typename BUF>
    void texImage(GLuint _target, BUF const& _buf) {
      texImage<FORMAT,T>(_target, 
                traits::width(_buf),
                traits::height(_buf), 
                traits::ptr(_buf));
      }
  }
}

#endif /* OMNIC_GL_FUNCTIONS_H_ */
