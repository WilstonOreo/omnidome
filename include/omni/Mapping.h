/* Copyright (c) 2014-2017 "Omnidome" by Michael Winkelmann
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

#ifndef OMNI_MAPPING_H_
#define OMNI_MAPPING_H_

#include <map>
#include <memory>
#include <omni/PluginInfo.h>
#include <omni/geometry/AffineTransform.h>
#include <omni/serialization/PropertyMapSerializer.h>
#include <omni/visual/Interface.h>

class QOpenGLShaderProgram;

namespace omni {

  namespace visual {
    class Scene;
  }

  class Input;

  namespace mapping {
    enum class OutputMode
    {
      MAPPED_INPUT,  // Draws actual input texture on mapping
      TEXCOORDS,     // Draw texture coordinates of mapping
      UVW,           // Draws uvw coordinates of mapping
      LIGHTING_ONLY, // Draw plain canvas with lighting
      LIGHTING_TEX   // Draw canvas with lighting
    };
  }


    /**@brief Mapping interface with one or several inputs and shader
     * @detail Holds inputs and shader
     */
    class Mapping :
      public QObject,
      public TypeIdInterface,
      public PropertyMapSerializer,
      public visual::Interface {
        Q_OBJECT
        OMNI_PROPERTY_RW_DEFAULT(bool,flipHorizontal,setFlipHorizontal,false)
        OMNI_PROPERTY_RW_DEFAULT(bool,flipVertical,setFlipVertical,false)
        OMNI_PROPERTY_RW_DEFAULT(bool,boundToCanvas,setBoundToCanvas,false)
        Q_PROPERTY(bool isUVW READ isUVW CONSTANT)
        OMNI_PROPERTY_OBJ(AffineTransform,transform)
      public:
        Mapping(QObject* = nullptr);

        virtual ~Mapping();

        /// Initialized OpenGL shader
        void         initialize();

        /// Bind shaders and set uniforms
        virtual void bind();
        void bind(Input const*, mapping::OutputMode, bool _grayscale);
        void bind(Input const*, float _transparency);

        /// Release shader
        void release();

        /**@brief Flag which tells if this mapping uses UVW texture coordinates
           (true by default)
           @detail
         **/
        inline virtual bool isUVW() const
        {
          return true;
        }

#ifndef OMNI_DAEMON
        /// Return pointer to parameter widget
        virtual QWidget* widget() = 0;
#endif

        inline void draw() const {
          draw(1.0);
        }

        /// Draw mapping with opacity
        inline virtual void draw(float _opacity) const {}

        /// Update mapping visualizer
        inline virtual void update() {}

        /// Return matrix of transform
        virtual QMatrix4x4 matrix() const;

        /// The scene this mapping belongs to
        visual::Scene const*       scene() const;

        /**@brief Set the scene this canvas belongs to.
           @detail Is set automatically when a canvas is added to a session
         **/
        void               setScene(visual::Scene const*);

      signals:
        void flipVerticalChanged();
        void flipHorizontalChanged();
        void boundToCanvasChanged();

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

        visual::Scene const* scene_ = nullptr;
    };

  /// Abstract mapping factory
  typedef AbstractFactory<Mapping> MappingFactory;
}

OMNI_DECL_ENUM_STREAM_OPERATORS(omni::mapping::OutputMode)

#define OMNI_MAPPING_IID  "org.omnidome.mapping.Interface"

Q_DECLARE_INTERFACE(omni::Mapping, OMNI_MAPPING_IID)

#define OMNI_MAPPING_PLUGIN_DECL                    \
  Q_OBJECT                                          \
  Q_PLUGIN_METADATA(IID OMNI_MAPPING_IID)           \
  Q_INTERFACES(omni::Mapping)                       \
  OMNI_PLUGIN_TYPE("Mapping")


#endif /* OMNI_MAPPING_H_ */
