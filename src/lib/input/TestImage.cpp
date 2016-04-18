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
#include <omni/util.h>

namespace omni
{
  namespace input
  {
    TestImage::TestImage(Interface const* _parent) :
      Interface(_parent),
      rulerPos_(-1.0,-1.0)
    {
    }

    TestImage::~TestImage()
    {
      free();
    }

    GLuint TestImage::textureId() const
    {
      return !framebuffer_ ? 0 : framebuffer_->texture();
    }

    void TestImage::free()
    {
      shader_.reset();
      framebuffer_.reset();
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
        QString _vertSrc = this->vertexShaderSource();
        QString _fragmentSrc = this->fragmentShaderSource();
        shader_.reset(new QOpenGLShaderProgram());
        shader_->addShaderFromSourceCode(QOpenGLShader::Vertex,_vertSrc);
        shader_->addShaderFromSourceCode(QOpenGLShader::Fragment,_fragmentSrc);
        shader_->link();
      }

      if (!framebuffer_)
      {
        QOpenGLFramebufferObjectFormat _format;
        _format.setMipmap(false);
        _format.setSamples(0);
        _format.setTextureTarget(GL_TEXTURE_RECTANGLE);
        _format.setAttachment(QOpenGLFramebufferObject::NoAttachment);
        auto _size = size();
        framebuffer_.reset(new QOpenGLFramebufferObject(
              _size.width(),
              _size.height(),_format));
      }

      visual::draw_on_framebuffer(framebuffer_,
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
          glBegin(GL_QUADS);
          {
            glTexCoord2f(0.0f,0.0f);
            glVertex2f(-0.5f,-0.5f);
            glTexCoord2f(1.0f,0.0f);
            glVertex2f(0.5f,-0.5f);
            glTexCoord2f(1.0f,1.0f);
            glVertex2f(0.5f,0.5f);
            glTexCoord2f(0.0f,1.0f);
            glVertex2f(-0.5f,0.5f);
          }
          glEnd();
        }
        shader_->release();
      });
    }

    void TestImage::toStream(QDataStream& _stream) const
    {
      input::Interface::toStream(_stream);
      _stream << rulerPos_;
    }

    void TestImage::fromStream(QDataStream& _stream)
    {
      input::Interface::fromStream(_stream);
      _stream >> rulerPos_;
      update();
    }
  }
}
