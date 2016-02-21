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

#include <omni/visual/Grid.h>
#include <omni/util.h>
#include <QOpenGLShaderProgram>

namespace omni {
    namespace visual {
        Grid::Grid(Camera const& _camera) :
            camera_(_camera),
            size_(10.0,10.0) {
            setResolution(QSize(1024,1024));
            plane_.resize(2,2);

        }

        Grid::~Grid() {
        }

        void Grid::draw() const {
            draw(1.0);
        }

        void Grid::draw(float _alpha) const {

            with_current_context([&](QOpenGLFunctions& _) {

                    shader_->bind();
                    shader_->setUniformValue("resolution",
                        GLfloat(resolution_.width()),
                        GLfloat(resolution_.height()));
                    shader_->setUniformValue("cam_pos",camera_.eye());
                    shader_->setUniformValue("dir",camera_.direction().vec());
                    shader_->setUniformValue("alpha",_alpha);
                glPushMatrix();
                {
                    glScalef(size_.x(),size_.y(),1.0);
                    plane_.draw();
                }
                glPopMatrix();
                    shader_->release();

            });

        }

        void Grid::update() {
            if (!shader_) {
                using omni::util::fileToStr;
                static QString _vertSrc = fileToStr(":/shaders/grid.vert");
                static QString _fragmentSrc = fileToStr(":/shaders/grid.frag");
                shader_.reset(new QOpenGLShaderProgram());
                shader_->addShaderFromSourceCode(QOpenGLShader::Vertex,_vertSrc);
                shader_->addShaderFromSourceCode(QOpenGLShader::Fragment,_fragmentSrc);
                shader_->link();
            }

            plane_.update();
        }

        QSize const& Grid::resolution() const {
            return resolution_;
        }

        void Grid::setResolution(QSize _resolution) {
            resolution_ = _resolution;
        }
    }
}
