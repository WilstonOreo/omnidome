#ifndef OMNI_MAPPING_CUBEMAPSINGLE_H_
#define OMNI_MAPPING_CUBEMAPSINGLE_H_

#include <omni/mapping/Rotatable.h>

namespace omni
{
  namespace mapping
  {
    class CubeMapSingle : 
      public Rotatable,
      Registrar<CubeMapSingle>
    {
    public:
      OMNI_TYPEID("CubeMapSingle")

      CubeMapSingle(input::List const&);
      ~CubeMapSingle();

      MappingMode mode() const;
    };
  }
}

#endif /* OMNI_MAPPING_CUBEMAPSINGLE_H_ */
