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

#include "TestImage.h"

#include <omni/util.h>
#include <omni/visual/Shader.h>
#include <omni/visual/Rectangle.h>
#include <omni/visual/util.h>
#include "TestImageWidget.h"

namespace omni
{
  namespace input
  {
    ContextBoundPtr<QOpenGLTexture> TestImage::polarTex_;
    ContextBoundPtr<QOpenGLShaderProgram> TestImage::cubeMapShader_;
    ContextBoundPtr<QOpenGLShaderProgram> TestImage::sphericalShader_;

    TestImage::TestImage() :
      rulerPos_(0.0,0.0)
    {
    }

    void TestImage::update() {
      bool _allocate = !polarTex_ || !sphericalShader_ || !cubeMapShader_;

      if (_allocate) {
        primaryContextSwitch([&](QOpenGLFunctions& _) {
          if (!polarTex_) {
            QImage _image(QString(":/OMNISTAR.png"));
            polarTex_.reset(new QOpenGLTexture(_image.rgbSwapped().mirrored()));
            _.glFinish();
          }

          initShader(sphericalShader_,"SphericalTestImage");
          initShader(cubeMapShader_,"CubeMapTestImage");
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
        _.glDisable(GL_BLEND);

        if (mode_ == Mode::CUBEMAP) {
          visual::useShader(*cubeMapShader_,[&](visual::UniformHandler& _h) {
            _h.uniform("flip_text",flipText_);
            _h.uniform("display_numbers",displayNumbers_);

            visual::Rectangle::drawFlipped();
          });
        } else {
          visual::useShader(*sphericalShader_,[&](visual::UniformHandler& _h) {
            _h.uniform("ruler_pos",rulerPos_ - QPointF(0.5,0.5));
            _h.uniform("fisheye_mode",mode_ == Mode::FISHEYE);
            _h.texUniform("polar_tex",*polarTex_);
            visual::Rectangle::drawFlipped();
          });
        }
        _.glFinish();
      });
    }

    QSize TestImage::size() const
    {
      switch(mode_) {
        case Mode::FISHEYE: return QSize(2048,2048);
        case Mode::EQUIRECTANGULAR: return QSize(4096,2048);
        case Mode::CUBEMAP: return QSize(6144,1024);
      }
      return QSize(1024,1024);
    }

    QWidget* TestImage::widget() {
        return new omni::ui::input::TestImage(this);
    }

    void TestImage::setMode(Mode _mode) {
      mode_=_mode;
      update();
    }

    TestImage::Mode TestImage::mode() const {
      return mode_;
    }

    QPointF TestImage::rulerPos() const {
      return rulerPos_;
    }

    void TestImage::setRulerPos(QPointF const& _rulerPos) {
      rulerPos_ = _rulerPos;
    }

    void TestImage::setFlipText(bool _flipText) {
      flipText_ = _flipText;
    }

    bool TestImage::flipText() const {
      return flipText_;
    }

    void TestImage::setDisplayNumbers(bool _displayNumbers) {
      displayNumbers_ = _displayNumbers;
    }

    bool TestImage::displayNumbers() const {
      return displayNumbers_;
    }

    void TestImage::toPropertyMap(PropertyMap& _map) const
    {
      _map("mode",mode_);
      _map("rulerPos",rulerPos_);
      _map("flipText",flipText_);
      _map("displayNumbers",displayNumbers_);
      Framebuffer::toPropertyMap(_map);
    }

    void TestImage::fromPropertyMap(PropertyMap const& _map)
    {
      Framebuffer::fromPropertyMap(_map);
      _map.get("mode",mode_);
      _map.get("rulerPos",rulerPos_);
      _map.get("flipText",flipText_);
      _map.get("displayNumbers",displayNumbers_);
      update();
    }
  }
}
