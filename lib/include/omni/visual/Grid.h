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
 #ifndef OMNI_VISUAL_GRID_H_
 #define OMNI_VISUAL_GRID_H_

#include <memory>
#include <QSize>
#include <QVector2D>
#include <omni/visual/Interface.h>
#include <omni/visual/CameraInterface.h>
#include <omni/visual/Plane.h>
#include <omni/visual/ContextBoundPtr.h>

class QOpenGLShaderProgram;

namespace omni {
  namespace visual {
    /**@brief A cartesian grid with lining adapted to distance
    **/
    class OMNI_EXPORT Grid : public visual::Interface {
      public:
        Grid(CameraInterface const*);
        ~Grid();

        /// Draw grid
        void         draw() const;

        /// Draw grid with alpha value
        void         draw(float _alpha) const;

        /// Update grid mesh
        void         update();

        /**@brief Resolution
        **/
        void         setResolution(QSize _resolution);

        /// Return resolution
        QSize const& resolution() const;

        QVector2D const& size() const;

        /// Set size of grid
        void         setSize(QVector2D const& _size);

        void        setCamera(CameraInterface const*);
        CameraInterface const* camera() const;

      private:
        QSize resolution_;
        CameraInterface const* camera_ = nullptr;
        Plane plane_;
        QVector2D size_;
        static ContextBoundPtr<QOpenGLShaderProgram> shader_;
    };
  }
}

#endif /* OMNI_VISUAL_GRID_H_ */
