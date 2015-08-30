#ifndef FFGLOMNIUVW_H
#define FFGLOMNIUVW_H

#include <iostream>
#include "../ShaderParam.h"
#include "../FFGLPluginSDK.h"

namespace omni
{
  class FFGLOmniUVW :
    public CFreeFrameGLPlugin
  {
  public:
    enum class ParamId : DWORD {
      YAW,
      PITCH,
      ROLL,
      MODE,
      FLIP_HORIZONTAL,
      FLIP_VERTICAL
    };

    FFGLOmniUVW();
    virtual ~FFGLOmniUVW() {}

  	///////////////////////////////////////////////////
  	// FreeFrame plugin methods
	  ///////////////////////////////////////////////////

	  DWORD	SetParameter(const SetParameterStruct* pParam);
  	DWORD	GetParameter(DWORD dwIndex);
  	DWORD	ProcessOpenGL(ProcessOpenGLStruct *pGL);
    DWORD InitGL(const FFGLViewportStruct *vp);
    DWORD DeInitGL();

    ///////////////////////////////////////////////////
    // Factory method
	  ///////////////////////////////////////////////////

	  static DWORD __stdcall CreateInstance(CFreeFrameGLPlugin **ppOutInstance)
    {
      *ppOutInstance = new FFGLOmniUVW();
      std::cout << "OmnidomeFFGL: " << ppOutInstance << std::endl;

	    if (*ppOutInstance != NULL)
        return FF_SUCCESS;
	    return FF_FAIL;
    }

  private:
    ShaderParam<float,DWORD(ParamId::YAW)> yaw_;
    ShaderParam<float,DWORD(ParamId::PITCH)> pitch_;
    ShaderParam<float,DWORD(ParamId::ROLL)> roll_;
    ShaderParam<float,DWORD(ParamId::MODE)> mode_;
    ShaderParam<bool,DWORD(ParamId::FLIP_HORIZONTAL)> flipHorizontal_;
    ShaderParam<bool,DWORD(ParamId::FLIP_VERTICAL)> flipVertical_;

    template<typename PARAM>
    inline DWORD shaderParamValue(const SetParameterStruct* param, PARAM& _shaderParam) {
      if (!param) return FF_FAIL;

      _shaderParam.value() = *static_cast<typename PARAM::value_type*>((void*)(&param->NewParameterValue));
      return FF_SUCCESS;
    }

    template<typename PARAM, typename VALUE>
    inline void initShaderParam(PARAM& _param, char const* name, DWORD type, VALUE _value) {
      _param.value() =_value;
      SetParamInfo(_param.id(),name,type,_value);
    }

  	int initResources_ = 1;
	  FFGLExtensions extensions_;
	  FFGLShader shader_;
	  GLint inputLocation_ = -1;
    GLint uvwMapLocation_ = -1;
	  GLint inputSizeLocation_ = -1;
    GLint uvwMapSizeLocation_ = -1;
  };
}


#endif
