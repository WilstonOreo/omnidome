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

#include "Syphon.h"

#include <QDebug>
#include <omni/util.h>
#include <omni/visual/util.h>
#include "SyphonWidget.h"


#import <Syphon/Syphon.h>
#import "SyphonNameboundClient.h"


namespace omni
{
  namespace input
  {
    Syphon::Syphon(Interface const* _parent)  :
      Interface(_parent)
    {
    }

    Syphon::~Syphon()
    {
      NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

      [(SyphonNameboundClient*)client_ release];
      client_ = nil;

      [pool drain];
    }

    void Syphon::timerEvent(QTimerEvent *) {
      this->update();

      if (this->updatedCallback()) {
        this->updatedCallback()();
      }
    }

    void Syphon::setDescription(SyphonServerDescription const& _description) {
      int _oldTimerId = timerId_;
      if (timerId_) deactivate();
      description_ = _description;

      if (_oldTimerId) {
        activate();
      }
    }

    SyphonServerDescription const& Syphon::description() const {
      return description_;
    }

    QSize Syphon::size() const
    {
      return (!framebuffer_) ? QSize(0,0) : framebuffer_->size();
    }

    void Syphon::update() {
      if (!QOpenGLContext::currentContext()) return;
      NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
      if(isSetup_)

    {
      [(SyphonNameboundClient*)client_ lockClient];
          SyphonClient *client = [(SyphonNameboundClient*)client_ client];

          latestImage_ = [client newFrameImageForContext:CGLGetCurrentContext()];

		      NSSize _texSize = [(SyphonImage*)latestImage_ textureSize];
          if (_texSize.width == 0 || _texSize.height == 0) {
            [pool drain];
            texId_ = 0;
            return;
          }
		      GLuint _texId = [(SyphonImage*)latestImage_ textureName];
          texId_ = _texId;

          if ( (!framebuffer_) || (_texSize.width != width()) ||
               (_texSize.height != height())) {
            QOpenGLFramebufferObjectFormat _format;
            framebuffer_.reset(new QOpenGLFramebufferObject(_texSize.width,_texSize.height,_format));
            framebuffer_->setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);

            glBindTexture(GL_TEXTURE_2D, framebuffer_->texture());
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glBindTexture(GL_TEXTURE_2D, 0);
          }

        GLfloat _w = width();
        GLfloat _h = height();

        visual::util::draw_on_framebuffer(framebuffer_,[&](QOpenGLFunctions& _){
          QMatrix4x4 _m;
          _m.ortho(0.0, _w, 0.0, _h, -1.0, 1.0);
          glMultMatrixf(_m.constData());

        }, [&](QOpenGLFunctions& _) {

        glEnable(GL_TEXTURE_RECTANGLE);
        glBindTexture(GL_TEXTURE_RECTANGLE,texId_);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glTexParameterf( GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameterf( GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        glTexParameterf( GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );

        glBegin(GL_QUADS);
        {
          glColor4f(1.0,1.0,1.0,1.0);
          glTexCoord2f(0.0f, 0.0f);
          glVertex2f(0.0f, 0.0);
          glTexCoord2f(_w, 0.0f);
          glVertex2f(_w, 0.0f);
          glTexCoord2f(_w, _h);
          glVertex2f(_w, _h);
          glTexCoord2f(0.0f, _h);
          glVertex2f(0.0f, _h);
        }
        glEnd();
        glBindTexture(GL_TEXTURE_RECTANGLE,0);
        glDisable(GL_TEXTURE_RECTANGLE);

        });
      } else
        qDebug()<<"syphonClient is not setup, or is not properly connected to server.  Cannot bind.\n";

      [pool drain];
    }

    void Syphon::activate() {
      if (isSetup_) return;

      // Need pool
      NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

	     client_ = [[SyphonNameboundClient alloc] init];
	     isSetup_ = true;

       [pool drain];

       if(isSetup_)
       {
         NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

         NSString *nsAppName =
          [NSString stringWithCString:description_.applicationNameAsConstChar() encoding:[NSString defaultCStringEncoding]];

          NSString *nsServerName =
          [NSString stringWithCString:description_.serverNameAsConstChar() encoding:[NSString defaultCStringEncoding]];

          [(SyphonNameboundClient*)client_ setAppName:nsAppName];
          [(SyphonNameboundClient*)client_ setName:nsServerName];

          [pool drain];
        }

        int _timerId = this->startTimer(20);
        if (!timerId_) {
          timerId_ = _timerId;
        }
      }

      void Syphon::deactivate() {
         this->killTimer(timerId_);
         timerId_ = 0;

        NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
        if(isSetup_)
        {
          framebuffer_.reset();
          [(SyphonNameboundClient*)client_ unlockClient];
          [(SyphonImage*)latestImage_ release];
          latestImage_ = nil;
          texId_ = 0;
          isSetup_ = false;
        } else
        {
          qDebug() << "syphonClient is not setup, or is not properly connected to server.  Cannot unbind.\n";
        }
	       [pool drain];

      }

    QWidget* Syphon::widget() {
        return new omni::ui::input::Syphon(this);
    }

    GLuint Syphon::textureId() const {
      return (!framebuffer_) ? 0 : framebuffer_->texture();
    }

    void Syphon::toStream(QDataStream& _stream) const
    {
      input::Interface::toStream(_stream);
      _stream << description_;
    }

    void Syphon::fromStream(QDataStream& _stream)
    {
      int _oldTimerId = timerId_;
      if (timerId_) {
        deactivate();
      }
      input::Interface::fromStream(_stream);
      _stream >> description_;
      if (_oldTimerId) {
        activate();
      }
    }
  }
}
