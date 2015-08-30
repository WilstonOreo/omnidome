#include <FFGL.h> 
#include <FFGLLib.h>
#include "FFGLOmniUVW.h"
#include "OmniUVWShaderSource.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//  Plugin information
////////////////////////////////////////////////////////////////////////////////////////////////////

static CFFGLPluginInfo PluginInfo(
    omni::FFGLOmniUVW::CreateInstance, // Create method
    "FFGLOMNIUVW", // Plugin unique ID
    "FFGLOMNIUVW", // Plugin name
    1, // API major version number
    000, // API minor version number
    1, // Plugin major version number
    000, // Plugin minor version number
    FF_EFFECT, // Plugin type
    "Omnidome FFGL UVW Map plugin", // Plugin description
    "by Michael Winkelmann (cr8tr.org / omnido.me)" // About
);

char const* vertexShaderCode =
    "void main()"
"{"
"	 gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"
"  gl_TexCoord[0] = gl_MultiTexCoord0;"
"  gl_FrontColor = gl_Color;"
"}";


////////////////////////////////////////////////////////////////////////////////////////////////////
//  Constructor and destructor
////////////////////////////////////////////////////////////////////////////////////////////////////

namespace omni {

    FFGLOmniUVW::FFGLOmniUVW() {
            // Input properties
            SetMinInputs(2);
            SetMaxInputs(2);

            // Parameters
            initShaderParam(yaw_,"Yaw",FF_TYPE_STANDARD,0.0f);
            initShaderParam(pitch_,"Pitch",FF_TYPE_STANDARD,0.0f);
            initShaderParam(roll_,"Roll",FF_TYPE_STANDARD,0.0f);
            initShaderParam(mode_,"Map Mode",FF_TYPE_STANDARD, 0.0f);
            initShaderParam(flipVertical_,"Flip Horizontal",FF_TYPE_BOOLEAN,false);
            initShaderParam(flipHorizontal_,"Flip Vertical",FF_TYPE_BOOLEAN,false);    
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    //  Methods
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    DWORD FFGLOmniUVW::InitGL(const FFGLViewportStruct * vp) {
        extensions_.Initialize();

        if (extensions_.ARB_shader_objects == 0)
            return FF_FAIL;

        shader_.SetExtensions( & extensions_);
        shader_.Compile(vertexShaderCode, omni_uvw_shader_source_code);

        //activate our shader
        shader_.BindShader();

        //to assign values to parameters in the shader, we have to lookup
        //the "location" of each value.. then call one of the glUniform* methods
        //to assign a value
        inputLocation_ = shader_.FindUniform("image");
        uvwMapLocation_ = shader_.FindUniform("uvw_map");
        
        yaw_.init(shader_,"map_yaw");
        roll_.init(shader_,"map_roll");
        pitch_.init(shader_,"map_pitch");
        mode_.init(shader_,"map_mode");
        flipVertical_.init(shader_,"map_flip_vertical");
        flipHorizontal_.init(shader_,"map_flip_horizontal");

        //the 0 means that the 'inputTexture' in
        //the shader will use the texture bound to GL texture unit 0
        extensions_.glUniform1iARB(inputLocation_, 0);
        extensions_.glUniform1iARB(uvwMapLocation_, 1);

        shader_.UnbindShader();
        return FF_SUCCESS;
    }

    DWORD FFGLOmniUVW::DeInitGL() {
        shader_.FreeGLResources();
        return FF_SUCCESS;
    }

    DWORD FFGLOmniUVW::ProcessOpenGL(ProcessOpenGLStruct * pGL) {
        if (pGL->numInputTextures < 2)
            return FF_FAIL;

        if (pGL->inputTextures[0] == NULL)
            return FF_FAIL;

        if (pGL->inputTextures[1] == NULL)
            return FF_FAIL;
        
        //activate our shader
        shader_.BindShader();

        FFGLTextureStruct& uvwMapTex = * (pGL->inputTextures[0]);
        FFGLTextureStruct& inputTex = * (pGL->inputTextures[1]);

        //activate rendering with the input texture
        //note that when using shaders, no glEnable(Texture.Target) is required
        
        extensions_.glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, uvwMapTex.Handle);
 
        extensions_.glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, inputTex.Handle);


        //get the max s,t that correspond to the
        //width,height of the used portion of the allocated texture space
        FFGLTexCoords maxCoords = GetMaxGLTexCoords(uvwMapTex);

        extensions_.glUniform1fARB(yaw_.location(),yaw_.value()*360.0);
        extensions_.glUniform1fARB(pitch_.location(),pitch_.value()*360.0);
        extensions_.glUniform1fARB(roll_.location(),roll_.value()*360.0);
        extensions_.glUniform1fARB(mode_.location(),mode_.value()*3.0);
        extensions_.glUniform1iARB(flipVertical_.location(),flipVertical_.value());
        extensions_.glUniform1iARB(flipHorizontal_.location(),flipHorizontal_.value());

        extensions_.glUniform2fARB(inputSizeLocation_,(GLfloat)inputTex.Width,(GLfloat)inputTex.Height);
        extensions_.glUniform2fARB(uvwMapSizeLocation_,(GLfloat)uvwMapTex.Width,(GLfloat)uvwMapTex.Height);

        //draw the quad that will be painted by the shader/texture
        glBegin(GL_QUADS);

        //lower left
        glTexCoord2f(0, 0);
        glVertex2f(-1, -1);

        //upper left
        glTexCoord2f(0, maxCoords.t);
        glVertex2f(-1, 1);

        //upper right
        glTexCoord2f(maxCoords.s, maxCoords.t);
        glVertex2f(1, 1);

        //lower right
        glTexCoord2f(maxCoords.s, 0);
        glVertex2f(1, -1);
        glEnd();

        //unbind the shader and texture
        shader_.UnbindShader();

        //GL_TEXTURE1 is still active
        extensions_.glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);

        extensions_.glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
        
      //  glDisable(GL_TEXTURE_2D);

        return FF_SUCCESS;
    }

    DWORD FFGLOmniUVW::GetParameter(DWORD dwIndex) {

        switch (ParamId(dwIndex)) {
            case ParamId::YAW:
              return yaw_.cast();
            case ParamId::PITCH:
              return pitch_.cast();
            case ParamId::ROLL:
              return roll_.cast();
            case ParamId::MODE:
              return mode_.cast();
            case ParamId::FLIP_HORIZONTAL:
              return flipHorizontal_.cast();
            case ParamId::FLIP_VERTICAL:
              return flipVertical_.cast();
            default:
                return FF_FAIL;
        }
        return FF_FAIL;
    }

    DWORD FFGLOmniUVW::SetParameter(const SetParameterStruct * pParam) {
        if (pParam == nullptr) return FF_FAIL;
        
        switch (ParamId(pParam->ParameterNumber)) {

            case ParamId::YAW:
              return shaderParamValue(pParam,yaw_);
            case ParamId::PITCH:
              return shaderParamValue(pParam,pitch_);
            case ParamId::ROLL:
              return shaderParamValue(pParam,roll_);
            case ParamId::MODE:
              return shaderParamValue(pParam,mode_);
            case ParamId::FLIP_HORIZONTAL:
              return shaderParamValue(pParam,flipHorizontal_);
            case ParamId::FLIP_VERTICAL:
              return shaderParamValue(pParam,flipVertical_);
            default:
              return FF_FAIL;
        }

        return FF_SUCCESS;
    }
}
