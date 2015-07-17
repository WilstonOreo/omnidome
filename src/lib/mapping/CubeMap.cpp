#include <omni/mapping/CubeMap.h>

#include <QOpenGLShaderProgram>

namespace omni
{
  namespace mapping
  {
    CubeMap::CubeMap() 
    {
    }

    CubeMap::~CubeMap()
    {
    }
      
    MappingMode CubeMap::mode() const
    {
      return CUBEMAP;
    }
  }
}
