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

#include <omni/visual/Scene.h>

#include <omni/serialization/PropertyMap.h>
#include <omni/util.h>

namespace omni {
    namespace visual {
        float Scene::scale() const {
            return scale_;
        }

        void Scene::setScale(float _scale) {
            scale_ = _scale;
        }

        QString const& Scene::unit() const {
            return unit_;
        }

        void Scene::setUnit(QString const& _unit) {
            unit_ = _unit;
        }

        /// Input is shown on canvas
        bool Scene::displayInput() const {
            return displayInput_;
        }

        void Scene::setDisplayInput(bool _displayInput) {
            displayInput_ = _displayInput;
        }

        /// Display line grid
        bool Scene::displayGrid() const {
            return displayGrid_;
        }

        void Scene::setDisplayGrid(bool _displayGrid) {
            displayGrid_ = _displayGrid;
        }

        /// Display projector frustra (selected projector is always shown)
        bool Scene::displayProjectors() const {
            return displayProjectors_;
        }

        void Scene::setDisplayProjectors(bool _displayProjectors) {
            displayProjectors_ = _displayProjectors;
        }

        /// Display projected areas
        bool Scene::displayProjectedAreas() const {
            return displayProjectedAreas_;
        }

        void Scene::setDisplayProjectedAreas(bool _displayProjectedAreas) {
            displayProjectedAreas_ = _displayProjectedAreas;
        }

        /// Set mode whether to manip
        EditMode Scene::editMode() const {
            return editMode_;
        }

        void Scene::setEditMode(EditMode _editMode) {
            editMode_ = _editMode;
        }

        RotateMode Scene::rotateMode() const {
            return rotateMode_;
        }

        void Scene::setRotateMode(RotateMode _rotateMode) {
            rotateMode_ = _rotateMode;
        }

        MoveMode Scene::moveMode() const {
            return moveMode_;
        }

        void Scene::setMoveMode(MoveMode _moveMode) {
            moveMode_ = _moveMode;
        }

        ProjectorViewMode Scene::projectorViewMode() const {
            return projectorViewMode_;
        }

        void Scene::setProjectorViewMode(ProjectorViewMode _projectorViewMode) {
            projectorViewMode_ = _projectorViewMode;
        }

        /// Setup light for use in OpenGL
        void Scene::updateLights() {
            GLuint _index = GL_LIGHT0;

            for (auto& _light : lights_) _light.setup(++_index);
        }

        /// Deserialize from stream
        void Scene::fromStream(QDataStream& _is) {
            PropertyMap _map;

            _is >> _map;
            _map.get("scale",                 scale_);
            _map.get("displayInput",          displayInput_);
            _map.get("displayGrid",           displayGrid_);
            _map.get("displayProjectors",     displayProjectors_);
            _map.get("displayProjectedAreas", displayProjectedAreas_);
            _map.get("editMode",              editMode_);
            _map.get("rotateMode",            rotateMode_);
            _map.get("moveMode",              moveMode_);
            _map.get("projectorViewMode",     projectorViewMode_);
        }

        /// Serialize to stream
        void Scene::toStream(QDataStream& _os) const {
            PropertyMap _map;

            _map("scale", scale_)
                ("displayInput", displayInput_)
                ("displayGrid", displayGrid_)
                ("displayProjectors", displayProjectors_)
                ("displayProjectedAreas", displayProjectedAreas_)
                ("editMode", editMode_)
                ("rotateMode", rotateMode_)
                ("moveMode", moveMode_)
                ("projectorViewMode", projectorViewMode_);
            _os << _map;
        }

        bool operator==(Scene const& _lhs, Scene const& _rhs) {
            return
                OMNI_TEST_MEMBER_EQUAL(scale_) &&
                OMNI_TEST_MEMBER_EQUAL(displayInput_) &&
                OMNI_TEST_MEMBER_EQUAL(displayGrid_) &&
                OMNI_TEST_MEMBER_EQUAL(displayProjectors_) &&
                OMNI_TEST_MEMBER_EQUAL(displayProjectedAreas_) &&
                OMNI_TEST_MEMBER_EQUAL(editMode_) &&
                OMNI_TEST_MEMBER_EQUAL(rotateMode_) &&
                OMNI_TEST_MEMBER_EQUAL(moveMode_) &&
                OMNI_TEST_MEMBER_EQUAL(projectorViewMode_);
        }
    }
}
