#ifndef OMNI_MAPPING_INTERFACE_H_
#define OMNI_MAPPING_INTERFACE_H_

#include <map>
#include <memory>
#include <omni/SerializationInterface.h>

class QOpenGLShaderProgram;

namespace omni
{
  namespace input
  {
    class Interface;
    class List;
  }

  namespace mapping
  {
    /**@brief Mapping interface with one or several inputs and shader 
     * @detail Holds inputs and shader
     */
    class Interface : public SerializationInterface
    {
    public:
      enum Mode 
      {
        PLANAR, // Planar mapping 
        FISHEYE, // Fish eye mapping, e.g. for half domes
        EQUIRECTANGULAR, // Equirectangular mappings, e.g. for inflatable domes or fullsphere domes
        CUBEMAP // Cube mapping 
      };

      Interface();

      virtual ~Interface();

      /// Returns mapping mode
      virtual Mode mode() const = 0;

      /// Initialized OpenGL shader
      void initialize();

      /// Bind shaders and set uniforms
      virtual void bind();

      /// Release shader
      void release();

      virtual void fromStream(QDataStream&);
      virtual void toStream(QDataStream&) const;

      /// Returns stored id of stored input. nullptr is returned when input doesn't exist
      input::Interface const* input(Id const& _id) const;
 
    protected:
      std::unique_ptr<QOpenGLShaderProgram> shader_;

    private:
      /**@brief Returns vertex shader source code 
       * @detail Ss taken from file :/shaders/mapping/common.vert by default)
      **/
      virtual QString vertexShaderSourceCode() const;

      /**@brief Returns fragment shader source code 
       * @detail Is taken from file :/shaders/mapping/$MAPPING_TYPEID by default) 
      **/
      virtual QString fragmentShaderSourceCode() const;
    };

    /// Abstract mapping factory
    typedef AbstractFactory<Interface> Factory;

    /// Template alias for our registrar (for auto registration)
    template<typename T>
    using Registrar = typename Factory::registrar_type<T>;
  }

  typedef mapping::Interface Mapping;
  typedef mapping::Factory MappingFactory;
  template<typename T>
  using MappingRegistrar = mapping::Registrar<T>;

  typedef mapping::Interface::Mode MappingMode;
  typedef std::set<MappingMode> MappingModeSet;
}

#endif /* OMNI_MAPPING_INTERFACE_H_ */
