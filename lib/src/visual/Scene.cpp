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

#include <omni/visual/Scene.h>

#include <omni/visual/PerspectiveCamera.h>
#include <omni/serialization/PropertyMap.h>
#include <omni/util.h>
#include <omni/serialization/container.h>

namespace omni {
  namespace visual {
    Scene::Scene() {
      setSize(size_);
      float _radius = size_;
      std::unique_ptr<CameraInterface> _cam(new PerspectiveCamera(
                                              Tracker(QVector3D(0,
                                                                0,
                                                                0), PolarVec(
                                                        -45.0,
                                                        45.0,
                                                        _radius * 5.0))));
      cameras_["camera"] = std::move(_cam);
      setCurrentCameraId("camera");

      lights_.emplace_back(Tracker(QVector3D(0.0, 0.0, 0.0),
                                   PolarVec(-45.0, 45.0, _radius * 10.0)), 0.5);
      lights_.emplace_back(Tracker(QVector3D(0.0, 0.0, 0.0),
                                   PolarVec(45.0, 45.0, _radius * 10.0)), 0.3);
      lights_.emplace_back(Tracker(QVector3D(0.0, 0.0, 0.0),
                                   PolarVec(45.0, -45.0, _radius * 10.0)), 0.3);
    }

    float Scene::size() const {
      return size_;
    }

    void Scene::setSize(float _size) {
      size_ = _size;
      updateGrid();
    }

    LengthUnit const& Scene::unit() const {
      return unit_;
    }

    void Scene::setUnit(QString const& _unit) {
      unit_ = LengthUnit(_unit);
    }

    void Scene::setUnit(LengthUnit const& _unit) {
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
    ProjectorSelectionMode Scene::displayProjectors() const {
      return displayProjectors_;
    }

    void Scene::setDisplayProjectors(ProjectorSelectionMode _displayProjectors) {
      displayProjectors_ = _displayProjectors;
    }

    /// Display projected areas
    ProjectorSelectionMode Scene::displayProjectedAreas() const {
      return displayProjectedAreas_;
    }

    void Scene::setDisplayProjectedAreas(ProjectorSelectionMode _displayProjectedAreas) {
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

    /// Setup light for use in OpenGL
    void Scene::updateLights() {
      GLuint _index = GL_LIGHT0;

      for (auto& _light : lights_) _light.setup(++_index);
    }

    /// Inside / outside transparency for 3D canvas view
    float Scene::insideOutside() const {
      return insideOutside_;
    }

    void Scene::setInsideOutside(float _insideOutside) {
      insideOutside_ = qBound(0.0f,_insideOutside,1.0f);
    }

    /// Opacity value for wireframe between 0.0 and 1.0
    float Scene::wireframe() const {
      return wireframe_;
    }

    /* @brief Set wireframe opacity for canvas view
      @param _wireframe Value 0.0 means wireframes are not drawn.
            1.0 means wireframe is fully visible and thick.
      */
    void Scene::setWireframe(float _wireframe) {
      wireframe_ = qBound(0.0f,_wireframe,1.0f);
    }

    /// Return reference to camera
    visual::CameraInterface * Scene::camera() {
      if (cameras_.count(cameraId_) == 0) return nullptr;

      return cameras_.at(cameraId_).get();
    }

    /// Return const reference to camera
    visual::CameraInterface const * Scene::camera() const {
      if (cameras_.count(cameraId_) == 0) return nullptr;

      return cameras_.at(cameraId_).get();
    }

    /// Return current camera id
    QString Scene::currentCameraId() const {
      return cameraId_;
    }

    /// Set id to current camera
    void Scene::setCurrentCameraId(QString const& _cameraId) {
      cameraId_ = _cameraId;

      updateGrid();
    }

    Scene::camera_map_type& Scene::cameras() {
      return cameras_;
    }

    Scene::camera_map_type const& Scene::cameras() const {
      return cameras_;
    }

    /// Deserialize from stream
    void Scene::fromStream(QDataStream& _is) {
      PropertyMap _map;

      _is >> _map;
      _map.get("size",                 size_);
      _map.get("displayInput",          displayInput_);
      _map.get("displayGrid",           displayGrid_);
      _map.get("displayProjectors",     displayProjectors_);
      _map.get("displayProjectedAreas", displayProjectedAreas_);
      _map.get("editMode",              editMode_);
      _map.get("rotateMode",            rotateMode_);
      _map.get("moveMode",              moveMode_);
      _map.get("insideOutside",     insideOutside_);
      _map.get("lights", lights_);
    }

    /// Serialize to stream
    void Scene::toStream(QDataStream& _os) const {
      PropertyMap _map;

      _map("size", size_)
        ("displayInput", displayInput_)
        ("displayGrid", displayGrid_)
        ("displayProjectors", displayProjectors_)
        ("displayProjectedAreas", displayProjectedAreas_)
        ("editMode", editMode_)
        ("rotateMode", rotateMode_)
        ("moveMode", moveMode_)
        ("insideOutside", insideOutside_)
        ("lights", lights_);

      _os << _map;
    }

    bool operator==(Scene const& _lhs, Scene const& _rhs) {
      return
        OMNI_TEST_MEMBER_EQUAL(size_) &&
        OMNI_TEST_MEMBER_EQUAL(displayInput_) &&
        OMNI_TEST_MEMBER_EQUAL(displayGrid_) &&
        OMNI_TEST_MEMBER_EQUAL(displayProjectors_) &&
        OMNI_TEST_MEMBER_EQUAL(displayProjectedAreas_) &&
        OMNI_TEST_MEMBER_EQUAL(editMode_) &&
        OMNI_TEST_MEMBER_EQUAL(rotateMode_) &&
        OMNI_TEST_MEMBER_EQUAL(moveMode_) &&
        OMNI_TEST_MEMBER_EQUAL(lights_) &&
        OMNI_TEST_MEMBER_EQUAL(cameras_);
    }

    void Scene::drawGrid() const {
      if (!grid_ || !displayGrid()) return;

      grid_->draw(insideOutside());
      glDisable(GL_DEPTH_TEST);
      grid_->draw(1.0 - insideOutside());
      glEnable(GL_DEPTH_TEST);
    }

    void Scene::updateGrid() {
      if (!grid_) {
        grid_.reset(new visual::Grid(camera()));
      }
      grid_->setSize(QVector2D(size_, size_));
      grid_->update();
      grid_->setCamera(camera());
    }
  }
}
