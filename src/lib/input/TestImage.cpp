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
      return (!drawn_) ? 0 : framebuffer_->texture();
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
      if (!QOpenGLContext::currentContext()) return;

      drawn_ = false;

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
        _format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
        auto _size = size();
        framebuffer_.reset(new QOpenGLFramebufferObject(
              _size.width(),
              _size.height(),_format));



        visual::with_current_context([&](QOpenGLFunctions& _) {
          _.glEnable(GL_DEPTH_TEST);
          _.glDepthFunc(GL_LEQUAL);
          _.glEnable(GL_BLEND);
          _.glBindTexture(GL_TEXTURE_2D, framebuffer_->texture());
          _.glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
          _.glPixelStorei(GL_UNPACK_ROW_LENGTH,  0);
          _.glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
          _.glPixelStorei(GL_UNPACK_SKIP_ROWS,   0);
          glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
          glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
          glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
          glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
          _.glBindTexture(GL_TEXTURE_2D, 0);
        });
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

      drawn_ = true;
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
