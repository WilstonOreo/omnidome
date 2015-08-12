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
      
    Mode CubeMap::mode() const
    {
      return Mode::CUBEMAP;
    }
  }
}
