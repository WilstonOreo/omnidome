#ifndef OMNI_SHADER_PARAM_H_
#define OMNI_SHADER_PARAM_H_

#include <FFGLShader.h>

namespace omni
{
  /// Shader parameter template with id, value and location
  template<typename TYPE, DWORD ID>
  struct ShaderParam
  {
    typedef TYPE value_type;

    static constexpr DWORD id() { return ID; }


    /// Initialize shader location from shader
    inline void init(FFGLShader& _shader, const char* uniform)
    {
      location_ = _shader.FindUniform(uniform); 
    }

    /// Cast value to DWORD
    inline DWORD cast() const {

      return *(DWORD*)(&value());
    }

    inline GLint location() const {
      return location_;
    }

    inline value_type const& value() const {
      return value_;
    }
    
    inline value_type& value() {
      return value_;
    }

  private:
    GLint location_ = 0;
    value_type value_;
  };
}

#endif /* OMNI_SHADER_PARAM_H_ */
