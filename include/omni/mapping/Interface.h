/* Copyright (c) 2014-2015 "Omnidome" by cr8tr
 * Dome Mapping Projection Software (http://omnido.me).
 * Omnidome was created by Michael Winkelmann aka Wilston Oreo (@WilstonOreo)
 *
 * This file is part of Omnidome.
 *
 * Omnidome is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

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
      CUBEMAP, // Cube mapping
      NONE // Invalid Mode, default mode is used when set
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
      inline virtual bool isUVW() const
      {
        return true;
      }

      /// Static function to retrieve all registered mappings that support UVW coords
      static IdSet getUVWMappings();

      /// Static function to retrieve all registered mappings that dont support UVW coords
      static IdSet getPlanarMappings();

      /// Return pointer to parameter widget
      virtual QWidget* widget() = 0;

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
  }

  typedef mapping::Interface Mapping;
  typedef mapping::Factory MappingFactory;

  typedef mapping::Mode MappingMode;
  typedef std::set<MappingMode> MappingModeSet;
}


#define OMNI_MAPPING_INTERFACE_IID  "org.omnidome.mapping.Interface"

Q_DECLARE_INTERFACE(omni::mapping::Interface, OMNI_MAPPING_INTERFACE_IID)

#define OMNI_MAPPING_PLUGIN_DECL \
    Q_OBJECT \
    Q_PLUGIN_METADATA(IID OMNI_MAPPING_INTERFACE_IID) \
    Q_INTERFACES(omni::mapping::Interface)


#endif /* OMNI_MAPPING_INTERFACE_H_ */
