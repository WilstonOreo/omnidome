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

#include <omni/ui/InputPreview.h>

#include <QDebug>
#include <QResizeEvent>
#include <QMatrix4x4>
#include <omni/visual/util.h>
#include <omni/visual/Shader.h>
#include <omni/visual/Rectangle.h>

namespace omni {
  namespace ui {
    InputPreview::InputPreview(QWidget *_parent) :
      GLView(_parent)
    {
      setUpdateFrequency(25.0);
    }

    InputPreview::InputPreview(input::Interface *_input, QWidget *_parent) :
      GLView(_parent),
      input_(_input) {
      input_->update();
      setUpdateFrequency(25.0);
    }

    InputPreview::~InputPreview()
    {}

    float InputPreview::border() const
    {
      return border_;
    }

    void InputPreview::setBorder(float _border)
    {
      border_ = _border;
      update();
    }

    input::Interface * InputPreview::input()
    {
      return input_;
    }

    void InputPreview::showEvent(QShowEvent*) {
      if (!input()) return;

      input_->update();
      update();
      makeCurrent();
    }

    input::Interface const * InputPreview::input() const
    {
      return input_;
    }

    void InputPreview::setInput(input::Interface *_input) {
      input_ = _input;
      makeCurrent();
    }

    bool InputPreview::initialize()
    {
      makeCurrent();
      return context() != nullptr;
    }

    QRectF InputPreview::viewRect() const
    {
      if (!input()) return QRectF(0.0, 0.0, 1.0, 1.0);

      return visual::util::viewRect(input()->width(), input()->height(),
                                    width(), height(), border_);
    }

    QPointF InputPreview::screenPos(QPointF const& _pos) const
    {
      QRectF && _rect = viewRect();
      QPointF _p = QPointF(_pos.x() / width(), _pos.y() / height());
      return QPointF(_p.x() * _rect.width(), -_p.y() * _rect.height());
    }

    void InputPreview::paint()
    {
      if (!input() || !isVisible()) return;

      makeCurrent();

      if (!shader_) {
        primaryContextSwitch([&](QOpenGLFunctions& _) {
          visual::initShader(shader_,"textureRect");
        });
      }

      withCurrentContext([this](QOpenGLFunctions& _)
      {
        _.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        auto _rect = viewRect();

        visual::viewport(this);

        /// Setup orthogonal projection
        glMatrixMode(GL_PROJECTION);
        {
          glLoadIdentity();
          QMatrix4x4 _m;
          _m.ortho(_rect.left(), _rect.right(), _rect.top(), _rect.bottom(), -1.0,
                   1.0);
          glMultMatrixf(_m.constData());
        }

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        shader_->bind();
        _.glBindTexture(GL_TEXTURE_RECTANGLE, input_->textureId());
        visual::Rectangle::draw(input_->size());
        _.glBindTexture(GL_TEXTURE_RECTANGLE, 0);
        shader_->release();
      });
    }
  }
}
