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
    Syphon::Syphon()  : size_(0,0)
    {
    }

    Syphon::~Syphon()
    {
      NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

      [(SyphonNameboundClient*)client_ release];
      client_ = nil;

      [pool drain];
    }

    void Syphon::setDescription(SyphonServerDescription const& _description) {
      deactivate();
      description_ = _description;
      activate();
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

          if (_texSize.width * _texSize.height == 0) {
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

	     client_ = [[SyphonNameboundClient alloc] initWithHandler:^(SyphonClient* client) {
         this->update();
         this->triggerUpdateCallbacks();
       }];

        NSString *nsAppName =
        [NSString stringWithCString:description_.applicationNameAsConstChar()];

        qDebug() << Q_FUNC_INFO << description_.applicationNameAsConstChar() << description_.serverNameAsConstChar();

        NSString *nsServerName =
        [NSString stringWithCString:description_.serverNameAsConstChar()];

        [(SyphonNameboundClient*)client_ setAppName:nsAppName];
        [(SyphonNameboundClient*)client_ setName:nsServerName];

        [(SyphonNameboundClient*)client_ lockClient];

        isSetup_ = true;

        [pool drain];

        update();
        triggerUpdateCallbacks();
      }

      void Syphon::deactivate() {
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
          qDebug() << "syphonClient is not setup, or is not properly connected to server.  Cannot unbind.";
        }

        [pool drain];
      }

    QWidget* Syphon::widget() {
        return new omni::ui::input::Syphon(this);
    }

    GLuint Syphon::textureId() const {
      return texId_;
    }

    void Syphon::toPropertyMap(PropertyMap& _map) const
    {
      input::Interface::toPropertyMap(_map);
      _map("description",description_);
    }

    void Syphon::fromPropertyMap(PropertyMap const& _map)
    {
      deactivate();
      input::Interface::fromPropertyMap(_map);
      _map.get("description",description_);
      activate();
    }
  }
}
