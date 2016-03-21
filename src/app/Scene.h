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

#include <memory>
#include <unordered_set>
#include "DockWidget.h"

namespace omni {
    namespace ui {
        class GLView3D;

        namespace Ui {
            class Scene;
        }

        /// Dock widget that contains view settings for the scene
        class Scene : public DockWidget {
            Q_OBJECT
        public:
            Scene(QWidget* _parent = nullptr);
            ~Scene();

            void registerView(GLView3D*);
            void unregisterView(GLView3D*);

        private:
            std::unique_ptr<Ui::Scene> ui_;

            std::unordered_set<GLView3D*> views_;
        };
    }
}
