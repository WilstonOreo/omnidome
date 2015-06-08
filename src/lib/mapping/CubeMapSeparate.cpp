#include <omni/mapping/CubeMapSeparate.h>

#include <QOpenGLShaderProgram>

namespace omni
{
  namespace mapping
  {
    CubeMapSeparate::CubeMapSeparate(input::List const& _list) :
      Rotatable(_list)
    {
    }

    CubeMapSeparate::~CubeMapSeparate()
    {
    }

    IdSet CubeMapSeparate::inputIDs() const
    {
      return IdSet({ "top", "bottom", "left", "right", "front", "back" });
    }

    MappingMode CubeMapSeparate::mode() const
    {
      return CUBEMAP_SEPARATE;
    }
  }
}
