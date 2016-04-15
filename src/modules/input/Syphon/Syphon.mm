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
      Interface(_parent),
      size_(0,0)
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
      return size_;
    }

    void Syphon::update() {
      if (!QOpenGLContext::currentContext()) return;
      if(isSetup_)
    {
      NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
      [(SyphonNameboundClient*)client_ lockClient];
          SyphonClient *client = [(SyphonNameboundClient*)client_ client];

          latestImage_ = [client newFrameImageForContext:CGLGetCurrentContext()];

		      NSSize _texSize = [(SyphonImage*)latestImage_ textureSize];
          if (_texSize.width == 0 || _texSize.height == 0) {
            [pool drain];
            texId_ = 0;
            size_ = QSize(0,0);
            return;
          }
		      GLuint _texId = [(SyphonImage*)latestImage_ textureName];
          texId_ = _texId;
          size_ = QSize(_texSize.width,_texSize.height);
        [pool drain];
      } else
        qDebug()<<"syphonClient is not setup, or is not properly connected to server.  Cannot bind.\n";
    }

    void Syphon::activate() {
      if (isSetup_) return;

      // Need pool
      NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

	     client_ = [[SyphonNameboundClient alloc] init];
	     isSetup_ = true;

        NSString *nsAppName =
        [NSString stringWithCString:description_.applicationNameAsConstChar() encoding:[NSString defaultCStringEncoding]];

        NSString *nsServerName =
        [NSString stringWithCString:description_.serverNameAsConstChar() encoding:[NSString defaultCStringEncoding]];

        [(SyphonNameboundClient*)client_ setAppName:nsAppName];
        [(SyphonNameboundClient*)client_ setName:nsServerName];

        [pool drain];

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
          [(SyphonNameboundClient*)client_ unlockClient];
          [(SyphonImage*)latestImage_ release];
          latestImage_ = nil;
          texId_ = 0;
          size_ = QSize(0,0);
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
      return texId_;
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
