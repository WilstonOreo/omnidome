#include <omni/mapping/CubeMapSingle.h>

#include <QOpenGLShaderProgram>

namespace omni
{
  namespace mapping
  {
    CubeMapSingle::CubeMapSingle(input::List const& _list) : 
      Rotatable(_list)
    {
    }

    CubeMapSingle::~CubeMapSingle()
    {
    }
      
    MappingMode CubeMapSingle::mode() const
    {
      return CUBEMAP_SINGLE;
    }
  }
}
