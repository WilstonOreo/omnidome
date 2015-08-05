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

      /// Return supported map modes (if set is empty, all map modes are supported)
      inline virtual MappingModeSet mapModes() const
      {
        return  MappingModeSet({  });
      }
      
      /// An input must return an OpenGL texture ID 
      virtual GLuint textureId() const = 0;

      /// Update interface
      inline virtual void update() {}

      /**@brief Free stored OpenGL Content (like textures, shaders etc)
       * @detail Is called before destructor, when there is still an active OpenGL context
       **/
      inline virtual void free() {}

      /// Returns an optional string with some basic information about the object
      inline virtual QString infoText() const
      {
        return QString();
      };

      /// An input must return width and height information
      inline virtual QSize size() const = 0;

      /// Return width from size
      inline int width() const
      {
        return size().width();
      }

      /// Return height from size
      inline int height() const
      {
        return size().height();
      }

      /**@brief An Input may have an optional ruler position
       * @detail Position is between (0.0,0.0) and (1.0,1.0)
       **/
      virtual inline QPointF rulerPos() const
      {
        return QPointF(-1.0,-1.0);
      }

      /// Set ruler position. 
      virtual inline void setRulerPos(QPointF const&) {}

      /// True, if this input accepts a mapping, false for test images
      virtual inline bool acceptMapping() const { return true; }
      
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
