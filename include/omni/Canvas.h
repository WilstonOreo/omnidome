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
#include <omni/Visualizer.h>

namespace omni {
  namespace ui {
    class CanvasParameters;
  }
  namespace visual {
    class Scene;
  }

  /**@brief Abstract interface for a canvas
     * @detail A canvas represents the surface on which the projection is
     *performed.
     *         It might be a dome or a planar surface.
     **/
    class Canvas :
      public QObject,
      public TypeIdInterface,
      public SerializationInterface,
      public PropertyMapSerializer,
      public Visualizer {
        Q_OBJECT
        enum ViewMode {
            VIEW_INSIDE,
            VIEW_OUTSIDE,
            VIEW_BOTH
        };
        Q_ENUM(ViewMode)
      private:
        OMNI_PROPERTY_RW_DEFAULT(ViewMode,viewMode,setViewMode,VIEW_BOTH)
        OMNI_PROPERTY_OBJ(AffineTransform,transform)
      public:
        Interface(QObject* = nullptr);

        /// Virtual destructor
        virtual ~Interface();

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

        /// Return ref to affine transform
        AffineTransform  * transform() const;

        /// Transformation matrix for canvas
        virtual QMatrix4x4 matrix() const;

#ifndef OMNI_DEAMON
        /// Returns pointer to new parameter widget
        virtual ui::CanvasParameters* widget() = 0;
#endif

      signals:
        void viewModeChanged();
        void geometryChanged();

      protected:
        bool needsUpdate_ = true;
    };

  /// Our canvas factory
  typedef AbstractFactory<Canvas> CanvasFactory;
}

#define OMNI_CANVAS_INTERFACE_IID  "org.omnidome.canvas.Interface"

Q_DECLARE_INTERFACE(omni::Canvas, OMNI_CANVAS_IID)

#define OMNI_CANVAS_PLUGIN_DECL                    \
  Q_OBJECT                                         \
  Q_PLUGIN_METADATA(IID OMNI_CANVAS_INTERFACE_IID) \
  Q_INTERFACES(omni::Canvas)                       \
  OMNI_PLUGIN_TYPE("Canvas")

#endif /* OMNI_CANVAS_INTERFACE_H_ */
