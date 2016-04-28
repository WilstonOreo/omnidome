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

#include <omni/input/TestImage.h>

#include <QOpenGLContext>
#include <QOpenGLFramebufferObjectFormat>
#include <omni/visual/util.h>
#include <omni/visual/Rectangle.h>
#include <omni/util.h>

namespace omni
{
  namespace input
  {
    TestImage::TestImage(Interface const* _parent) :
      Framebuffer(_parent),
      rulerPos_(-1.0,-1.0)
    {
    }

    void TestImage::destroy()
    {
      Framebuffer::destroy();
      shader_.reset();
    }

    void TestImage::setRulerPos(QPointF const& _rulerPos)
    {
      rulerPos_ = _rulerPos;
    }

    QPointF TestImage::rulerPos() const
    {
      return rulerPos_;
    }

    void TestImage::shaderUniformHandler()
    {
      shader_->setUniformValue("ruler_pos",rulerPos_ - QPointF(0.5,0.5));
    }

    void TestImage::update()
    {
      if (!shader_)
      {
        primaryContextSwitch([&](QOpenGLFunctions& _) {
          QString _vertSrc = this->vertexShaderSource();
          QString _fragmentSrc = this->fragmentShaderSource();
          if (!shader_.reset(new QOpenGLShaderProgram())) return;
          shader_->addShaderFromSourceCode(QOpenGLShader::Vertex,_vertSrc);
          shader_->addShaderFromSourceCode(QOpenGLShader::Fragment,_fragmentSrc);
          shader_->link();
        });
      }

      setupFramebuffer(size());

      visual::draw_on_framebuffer(framebuffer(),
      [&](QOpenGLFunctions& _) // Projection Operation
      {
        QMatrix4x4 _m;
        _m.ortho(-0.5,0.5,-0.5,0.5,-1.0,1.0);
        glMultMatrixf(_m.constData());
      },
      [&](QOpenGLFunctions& _) // Model View Operation
      {
        shader_->bind();
        {
          shaderUniformHandler();
          visual::Rectangle::drawFlipped();
        }
        shader_->release();
      });
    }

    void TestImage::toPropertyMap(PropertyMap& _map) const
    {
      Framebuffer::toPropertyMap(_map);
      _map("rulerPos",rulerPos_);
    }

    void TestImage::fromPropertyMap(PropertyMap const& _map)
    {
      Framebuffer::fromPropertyMap(_map);
      _map.get("rulerPos",rulerPos_);
      update();
    }
  }
}
