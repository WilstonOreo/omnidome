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

#include <omni/visual/Projector.h>

#include <omni/util.h>
#include <omni/visual/Shader.h>
#include <omni/proj/Frustum.h>

namespace omni {
    namespace visual {
        ContextBoundPtr<QOpenGLShaderProgram> Projector::haloShader_;

        Projector::Projector(const proj::Projector& _proj) :
            proj_(_proj)
        {
            update();
        }

        qreal Projector::size() const
        {
            return size_;
        }

        void Projector::setSize(qreal _size)
        {
            size_ = _size;
            update();
        }

        bool Projector::isSelected() const
        {
            return selected_;
        }

        void Projector::setSelected(bool _selected)
        {
            selected_ = _selected;
        }

        QColor Projector::color() const
        {
            return color_;
        }

        void Projector::setColor(QColor _color)
        {
            color_ = _color;
        }

        void Projector::update()
        {
          withCurrentContext([&](QOpenGLFunctions& _) {
            initShader(haloShader_,"halo");
          });

            proj::Frustum _frustum(proj_);
            eye_         = QVector3D(0, 0, 0);
            topLeft_     = _frustum.topLeft() *size_;
            topRight_    = _frustum.topRight() *size_;
            bottomLeft_  = _frustum.bottomLeft() *size_;
            bottomRight_ = _frustum.bottomRight() * size_;
        }

        void Projector::draw() const
        {
            Interface::color(color_);

            withCurrentContext([this](QOpenGLFunctions& _)
            {
                _.glLineWidth(selected_ ? 2.0 : 1.0);

                glPushMatrix();
                {
                    glLoadIdentity();
                    _.glDisable(GL_COLOR_MATERIAL);
                    _.glDisable(GL_LIGHTING);

                    // Apply matrix to OpenGL
                    glMultMatrixf(proj_.matrix().constData());
                    this->visualLine(eye_, topLeft_);
                    this->visualLine(eye_, topRight_);
                    this->visualLine(eye_, bottomLeft_);
                    this->visualLine(eye_, bottomRight_);
                    this->visualLine(topLeft_, topRight_);
                    this->visualLine(topLeft_, bottomLeft_);
                    this->visualLine(topRight_, bottomRight_);
                    this->visualLine(bottomLeft_, bottomRight_);
                }
                glPopMatrix();
            });
        }

        void Projector::drawPositioning(QVector3D const& _center) const
        {
            Interface::color(color_);
            withCurrentContext([this](QOpenGLFunctions& _)
            {
              _.glLineWidth(selected_ ? 4.0 : 1.5);
            });

            auto _p       = proj_.pos();

            switch (proj_.setup()) {
            default:
            case proj::Projector::FREE:
              // Draw manhattan line from center to projector ground position
              this->visualLine(_center,QVector3D(_p.x(), _center.y(), _center.z()));
              this->visualLine(QVector3D(_p.x(), _center.y(), _center.z()),
                              QVector3D(_p.x(), _p.y(),      _center.z()));
              this->visualLine(QVector3D(_p.x(), _p.y(),      _center.z()), _p);
              break;
            case proj::Projector::PERIPHERAL:
              // Draw line from center to projector ground position
              this->visualLine(_center, QVector3D(_p.x(), _p.y(), _center.z()));
              this->visualLine(QVector3D(_p.x(), _p.y(), _center.z()), _p);
            break;
            }
        }

        void Projector::drawHalo() const
        {
          if (!haloShader_) return;
            glPushMatrix();
            {
                glLoadIdentity();

                // Apply matrix to OpenGL
                glMultMatrixf(proj_.matrix().data());
                haloShader_->bind();
                haloShader_->setUniformValue("color",
                                             color_.redF(),
                                             color_.greenF(),
                                             color_.blueF(),
                                             selected_ ? 0.25 : 0.05);

                glDisable(GL_DEPTH_TEST);
                glBegin(GL_TRIANGLE_FAN);
                Interface::color(color_, selected_ ? 0.45 : 0.15);

                glTexCoord2f(0.0, 0.0);

                this->vertex3(eye_);
                Interface::color(color_, 0.0);
                float _scale = size_ * 2.0;

                if (selected_) _scale *= 2.0;

                glTexCoord2f(0.0, 1.0);
                this->vertex3(topLeft_ * _scale);
                this->vertex3(topRight_ * _scale);
                this->vertex3(bottomRight_ * _scale);
                this->vertex3(bottomLeft_ * _scale);
                this->vertex3(topLeft_ * _scale);
                glEnd();
                glEnable(GL_DEPTH_TEST);

                haloShader_->release();
            }
            glPopMatrix();
        }
    }
}
