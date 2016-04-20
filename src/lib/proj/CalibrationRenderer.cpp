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

#include <omni/proj/CalibrationRenderer.h>

#include <omni/proj/Calibration.h>
#include <omni/proj/Tuning.h>

#include <omni/visual/UniformHandler.h>
#include <omni/visual/Tuning.h>
#include <omni/visual/Session.h>
#include <omni/visual/Rectangle.h>
#include <omni/visual/Framebuffer32F.h>
#include <omni/visual/ContextManager.h>
#include <omni/RenderBuffer.h>

namespace omni {
  namespace proj {
    CalibrationRenderer *CalibrationRenderer::instance_ = nullptr;
    std::unique_ptr<QOpenGLShaderProgram> CalibrationRenderer::shader_;

    CalibrationRenderer::CalibrationRenderer() {}

    void CalibrationRenderer::initialize(QOpenGLContext *_context) {
      if (!this->context_) {
        context_= _context;// QOpenGLContext::globalShareContext();
        OMNI_DEBUG << context_->isValid();
      }

      if (!this->surface_) {
        surface_.reset(new QOffscreenSurface());
        surface_->setFormat(this->context_->format());
        surface_->create();
      }

      if (!shader_) {
        this->context_->makeCurrent(surface_.get());
        visual::initShader(shader_, "texture");
        this->context_->doneCurrent();
      }
    }

    void CalibrationRenderer::render(Tuning const& _tuning, Calibration& _calib) {
      if (!isInitialized()) return;

      int _w = _calib.renderSize().width();
      int _h = _calib.renderSize().height();

      RenderBuffer _projBuffer(_w, _h);
      RenderBuffer _warpBuffer(_w, _h);
      RenderBuffer _blendBuffer(_w, _h);

      std::unique_ptr<visual::Framebuffer32F> _framebuffer;
      std::unique_ptr<visual::Framebuffer32F> _warpFramebuffer;

      visual::ContextManager::instance()->withPrimaryContext([&](QOpenGLFunctions& _) {
      {
        visual::resetOpenGLState();
        _framebuffer.reset(new visual::Framebuffer32F(QSize(_w, _h)));
        _warpFramebuffer.reset(new visual::Framebuffer32F(QSize(_w, _h)));

        visual::Session _sessionViz(_tuning.session());
        _sessionViz.update();

        visual::Tuning _tuningViz(const_cast<proj::Tuning&>(_tuning));
        _tuningViz.update();
        _tuningViz.updateWarpBuffer(&_sessionViz);
        _tuningViz.updateBlendTexture();

        /// 1st Step: Render projectors view to framebuffer texture
        visual::draw_on_framebuffer(_framebuffer, [&](QOpenGLFunctions& _) {
          // Projection operation
          glMultMatrixf(_tuning.projector().projectionMatrix().constData());
        }, [&](QOpenGLFunctions& _) {
          // Model view operation
          _sessionViz.drawCanvas(_tuning.session().exportSettings().
                                 mappingOutputMode());
          _.glReadPixels(0, 0, _w, _h, GL_RGBA, GL_FLOAT, _projBuffer.ptr());
        });

        visual::draw_on_framebuffer(_warpFramebuffer, [&](QOpenGLFunctions& _) {
          // Projection operation
          QMatrix4x4 _m;
          _m.ortho(-0.5, 0.5, -0.5, 0.5, -1.0, 1.0);
          glMultMatrixf(_m.constData());
        }, [&](QOpenGLFunctions& _) {
          // Model view operation

          // 3rd step: Render warp grid
          visual::useShader(*shader_, [&](visual::UniformHandler& _h) {
            _h.texUniform("tex", _framebuffer->texture());
            _tuningViz.drawWarpPatch();
          });
          _.glReadPixels(0, 0, _w, _h, GL_RGBA, GL_FLOAT, _warpBuffer.ptr());

          _.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
                    GL_STENCIL_BUFFER_BIT);
          // 4th Step: Render blend mask
          _tuningViz.drawOutput();
          _.glReadPixels(0, 0, _w, _h, GL_RGBA, GL_FLOAT, _blendBuffer.ptr());
        });

        _framebuffer.reset();
        _warpFramebuffer.reset();
      }
    });

      // 5th step: Merge blend and warp buffer
      for (int i = 0; i < _calib.buffer_.data().size(); ++i)
      {
        auto& _wP = _warpBuffer[i]; // Pixel from warp grid buffer
        _calib.buffer_[i] = RGBAFloat(_wP.r, _wP.g, _wP.b, _blendBuffer[i].r);
      }
    }

    bool CalibrationRenderer::isInitialized() const {
      return surface_ && context_;
    }

    CalibrationRenderer * CalibrationRenderer::instance() {
      if (!instance_) {
        instance_ = new CalibrationRenderer();
      }
      return instance_;
    }
  }
}
