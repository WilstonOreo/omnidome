/* Copyright (c) 2014-2016 "Omnidome" by Michael Winkelmann
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

#ifndef OMNI_INPUT_FRAMEBUFFER_H_
#define OMNI_INPUT_FRAMEBUFFER_H_

#include <QOpenGLFramebufferObject>
#include <omni/input/Interface.h>
#include <omni/visual/ContextBoundPtr.h>

namespace omni {
  namespace input {
    /// Input object that is renderer to a framebuffer
    class OMNI_EXPORT Framebuffer : public input::Interface {
    public:
      Framebuffer();
      virtual ~Framebuffer() override {}

      virtual QSize size() const override;
      virtual void setSize(QSize const&);

      GLuint  textureId() const override;

      virtual void destroy();

      virtual void toPropertyMap(PropertyMap&) const override;
      virtual void fromPropertyMap(PropertyMap const&) override;

    protected:
      QOpenGLFramebufferObject* framebuffer();
      QOpenGLFramebufferObject const* framebuffer() const;

      void setupFramebuffer(QSize const& _size);
      void setupFramebuffer();

    private:
      QSize size_;
      ContextBoundPtr<QOpenGLFramebufferObject> framebuffer_;
    };
  }
}

#endif /* OMNI_INPUT_FRAMEBUFFER_H_ */
