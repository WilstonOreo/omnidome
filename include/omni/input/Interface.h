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
    /// Generic input interface
    class Interface : public SerializationInterface
    {
    public:
      /// Virtual destructor
      virtual ~Interface();

      /// Bind input to OpenGL context (to be implement)
      virtual void bind() = 0;

      /// Release input from OpenGL context
      inline virtual void release() {};

      /// Return supported map modes (if set is empty, all map modes are supported)
      inline virtual MappingModeSet mapModes() const
      {
        return  MappingModeSet({  });
      }

      /// Update interface
      inline virtual void update() {}

      /**@brief Free stored OpenGL Content (like textures, shaders etc)
       * @detail Is called before destructor, when there is still an active OpenGL context 
       **/
      inline virtual void free() {}

      /// Return texture id
      inline virtual GLuint textureId() const 
      {
        return 0;
      }

    };

    /// Input Factory typedef 
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
