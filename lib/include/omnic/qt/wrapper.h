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


#ifndef OMNIC_QT_WRAPPER_H_
#define OMNIC_QT_WRAPPER_H_

#include <QRect>
#include <omnic/Rect.h>

namespace omnic {

  namespace qt {
    /// Convert OMNIC rectangle to Qt rectangle
    static inline void wrap(omnic::Rect const& _in, QRect& _out) {
      _out = QRect(QPoint(_in.offsetX(),_in.offsetY()),QSize(_in.width(),_in.height()));
    }
    
    /// Convert Qt rectangle to OMNIC rectangle
    static inline void wrap(QRect const& _in, omnic::Rect& _out) {  
      _out = omnic::Rect(_in.left(),_in.top(),_in.width(),_in.height());
    }

    /// Convenience function for return value
    template<typename A, typename B>
    A wrap(B const& _b) {
      A _a;
      wrap(_b,_a);
      return _a;
    }
  }
}


#endif /* OMNIC_QT_WRAPPER_H_ */

