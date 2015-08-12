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
    enum class Mode   
    {
      PLANAR, // Planar mapping 
      FISHEYE, // Fish eye mapping, e.g. for half domes
      EQUIRECTANGULAR, // Equirectangular mappings, e.g. for inflatable domes or fullsphere domes
      CUBEMAP // Cube mapping 
    };

    enum class OutputMode
    {
      MAPPED_INPUT, // Draws actual input texture on mapping
      TEXCOORDS, // Draw texture coordinates of mapping
      UVW // Draws uvw coordinates of mapping
    };


    /**@brief Mapping interface with one or several inputs and shader 
     * @detail Holds inputs and shader
     */
    class Interface : public SerializationInterface
    {
    public:
      Interface();

      virtual ~Interface();

      /// Returns mapping mode
      virtual Mode mode() const = 0;

      /// Initialized OpenGL shader
      void initialize();

      /// Bind shaders and set uniforms
      virtual void bind(OutputMode = OutputMode::MAPPED_INPUT);

      /// Release shader
      void release();

      /// Read mapping from stream
      virtual void fromStream(QDataStream&);
      
      /// Write mapping to stream
      virtual void toStream(QDataStream&) const;

      bool flipHorizontal() const;
      void setFlipHorizontal(bool);

      bool flipVertical() const;
      void setFlipVertical(bool);

      /// Flag which tells if this mapping uses UVW texture coordinates (true by default)
      inline virtual bool isUVW()
      {
        return true;
      }

      /// Static function to retrieve all registered mappings that support UVW coords
      static IdSet getUVWMappings();
      
      /// Static function to retrieve all registered mappings that dont support UVW coords
      static IdSet getPlanarMappings();

    protected:
      std::unique_ptr<QOpenGLShaderProgram> shader_;


    private:
      /**@brief Returns vertex shader source code 
       * @detail Is taken from file :/shaders/mapping/common.vert by default)
      **/
      virtual QString vertexShaderSourceCode() const;

      /**@brief Returns fragment shader source code 
       * @detail Is taken from file :/shaders/mapping/$MAPPING_TYPEID by default) 
      **/
      virtual QString fragmentShaderSourceCode() const;

      bool flipHorizontal_ = false;
      bool flipVertical_ = false;
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

  typedef mapping::Mode MappingMode;
  typedef std::set<MappingMode> MappingModeSet;
}

#endif /* OMNI_MAPPING_INTERFACE_H_ */
