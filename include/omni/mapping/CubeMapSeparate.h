#ifndef OMNI_MAPPING_CUBEMAPSEPARATE_H_
#define OMNI_MAPPING_CUBEMAPSEPARATE_H_

#include <omni/mapping/Rotatable.h>

namespace omni
{
  namespace mapping
  {
    class CubeMapSeparate : 
      public Rotatable,
      Registrar<CubeMapSeparate>
    {
    public:
      OMNI_TYPEID("CubeMapSeparate")

      CubeMapSeparate(input::List const&);
        ~CubeMapSeparate();

      IdSet inputIDs() const;

      MappingMode mode() const;
    };
  }
}

#endif /* OMNI_MAPPING_CUBEMAPSEPARATE_H_ */
