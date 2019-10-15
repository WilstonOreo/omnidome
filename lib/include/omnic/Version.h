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
#ifndef OMNIC_VERSION_H_
#define OMNIC_VERSION_H_

#include <omnic/util.h>

namespace omnic {
  /// Simple struct for holding version numbers
  struct Version {
    /// Construct with latest version by default
    Version(
        uint8_t _major = OMNI_MAJOR_VERSION,
        uint8_t _minor = OMNI_MINOR_VERSION,
        uint16_t _patch = OMNI_PATCH_VERSION) :
      major_(_major),
      minor_(_minor),
      patch_(_patch) {}

    /// Return current version that is defined by version macros
    inline static Version current() {
      return Version();
    }

    /// Id of version to compare
    inline uint32_t id() const {
      return (major_ << 24) | (minor_ << 16) | patch_;
    }
 
    /// Overload less operator for comparing two versions
    friend bool operator <(const Version& lhs, const Version& rhs) //friend claim has to be here
    {
      return lhs.id() < rhs.id();
    }
    
    friend bool operator <=(const Version& lhs, const Version& rhs) //friend claim has to be here
    {
      return (lhs < rhs) || (lhs == rhs);
    }
    
    friend bool operator ==(const Version& lhs, const Version& rhs) //friend claim has to be here
    {
      return lhs.id() == rhs.id();
    }

    /// Major version
    inline uint32_t major() const {
      return major_;
    }

    /// Minor version
    inline uint32_t minor() const {
      return minor_;
    }
    
    /// Patch version
    inline uint32_t patch() const {
      return patch_;
    }
    
    /// Load version from stream
    template<typename STREAM>
    void load(STREAM& _is) {
      using namespace util;
      readBinary(_is,major_);
      readBinary(_is,minor_);
      readBinary(_is,patch_);
    }

    /// Save version to stream
    template<typename STREAM>
    void save(STREAM& _os) const {
      using namespace util;
      writeBinary(_os,major_);
      writeBinary(_os,minor_);
      writeBinary(_os,patch_);
    }

  private:
    uint8_t major_;
    uint8_t minor_;
    uint16_t patch_;
  };
}

#endif /* OMNIC_VERSION_H_ */
