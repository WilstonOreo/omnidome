/* Copyright (c) 2014-2016 "Omnidome" by cr8tr
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

#include <omni/input/Framebuffer.h>

#include <omni/serialization/PropertyMap.h>
#include <omni/visual/util.h>

namespace omni {
  namespace input {
    Framebuffer::Framebuffer(input::Interface const* _input) :
      input::Interface(_input) {

    }

    void Framebuffer::destroy() {
      framebuffer_.reset();
    }

    GLuint Framebuffer::textureId() const
    {
      return !framebuffer_ ? 0 : framebuffer_->texture();
    }

    QSize Framebuffer::size() const {
      return !framebuffer_ ? QSize(0,0) : framebuffer_->size();
    }

    void Framebuffer::setSize(QSize const& _size)  {
      setupFramebuffer(_size);
    }

    QOpenGLFramebufferObject* Framebuffer::framebuffer() {
      return framebuffer_.get();
    }

    QOpenGLFramebufferObject const* Framebuffer::framebuffer() const {
      return framebuffer_.get();
    }

    void Framebuffer::setupFramebuffer(QSize const& _size) {
      if (_size == QSize(0,0)) {
        framebuffer_.reset();
        return;
      }

      bool _reset = !framebuffer_;
      if (framebuffer_) {
        _reset |= framebuffer_->size() != _size;
      }

      // Framebuffer is already setup, no work to do
      if (!_reset) return;

      visual::with_current_context([&](QOpenGLFunctions& _) {
        QOpenGLFramebufferObjectFormat _format;
        _format.setMipmap(false);
        _format.setSamples(0);
        _format.setTextureTarget(GL_TEXTURE_RECTANGLE);
        _format.setAttachment(QOpenGLFramebufferObject::NoAttachment);
        auto _size = size();
        framebuffer_.reset(new QOpenGLFramebufferObject(
              _size.width(),
              _size.height(),_format));
        _.glBindTexture(GL_TEXTURE_RECTANGLE, framebuffer_->texture());
        _.glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        _.glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        _.glBindTexture(GL_TEXTURE_RECTANGLE, 0);
      });
    }

    void Framebuffer::toStream(QDataStream& _os) const {
      input::Interface::toStream(_os);
      PropertyMap _map;
      _map("size",size());
      _os << _map;
    }

    void Framebuffer::fromStream(QDataStream& _is) {
      input::Interface::fromStream(_is);
      PropertyMap _map;
      _is >> _map;
      QSize _size = _map.getValue("size",QSize(0,0));
      setupFramebuffer(_size);
    }
  }
}
