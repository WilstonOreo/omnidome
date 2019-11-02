/* Copyright (c) 2014-2015 "Omnidome" by Michael Winkelmann
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
#include <omni/PluginInfo.h>
#include <omni/geometry/AffineTransform.h>
#include <omni/serialization/PropertyMapSerializer.h>
#include <omni/visual/Interface.h>

class QOpenGLShaderProgram;

namespace omni {
  namespace input {
    class Interface;
    class List;
  }
  namespace visual {
    class Scene;
  }

  namespace mapping {
    enum class OutputMode
    {
      MAPPED_INPUT,  // Draws actual input texture on mapping
      TEXCOORDS,     // Draw texture coordinates of mapping
      UVW,           // Draws uvw coordinates of mapping
      LIGHTING_ONLY, // Draw plain canvas with lighting
      LIGHTING_TEX   // Draw canvas with
    };

    /**@brief Mapping interface with one or several inputs and shader
     * @detail Holds inputs and shader
     */
    class OMNI_EXPORT Interface :
      public TypeIdInterface,
      public PropertyMapSerializer,
      public visual::Interface {
      public:
        using factory_type = AbstractFactory<Interface>;

        Interface();

        virtual ~Interface();

        /// Initialized OpenGL shader
        void         initialize();

        /// Bind shaders and set uniforms
        virtual void bind();
        void bind(input::Interface const*, OutputMode, bool _grayscale);
        void bind(input::Interface const*, float _transparency);

        /// Release shader
        void release();

        /// Flip horizontally
        bool flipHorizontal() const;

        /// Flip horizontally
        void setFlipHorizontal(bool);

        /// Flip vertically
        bool flipVertical() const;

        /// Flip vertically
        void setFlipVertical(bool);

        /**@brief Flag which tells if this mapping uses UVW texture coordinates
           (true by default)
           @detail
         **/
        inline virtual bool isUVW() const
        {
          return true;
        }

        /// Static function to retrieve all registered mappings that support UVW
        // coords
        static IdSet     getUVWMappings();

        /// Static function to retrieve all registered mappings that dont
        // support UVW coords
        static IdSet     getPlanarMappings();

        /// Return pointer to parameter widget
        virtual QWidget* widget() = 0;

        inline void draw() const {
          draw(1.0);
        }

        /// Draw mapping with opacity
        inline virtual void draw(float _opacity) const {}

        /// Update mapping visualizer
        inline virtual void update() {}

        /// Return const ref to affine transform
        AffineTransform const& transform() const;

        /// Return ref to affine transform
        AffineTransform  & transform();

        /// Set new affine transform
        void               setTransform(AffineTransform const& _transform);

        /// Return matrix of transform
        virtual QMatrix4x4 matrix() const;

        /**@brief If true, mapping transform is attached to canvas transform
           @detail Is true by default
         **/
        bool               isBoundToCanvas() const;

        /// Set whether mapping transform is attached to canvas transform
        void               setBoundToCanvas(bool);

        /// Write mapping to stream
        virtual void       toPropertyMap(PropertyMap&) const;

        /// Read mapping from stream
        virtual void       fromPropertyMap(PropertyMap const&);

        /// The scene this canvas belongs to
        visual::Scene const*       scene() const;

        /**@brief Set the scene this canvas belongs to.
           @detail Is set automatically when a canvas is added to a session
         **/
        void               setScene(visual::Scene const*);

        static factory_type& factory();

      protected:
        std::unique_ptr<QOpenGLShaderProgram> shader_;

      private:
        /**@brief Returns vertex shader source code
         * @detail Is taken from file :/shaders/mapping/common.vert by default)
         **/
        virtual QString vertexShaderSourceCode() const;

        /**@brief Returns fragment shader source code
         * @detail Is taken from file :/shaders/mapping/$MAPPING_TYPEID by
         *default)
         **/
        virtual QString fragmentShaderSourceCode() const;

        AffineTransform transform_;
        bool boundToCanvas_  = true;
        bool flipHorizontal_ = false;
        bool flipVertical_   = false;

        visual::Scene const* scene_ = nullptr;
        static factory_type factory_;
    };

    /// Abstract mapping factory
    typedef AbstractFactory<Interface>Factory;
  }

  typedef mapping::Interface Mapping;
  typedef mapping::Factory   MappingFactory;
}

OMNI_DECL_ENUM_STREAM_OPERATORS(omni::mapping::OutputMode)

#define OMNI_MAPPING_INTERFACE_IID  "org.omnidome.mapping.Interface"

Q_DECLARE_INTERFACE(omni::mapping::Interface, OMNI_MAPPING_INTERFACE_IID)

#define OMNI_MAPPING_PLUGIN_DECL                    \
  Q_OBJECT                                          \
  Q_PLUGIN_METADATA(IID OMNI_MAPPING_INTERFACE_IID) \
  Q_INTERFACES(omni::mapping::Interface)            \
  OMNI_PLUGIN_TYPE("Mapping")


#endif /* OMNI_MAPPING_INTERFACE_H_ */
