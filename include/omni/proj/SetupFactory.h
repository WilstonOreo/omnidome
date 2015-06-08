#ifndef OMNI_PROJ_SETUPFACTORY_H_
#define OMNI_PROJ_SETUPFACTORY_H_

#include <omni/Factory.h>
#include "Setup.h"


namespace omni
{
  namespace proj
  {
    class SetupFactory : public Factory<Setup>
    {
    public:
      typedef std::unique_ptr<Setup> ptr_type;



    };
    

  }
}

#endif /* OMNI_PROJ_SETUPFACTORY_H_ */
