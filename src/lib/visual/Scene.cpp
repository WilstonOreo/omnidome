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

      connect(this,&Scene::sizeChanged,this,&Scene::updateGrid);
      connect(this,&Scene::currentCameraIdChanged,this,&Scene::updateGrid);
    }

    /// Setup light for use in OpenGL
    void Scene::updateLights() {
      GLuint _index = GL_LIGHT0;

      for (auto& _light : lights_) _light.setup(++_index);
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

    Scene::camera_map_type& Scene::cameras() {
      return cameras_;
    }

    Scene::camera_map_type const& Scene::cameras() const {
      return cameras_;
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
