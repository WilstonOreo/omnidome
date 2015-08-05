#ifndef OMNI_PROJ_SETUP_H_
#define OMNI_PROJ_SETUP_H_

#include <QDataStream>
#include <omni/SerializationInterface.h>

namespace omni
{
  namespace proj
  {
    class Projector;

    class Setup : public SerializationInterface
    {
    public:
      virtual void setup(Projector&) = 0;
    };

    /// Typedef for our factory
    typedef AbstractFactory<Setup> SetupFactory;

    /// Template alias for our registrar (for auto registration)
    template<typename T>
    using SetupRegistrar = typename SetupFactory::registrar_type<T>; 
  }

  typedef proj::Setup ProjectorSetup;
}

#endif /* OMNI_PROJ_SETUP_H_ */ 
