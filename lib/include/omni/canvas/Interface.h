/* Copyright (c) 2014-2016 "Omnidome" by Michael Winkelmann
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

#ifndef OMNI_CANVAS_INTERFACE_H_
#define OMNI_CANVAS_INTERFACE_H_

#include <set>
#include <QMatrix4x4>
#include <omni/PluginInfo.h>
#include <omni/geometry/Box.h>
#include <omni/geometry/AffineTransform.h>
#include <omni/serialization/PropertyMapSerializer.h>
#include <omni/mapping/Interface.h>
#include <omni/visual/Interface.h>

namespace omni {
  namespace ui {
    class CanvasParameters;
  }
  namespace visual {
    class Scene;
  }

  namespace canvas {
    /**@brief Abstract interface for a canvas
     * @detail A canvas represents the surface on which the projection is
     *performed.
     *         It might be a dome or a planar surface.
     **/
    class OMNI_EXPORT Interface :
      public TypeIdInterface,
      public PropertyMapSerializer,
      public visual::Interface {
      public:
        using factory_type = AbstractFactory<Interface>;

        Interface();

        /// Virtual destructor
        virtual ~Interface();

        enum class ViewMode {
            INSIDE,
            OUTSIDE,
            BOTH
        };

        /// Return current view mode
        ViewMode viewMode() const;

        /// Set view mode
        void setViewMode(ViewMode _viewMode);

        /// Draws with culled front or back faces, depending on view mode
        void drawWithViewMode() const;

        /// Draws auxiliary elements which are not used for mapping (e.g.
        // positioning grids)
        inline virtual void drawAux() const
        {}

        /// Abstract method for returning bounding box of canvas
        virtual Box            bounds() const = 0;

        /// Spectator's center of canvas (center of bounding box by default)
        virtual QVector3D      center() const;

        /// Canvas radius (is half of size by default)
        virtual qreal          radius() const;

        /// Return const ref to affine transform
        AffineTransform const& transform() const;

        /// Return ref to affine transform
        AffineTransform  & transform();

        /// Set new affine transform
        void               setTransform(AffineTransform const& _transform);

        /// Transformation matrix for canvas
        virtual QMatrix4x4 matrix() const;

        /// Returns pointer to new parameter widget
        virtual ui::CanvasParameters* widget() = 0;

        /// Write mapping to stream
        virtual void       toPropertyMap(PropertyMap&) const;

        /// Read mapping from stream
        virtual void       fromPropertyMap(PropertyMap const&);

        static factory_type& factory();

      protected:
        bool needsUpdate_ = true;

      private:
        AffineTransform transform_;
        ViewMode viewMode_ = ViewMode::BOTH;
        static factory_type factory_;
    };

    /// Our canvas factory
    typedef AbstractFactory<Interface>Factory;
  }

  typedef canvas::Interface Canvas;
  //typedef canvas::Factory   CanvasFactory;
}

#define OMNI_CANVAS_INTERFACE_IID  "org.omnidome.canvas.Interface"

Q_DECLARE_INTERFACE(omni::canvas::Interface, OMNI_CANVAS_INTERFACE_IID)

#define OMNI_CANVAS_PLUGIN_DECL                    \
  Q_OBJECT                                         \
  Q_PLUGIN_METADATA(IID OMNI_CANVAS_INTERFACE_IID) \
  Q_INTERFACES(omni::canvas::Interface)            \
  OMNI_PLUGIN_TYPE("Canvas")

#endif /* OMNI_CANVAS_INTERFACE_H_ */
