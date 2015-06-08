#ifndef OMNI_INPUT_INTERFACE_H_
#define OMNI_INPUT_INTERFACE_H_

#include <set>
#include <QOpenGLTexture>
#include <omni/SerializationInterface.h>
#include <omni/mapping/Interface.h>

namespace omni
{
  namespace input
  {
    class Interface : public SerializationInterface
    {
    public:
      virtual ~Interface();

      virtual void bind() = 0;

      inline virtual MappingModeSet mapModes() const
      {
        return  MappingModeSet({  });
      }

      virtual void update() = 0;

      virtual GLuint textureId() const = 0;
    };

    typedef AbstractFactory<Interface> Factory;

    /// Template alias for our registrar (for auto registration)
    template<typename T>
    using Registrar = typename Factory::registrar_type<T>;
  }

  typedef input::Interface Input;
  typedef input::Factory InputFactory;

  template<typename T>
  using InputRegistrar = input::Registrar<T>;
}

#endif /* OMNI_INPUT_INTERFACE_H_ */
