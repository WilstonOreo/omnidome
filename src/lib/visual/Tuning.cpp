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

#include <omni/visual/Tuning.h>

#include <QVector4D>
#include <QOpenGLTexture>
#include <QOpenGLPixelTransferOptions>
#include <omni/util.h>
#include <omni/visual/util.h>
#include <omni/visual/Rectangle.h>
#include <omni/visual/Session.h>
#include <omni/visual/ContextManager.h>
#include <omni/visual/UniformHandler.h>

namespace omni {
  namespace visual {
    ContextBoundPtr<QOpenGLShaderProgram> Tuning::blendShader_;
    ContextBoundPtr<QOpenGLShaderProgram> Tuning::blendBrushShader_;
    ContextBoundPtr<QOpenGLShaderProgram> Tuning::blendBrushCursorShader_;
    ContextBoundPtr<QOpenGLShaderProgram> Tuning::testCardShader_;
    ContextBoundPtr<QOpenGLShaderProgram> Tuning::calibrationShader_;

    Tuning::Tuning(omni::proj::Tuning& _tuning) :
      tuning_(_tuning)
    {}

    Tuning::~Tuning() {}

    omni::proj::Tuning const& Tuning::tuning() const
    {
      return tuning_;
    }

    void Tuning::drawTestCard(int _id, bool _grayscale) const
    {
      if (!testCardShader_) return;

      if (!QOpenGLContext::currentContext()) return;

      auto _color    = tuning_.color();
      GLfloat _red   = _color.redF();
      GLfloat _green = _color.greenF();
      GLfloat _blue  = _color.blueF();
      testCardShader_->bind();
      testCardShader_->setUniformValue("resolution",
                                       GLfloat(tuning_.width()),
                                       GLfloat(tuning_.height()));
      testCardShader_->setUniformValue("test_color",   _red,
                                       _green,
                                       _blue);
      testCardShader_->setUniformValue("projector_id", _id);
      testCardShader_->setUniformValue("gray_output",  _grayscale);
      Rectangle::draw();
      testCardShader_->release();
    }

    void Tuning::drawWarpGrid() const
    {
      if (!warpGrid_) return;

      warpGrid_->drawLines();
      warpGrid_->drawHandles(tuning_.color(), tuningRect());
    }

    void Tuning::drawWarpPatch() const
    {
      warpGrid_->draw();
    }

    void Tuning::update()
    {
      if (!QOpenGLContext::currentContext()) return;

      using omni::util::fileToStr;

      initShader(testCardShader_, "testcard");
      initShader(blendShader_, "blend");
      initShader(blendBrushShader_, "blendbrush");
      initShader(blendBrushCursorShader_, "blendBrushCursor");
      initShader(calibrationShader_, "calibration");

      updateWarpGrid();
      updateBlendTexture();
    }

    void Tuning::setBlendTextureUpdateRect(QRect const& _rect)
    {
      int _radius = tuning_.blendMask().brushSize() / 2 + 1;

      blendTextureUpdateRect_ = _rect.normalized().adjusted(-_radius,
                                                            -_radius,
                                                            _radius,
                                                            _radius)
                                & QRect(0, 0,
                                        tuning_.width(), tuning_.height());
    }

    void Tuning::updateBlendTexture()
    {
      visual::with_current_context([&](QOpenGLFunctions& _)
      {
        if (!blendTex_)
        {
          // Reset blend texture
          blendTextureUpdateRect_ =
            QRect(0, 0, tuning_.width(), tuning_.height());
          blendTex_.reset(new QOpenGLTexture(tuning_.blendMask().
                                             strokeBuffer().toQImage()));
          blendTex_->bind();
          _.glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                            GL_CLAMP_TO_EDGE);
          _.glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                            GL_CLAMP_TO_EDGE);
          _.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                            GL_LINEAR);
          _.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                            GL_LINEAR);
          blendTex_->release();
        }
        auto& _blendMask = tuning().blendMask();
        auto _ptr = _blendMask.strokeBufferData();


        const int _resolution = BlendMask::resolution();
        auto _fullRect = QRect(0, 0, _resolution, _resolution);

        /// Transform tuning sized rect to stroke buffer sized rect
        auto _transformRect = [&](QRect const& _rect) -> QRect {
                                auto _res = BlendMask::resolution();
                                return QRect(
                                  _res * _rect.x() / tuning_.width(),
                                  _res * _rect.y() / tuning_.height(),
                                  _res * _rect.width() / tuning_.width(),
                                  _res * _rect.height() / tuning_.height());
                              };

        /// Optimization for uploading only a portion of the blend
        // buffer to texture
        if (blendTextureUpdateRect_ != _fullRect)
        {
          auto _r = _transformRect(blendTextureUpdateRect_);
          auto _data = _blendMask.strokeBuffer().cropRect(_r);
          _ptr = (void *)(_data.data().data());
          _.glBindTexture(GL_TEXTURE_2D, blendTex_->textureId());
          _.glPixelStorei(GL_UNPACK_ALIGNMENT,   1);
          _.glPixelStorei(GL_UNPACK_ROW_LENGTH,  0);
          _.glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
          _.glPixelStorei(GL_UNPACK_SKIP_ROWS,   0);
          _.glTexSubImage2D(GL_TEXTURE_2D, 0, _r.x(), _r.y(),
                            _r.width(), _r.height(), GL_RED,
                            GL_UNSIGNED_BYTE, _ptr);
          _.glBindTexture(GL_TEXTURE_2D, 0);
          blendTextureUpdateRect_ = _fullRect;
        } else
        {
          auto _r = _transformRect(_fullRect);
          _.glBindTexture(GL_TEXTURE_2D, blendTex_->textureId());
          _.glTexSubImage2D(GL_TEXTURE_2D, 0, _r.x(), _r.y(),
                            _r.width(), _r.height(), GL_RED,
                            GL_UNSIGNED_BYTE, _ptr);
          _.glBindTexture(GL_TEXTURE_2D, 0);
        }
      });
    }

    void Tuning::drawCursor(QPointF const& _pos)
    {
      auto _rect = tuningRect();

      visual::with_current_context([&](QOpenGLFunctions& _)
      {
        _.glEnable(GL_BLEND);

        useShader(*blendBrushCursorShader_, [&](UniformHandler& _h) {
          float _rX = tuning_.blendMask().brushSize() * 0.5 / tuning_.width();
          float _rY = _rX * (_rect.height() / _rect.width());
          _h.uniform("width", GLfloat(0.002 / _rX));
          _h.uniform("soft", GLfloat(0.0005 / _rX));
          Rectangle::draw(_pos.x() - _rX, _pos.x() + _rX, _pos.y() + _rY,
                          _pos.y() - _rY);
        });
      });
    }

    bool Tuning::initialized() const
    {
      return !!blendTex_ && !!blendShader_ && !!testCardShader_;
    }

    void Tuning::drawOutput(float         _inputOpacity,
                            QColor const& _color,
                            float         _blendMaskOpacity,
                            bool          _grayscale) const
    {
      if (!blendShader_) return;

      visual::with_current_context([&](QOpenGLFunctions& _) {
        _.glEnable(GL_DEPTH_TEST);
        _.glEnable(GL_BLEND);
        _.glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

        if (!warpGridBuffer_) return;

        GLuint _inputTexId = warpGridBuffer_->texture();

        auto& _mask = tuning().blendMask();

        useShader(*blendShader_, [&](UniformHandler& _h) {
          _h.uniform("top", _mask.topWidth());
          _h.uniform("right", _mask.rightWidth());
          _h.uniform("bottom", _mask.bottomWidth());
          _h.uniform("left", _mask.leftWidth());
          _h.uniform("edge_gamma",    _mask.gamma());
          _h.texUniform("input_tex",     _inputTexId);
          _h.uniform("input_opacity", _inputOpacity);
          _h.uniform("color",         _color.redF(),
                     _color.greenF(),
                     _color.blueF());
          _h.uniform("gray_output",   _grayscale);

          auto& _cC = tuning().colorCorrection();

          if (_cC.isUsed()) {
            _h.uniform("cc_gamma", QVector4D(
                         _cC.red().gamma(),
                         _cC.green().gamma(),
                         _cC.blue().gamma(),
                         _cC.all().gamma()));
            _h.uniform("cc_brightness", QVector4D(
                         _cC.red().brightness(),
                         _cC.green().brightness(),
                         _cC.blue().brightness(),
                         _cC.all().brightness()));
            _h.uniform("cc_contrast", QVector4D(
                         _cC.red().contrast(),
                         _cC.green().contrast(),
                         _cC.blue().contrast(),
                         _cC.all().contrast()));
            _h.uniform("cc_multiplier", QVector4D(
                         _cC.red().multiplier(),
                         _cC.green().multiplier(),
                         _cC.blue().multiplier(),
                         _cC.all().multiplier()));
          } else {
            QVector4D _null(0.0, 0.0, 0.0, 0.0);
            _h.uniform("cc_multiplier", _null);
            _h.uniform("cc_contrast",   _null);
            _h.uniform("cc_brightness", _null);
            _h.uniform("cc_gamma",      _null);
          }

          _h.uniform("mask",
                     GLfloat(_blendMaskOpacity));
          warpGrid_->draw();
        });

        if (_blendMaskOpacity > 0.0) {
          glColor4f(0.0, 0.0, 0.0, 1.0);
          float _b = 4.0;


          // Draw masks for borders
          glBegin(GL_QUADS);
          {
            glVertex2f(-0.5,      -0.5 - _b);
            glVertex2f(0.5 - _b,  -0.5);
            glVertex2f(0.5 - _b,  0.5);
            glVertex2f(-0.5,      0.5 + _b);
            glVertex2f(-0.5 + _b, -0.5 - _b);
            glVertex2f(0.5,       -0.5);
            glVertex2f(0.5,       0.5);
            glVertex2f(-0.5 + _b, 0.5 + _b);
            glVertex2f(-0.5 + _b, 0.5 + _b);
            glVertex2f(0.5 - _b,  0.5 + _b);
            glVertex2f(0.5 - _b,  0.5);
            glVertex2f(-0.5 + _b, 0.5);
            glVertex2f(-0.5 + _b, -0.5);
            glVertex2f(0.5 - _b,  -0.5);
            glVertex2f(0.5 - _b,  -0.5 - _b);
            glVertex2f(-0.5 + _b, -0.5 - _b);
          }
          glEnd();

          useShader(*blendBrushShader_, [&](UniformHandler& _h) {
            _h.texUniform("blend_tex", *blendTex_);
            Rectangle::drawFlipped();
          });
        }
      });
    }

    void Tuning::drawBlendMask() const {
      visual::with_current_context([&](QOpenGLFunctions& _) {
        _.glEnable(GL_DEPTH_TEST);
        _.glEnable(GL_BLEND);
        _.glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

        useShader(*blendShader_, [&](UniformHandler& _h) {
          auto& _mask = tuning().blendMask();
          _h.uniform("top", _mask.topWidth());
          _h.uniform("right", _mask.rightWidth());
          _h.uniform("bottom", _mask.bottomWidth());
          _h.uniform("left", _mask.leftWidth());
          _h.uniform("edge_gamma",    _mask.gamma());
          _h.uniform("mask",
                     GLfloat(1.0));
          warpGrid_->draw();
        });

        glColor4f(0.0, 0.0, 0.0, 1.0);
        float _b = 4.0;


        // Draw masks for borders
        glBegin(GL_QUADS);
        {
          glVertex2f(-0.5,      -0.5 - _b);
          glVertex2f(0.5 - _b,  -0.5);
          glVertex2f(0.5 - _b,  0.5);
          glVertex2f(-0.5,      0.5 + _b);
          glVertex2f(-0.5 + _b, -0.5 - _b);
          glVertex2f(0.5,       -0.5);
          glVertex2f(0.5,       0.5);
          glVertex2f(-0.5 + _b, 0.5 + _b);
          glVertex2f(-0.5 + _b, 0.5 + _b);
          glVertex2f(0.5 - _b,  0.5 + _b);
          glVertex2f(0.5 - _b,  0.5);
          glVertex2f(-0.5 + _b, 0.5);
          glVertex2f(-0.5 + _b, -0.5);
          glVertex2f(0.5 - _b,  -0.5);
          glVertex2f(0.5 - _b,  -0.5 - _b);
          glVertex2f(-0.5 + _b, -0.5 - _b);
        }
        glEnd();

        useShader(*blendBrushShader_, [&](UniformHandler& _h) {
          _h.texUniform("blend_tex", *blendTex_);
          Rectangle::drawFlipped();
        });
      });
    }

    void Tuning::updateWarpGrid() {
      if (!warpGrid_) {
        warpGrid_.reset(new visual::WarpGrid(tuning_.warpGrid()));
      }

      warpGrid_->update();
    }

    /// Update warp buffer which contains image of projector perspective
    void Tuning::updateWarpBuffer(visual::Session const *_vizSession)
    {
      with_current_context([&](QOpenGLFunctions& _) {
        // If tuning size has changed, reset warpGrid framebuffer
        if (warpGridBuffer_)
        {
          if ((warpGridBuffer_->width() != tuning().width()) ||
              (warpGridBuffer_->height() !=
               tuning().height())) warpGridBuffer_.reset();
        }

        // If warp grid framebuffer is empty, make a new one
        if (!warpGridBuffer_)
        {
          if (!warpGridBuffer_.reset(new QOpenGLFramebufferObject(
                                       tuning().width(),
                                       tuning().height()))) return;

          warpGridBuffer_->setAttachment(QOpenGLFramebufferObject::Depth);
        }

        // Draw projector's perspective on framebuffer texture
        draw_on_framebuffer(warpGridBuffer_.get(),
                            [&](QOpenGLFunctions& _) // Projection
                                                     // Operation
        {
          glMultMatrixf(tuning().projector().projectionMatrix().constData());
        },
                            [&](QOpenGLFunctions& _) // Model View
                                                     // Operation
        {
          _.glEnable(GL_DEPTH_TEST);
          _vizSession->drawCanvas();
        });
      });
    }

    void Tuning::generateCalibrationData() {
      calibration_.setRenderSize(QSize(tuning_.width(),
                                       tuning_.height()));
      tuning_.renderCalibration(calibration_);
      calibrationTex_.reset(new Texture32F(calibration_.buffer()));
    }

    QVector4D Tuning::channelCorrectionAsVec(Channel _channel) const {
      QVector4D _vec(0.0, 0.0, 0.0, 0.0);
      auto     *_channelCorrection = calibration_.colorCorrection().correction(
        _channel);

      if (!_channelCorrection) return _vec;

      _vec.setX(_channelCorrection->gamma());
      _vec.setY(_channelCorrection->brightness());
      _vec.setZ(_channelCorrection->contrast());
      _vec.setW(_channelCorrection->multiplier());
      return _vec;
    }

    void Tuning::drawCalibratedInput() {
      if (!calibrationShader_) return;

      auto *_currentInput = tuning().session().inputs().current();

      if (!_currentInput) return;

      with_current_context([&](QOpenGLFunctions& _) {
        if (!calibrationTex_) {
          generateCalibrationData();
        }

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        QMatrix4x4 _m;
        _m.ortho(-0.5, 0.5, -0.5, 0.5, -1.0, 1.0);
        glMultMatrixf(_m.constData());

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        useShader(*calibrationShader_, [&](UniformHandler& _h) {
          _h.texUniform("image", _currentInput->textureId(),
                        GL_TEXTURE_RECTANGLE);
          _h.texUniform("uv_map", *calibrationTex_);
          _h.uniform("image_size",
                     QVector2D(_currentInput->width(),
                               _currentInput->height()));
          _h.uniform("cc_red_vec",
                     channelCorrectionAsVec(
                       Channel::RED));
          _h.uniform("cc_green_vec",
                     channelCorrectionAsVec(Channel::
                                            GREEN));
          _h.uniform("cc_blue_vec",
                     channelCorrectionAsVec(
                       Channel::BLUE));
          _h.uniform("cc_all_vec",
                     channelCorrectionAsVec(
                       Channel::ALL));

          Rectangle::draw(-0.5, 0.5, 0.5, -0.5);
        });
      });
    }

    QRectF Tuning::tuningRect() const
    {
      float _projAspect = float(tuning_.width()) / tuning_.height();
      float _left       = -0.5, _right = 0.5, _bottom = -0.5, _top = 0.5;

      if (_projAspect > 1.0)
      {
        _top    *= _projAspect;
        _bottom *=  _projAspect;
      }
      else
      {
        _left  /= _projAspect;
        _right /= _projAspect;
      }
      return QRectF(QPointF(_left, _top), QPointF(_right, _bottom));
    }
  }
}
