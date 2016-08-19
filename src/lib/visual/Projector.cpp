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

#include <omni/visual/Projector.h>

#include <array>
#include <omni/util.h>
#include <omni/visual/Shader.h>
#include <omni/proj/Frustum.h>

namespace omni {
    namespace visual {
        ContextBoundPtr<QOpenGLShaderProgram> Projector::haloShader_;
        ContextBoundPtr<QOpenGLShaderProgram> Projector::wireframeShader_;

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
          proj::Frustum _frustum(proj_);

          eye_         = QVector3D(0, 0, 0);
          topLeft_     = _frustum.topLeft() *size_;
          topRight_    = _frustum.topRight() *size_;
          bottomLeft_  = _frustum.bottomLeft() *size_;
          bottomRight_ = _frustum.bottomRight() * size_;

          withCurrentContext([&](QOpenGLFunctions& _) {
            initShader(haloShader_,"halo");
            initShader(wireframeShader_,"wireframe");

            frustumVbo_.reset(new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer));
            frustumVbo_->create();
            frustumVbo_->setUsagePattern(QOpenGLBuffer::StaticDraw);

            std::array<QVector3D,16> _vertices {
              eye_, topLeft_,
              eye_, topRight_,
              eye_, bottomLeft_,
              eye_, bottomRight_,
              topLeft_, topRight_,
              topLeft_, bottomLeft_,
              topRight_, bottomRight_,
              bottomLeft_, bottomRight_
            };

            frustumVbo_->bind();

            frustumVbo_->allocate(_vertices.data(), _vertices.size() * sizeof(QVector3D));
            frustumVbo_->write(0,_vertices.data(),_vertices.size()* sizeof(QVector3D));
            frustumVbo_->release();

            haloVbo_.reset(new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer));
          });
        }

        void Projector::draw() const
        {
            Interface::color(color_);

//            const_cast<Projector*>(this)->update();

            withCurrentContext([this](QOpenGLFunctions& _)
            {
                _.glLineWidth(selected_ ? 2.0 : 1.0);

                useShader(*wireframeShader_,[&](UniformHandler& h) {
                  frustumVbo_->bind();
                  {
                    wireframeShader_->enableAttributeArray("vertex");
                    wireframeShader_->setAttributeBuffer("vertex", GL_FLOAT, 0, 3);

                    h.uniform("color",QVector4D(color_.redF(),color_.greenF(),color_.blueF(),1.0f));
                    h.uniform("matrix",proj_.matrix());
                    glDrawArrays(GL_LINES,0,16);

                    wireframeShader_->disableAttributeArray("vertex");
                  }
                  frustumVbo_->release();
                });
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
            auto _setupId =
                proj_.setup() ? proj_.setup()->getTypeId().str() :
                QStringLiteral("PeripheralSetup");

            if (_setupId == QStringLiteral("PeripheralSetup"))
            {
                // Draw line from center to projector ground position
                this->visualLine(_center, QVector3D(_p.x(), _p.y(), _center.z()));
                this->visualLine(
                    QVector3D(_p.x(), _p.y(), _center.z()), _p);
            } else
            {
                // Draw manhattan line from center to projector ground position
                this->visualLine(_center,
                                 QVector3D(_p.x(), _center.y(), _center.z()));
                this->visualLine(
                                 QVector3D(_p.x(), _center.y(), _center.z()),
                                 QVector3D(_p.x(), _p.y(),      _center.z()));
                this->visualLine(
                                 QVector3D(_p.x(), _p.y(),      _center.z()), _p);
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
