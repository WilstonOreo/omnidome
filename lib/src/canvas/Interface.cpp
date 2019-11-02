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

#include <omni/canvas/Interface.h>

#include <omni/visual/util.h>

#include <omni/visual/Scene.h>
#include <omni/serialization/PropertyMap.h>

namespace omni {
    namespace canvas {
        Interface::factory_type Interface::factory_;

        Interface::Interface() {
            // Disable scaling and translation by default
            transform_.setTranslationEnabled(false);
            transform_.setScaleEnabled(false);
        }

        Interface::~Interface() {}

        /// Spectator's center of canvas (center of bounding box by default)
        QVector3D Interface::center() const
        {
            return (bounds().max() + bounds().min()) * 0.5;
        }
        /// Draws with culled front or back faces, depending on view mode
        void Interface::drawWithViewMode() const {
          withCurrentContext([&](QOpenGLFunctions& _) {
            switch (viewMode_) {
              case ViewMode::INSIDE:
              _.glEnable(GL_CULL_FACE);
              _.glCullFace(GL_FRONT);
              break;
              case ViewMode::OUTSIDE:
              _.glEnable(GL_CULL_FACE);
              _.glCullFace(GL_BACK);
              break;
              case ViewMode::BOTH:
              _.glDisable(GL_CULL_FACE);
              break;
            }
            this->draw();
            _.glDisable(GL_CULL_FACE);
          });
        }

        /// Canvas radius (is half of size by default)
        qreal Interface::radius() const
        {
            return bounds().size().length() * 0.5;
        }

        AffineTransform const& Interface::transform() const {
            return transform_;
        }

        AffineTransform& Interface::transform() {
            return transform_;
        }

        void Interface::setTransform(AffineTransform const& _transform) {
            transform_ = _transform;
        }

        Interface::ViewMode Interface::viewMode() const {
          return viewMode_;
        }

        void Interface::setViewMode(ViewMode _viewMode) {
          needsUpdate_ = viewMode_ != _viewMode;
          viewMode_ = _viewMode;
        }

        /// Transformation matrix for canvas
        QMatrix4x4 Interface::matrix() const
        {
            return transform_.matrix();
        }
        /// Write mapping to stream
        void Interface::toPropertyMap(PropertyMap& _map) const {
            _map("transform",transform_);
        }

        /// Read mapping from stream
        void Interface::fromPropertyMap(PropertyMap const& _map) {
            _map.get("transform",transform_);
        }

        Interface::factory_type& Interface::factory() {
            return factory_;
        }
    }
}
