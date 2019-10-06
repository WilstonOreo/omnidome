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

#ifndef OMNIC_UTIL_H_
#define OMNIC_UTIL_H_

#if DEBUG
#include <cassert>
#define OMNIC_ASSERT(TXT) assert(TXT)
#define OMNIC_DEBUG() std::cerr << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ":\t" 
#else
#define OMNIC_ASSERT(TXT) ((void)0)
#define OMNIC_DEBUG()
#endif

 #ifdef OMNIC_DOXYGEN_INVOKED
 #define OMNIC_STRINGIZE_SOURCE(source)
 #else
 #define OMNIC_STRINGIZE_SOURCE(...) #__VA_ARGS__
 #endif
 
#define OMNIC_SOURCE(VAR,...) \
  const char VAR[] = OMNIC_STRINGIZE_SOURCE(...)

namespace omnic {
  namespace util {
    /// Read binary encoded value from stream (e.g. std::istream)
    template<typename STREAM, typename T>
    void readBinary(STREAM& _stream, T& _v) {
      constexpr size_t _size = sizeof(T);
      _stream.read((char*)(&_v),_size);
    }
    
    /// Write binary encoded value to stream (e.g. std::ostream)
    template<typename STREAM, typename T>
    void writeBinary(STREAM& _stream, T const& _v) {
      constexpr size_t _size = sizeof(T);
      _stream.write((char*)(&_v),_size);
    }
   
    /// Clamp copy of value between min and max value and return clampled value
    template<typename T>
    T clamp(T const& _value, T const& _min, T const& _max) {
      if (_value < _min) { return _min; }
      if (_value > _max) { return _max; }
      return _value;
    }
  } 
}

#endif /* OMNIC_UTIL_H_ */

