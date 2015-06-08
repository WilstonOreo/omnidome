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
        CUBEMAP_SINGLE, // Cube mapping with a single input
        CUBEMAP_SEPARATE, // Cube mapping with 6 inputs for each side of the cube
        CYLINDRICAL // Cylindrical mapping with optional top and bottom image
      };

      Interface(input::List const& _inputList);

      virtual ~Interface();

      /// Returns mapping mode
      virtual Mode mode() const = 0;

      /// Initialized OpenGL shader
      void initializeShader();

      /// Bind shaders and set uniforms
      virtual void bindShader();

      /// Release shader
      void releaseShader();
      
      /// Returns true if mapping has only one input ID
      bool hasSingleInput() const;

      /**@brief Returns IDs which can be accepted as input
        *@detail Returns single id "input" by default.
                 If set to be returned is empty, any can accepted
       **/
      inline virtual IdSet inputIDs() const
      {
        return IdSet({ "input" });
      }

      /**@brief Add input with certain id
       * @detail Throws exception if mapping cannot be added
       * @param _id Id of mapping 
       * @param _input Id of input
       **/
      void addInput(Id const& _id, Id const&);

      /**@brief Remove input with certain id
        *@detail Nothing happens when id does not exist
       **/
      void removeInput(Id const& _id);

      virtual void fromStream(QDataStream&);
      virtual void toStream(QDataStream&) const;

      /// Returns stored id of stored input. nullptr is returned when input doesn't exist
      input::Interface const* input(Id const& _id) const;
 
    protected:
      std::unique_ptr<QOpenGLShaderProgram> shader_;

    private:
      /// Returns path to vertex shader file (is :/shader/common.vert by default)
      virtual QString vertexShaderFile() const;

      /// Abstract method for returning path to fragment shader (is :/shader/$TYPEID by default) 
      virtual QString fragmentShaderFile() const;

      /// Map with inputs
      std::map<Id,Id> inputs_;

      /// Reference to input list to retrieve inputs from id directly
      input::List const& inputList_;
    };

    /// Abstract mapping factory
    typedef AbstractFactory<Interface,input::List const&> Factory;

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
