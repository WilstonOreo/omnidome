#ifndef OMNI_MAPPING_CUBEMAP_H_
#define OMNI_MAPPING_CUBEMAP_H_

#include <omni/mapping/Rotatable.h>

namespace omni
{
  namespace mapping
  {
    class CubeMap : 
      public Rotatable,
      Registrar<CubeMap>
    {
    public:
      OMNI_TYPEID("CubeMap")

      CubeMap();
      ~CubeMap();

      MappingMode mode() const;
    };
  }
}

#endif /* OMNI_MAPPING_CUBEMAP_H_ */
