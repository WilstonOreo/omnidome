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

#include <omni/visual/Grid.h>
#include <omni/util.h>
#include <omni/visual/Shader.h>

namespace omni {
  namespace visual {
    ContextBoundPtr<QOpenGLShaderProgram> Grid::shader_;

    Grid::Grid(CameraInterface const* _camera) :
      camera_(_camera),
      size_(10.0, 10.0) {
      setResolution(QSize(1024, 1024));
      plane_.resize(2, 2);
    }

    Grid::~Grid() {}

    void Grid::draw() const {
      draw(1.0);
    }

    void Grid::draw(float _alpha) const {
      if (!shader_) return;
      withCurrentContext([&](QOpenGLFunctions& _) {
        shader_->bind();
        shader_->setUniformValue("resolution",
                                 GLfloat(resolution_.width() * size().x()),
                                 GLfloat(resolution_.height() * size().y()));
        shader_->setUniformValue("cam_pos", camera_->eye());
        shader_->setUniformValue("dir", camera_->direction().vec());
        shader_->setUniformValue("alpha", _alpha);
        glPushMatrix();
        {
          glScalef(size_.x(), size_.y(), 1.0);
          plane_.draw();
        }
        glPopMatrix();
        shader_->release();
      });
    }

    void Grid::update() {
      primaryContextSwitch([&](QOpenGLFunctions& _) {
        initShader(shader_,"grid");

        plane_.update();
      });
    }

    QSize const& Grid::resolution() const {
      return resolution_;
    }

    void Grid::setResolution(QSize _resolution) {
      resolution_ = _resolution;
    }

    QVector2D const& Grid::size() const {
      return size_;
    }

    void Grid::setSize(QVector2D const& _size) {
      size_=_size;
    }

    void Grid::setCamera(CameraInterface const* _camera) {
      camera_=_camera;
    }

    CameraInterface const* Grid::camera() const {
      return camera_;
    }
  }
}
