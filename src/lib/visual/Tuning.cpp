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

namespace omni {
  namespace visual {
    std::unique_ptr<QOpenGLShaderProgram> Tuning::blendShader_;
    std::unique_ptr<QOpenGLShaderProgram> Tuning::blendBrushShader_;
    std::unique_ptr<QOpenGLShaderProgram> Tuning::testCardShader_;
    std::unique_ptr<QOpenGLShaderProgram> Tuning::calibrationShader_;

    Tuning::Tuning(omni::proj::Tuning& _tuning) :
      tuning_(_tuning)
    {
      warpGrid_.reset(new visual::WarpGrid(tuning_.warpGrid()));
    }

    omni::proj::Tuning const& Tuning::tuning() const
    {
      return tuning_;
    }

    void Tuning::drawTestCard(int _id, bool _grayscale) const
    {
      if (!testCardShader_) return;

      if (!QOpenGLContext::currentContext()) return;

      visual::with_current_context([&](QOpenGLFunctions& _)
      {
        auto _color = tuning_.color();
        GLfloat _red = _color.redF();
        GLfloat _green = _color.greenF();
        GLfloat _blue = _color.blueF();
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
      });
    }

    void Tuning::drawWarpGrid() const
    {
      if (!warpGrid_) return;

      visual::with_current_context([this](QOpenGLFunctions& _)
      {
        warpGrid_->drawLines();
        warpGrid_->drawHandles(tuning_.color(), tuningRect());
      });
    }

    void Tuning::drawWarpPatch() const
    {
      visual::with_current_context([this](QOpenGLFunctions& _)
      {
        _.glEnable(GL_TEXTURE_2D);
        _.glDisable(GL_BLEND);
        glColor4f(1.0, 1.0, 1.0, 1.0);
        warpGrid_->draw();
      });
    }

    void Tuning::update()
    {
      if (!QOpenGLContext::currentContext()) return;

      using omni::util::fileToStr;

      if (!cursor_) {
        cursor_.reset(new Circle);
      }

      auto _initShader = [&](
        std::unique_ptr<QOpenGLShaderProgram>& _s,
        QString const& _filename) {
                           if (!!_s) return;

                           QString _vertSrc =
                             fileToStr(":/shaders/" + _filename + ".vert");
                           QString _fragmentSrc =
                             fileToStr(":/shaders/" + _filename + ".frag");
                           _s.reset(new QOpenGLShaderProgram());
                           _s->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                                       _vertSrc);
                           _s->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                                       _fragmentSrc);
                           _s->link();
                         };

      _initShader(testCardShader_, "testcard");
      _initShader(blendShader_, "blend");
      _initShader(blendBrushShader_, "blendbrush");
      _initShader(calibrationShader_, "calibration");

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
      if (!QOpenGLContext::currentContext()) return;

      visual::with_current_context([&](QOpenGLFunctions& _)
      {
        bool _reset = !blendTex_;

        if (!!blendTex_)
        {
          // Reset blend texture if its size has changed
          _reset = blendTex_->width() != tuning_.width() || blendTex_->height() != tuning_.height();
        }

        if (_reset)
        {
          // Reset blend texture
          blendTextureUpdateRect_ =
            QRect(0, 0, tuning_.width(), tuning_.height());
          blendTex_.reset(new QOpenGLTexture(tuning_.blendMask().
                                             strokeBuffer().toQImage()));

            blendTex_->bind();
            glTexEnvf(GL_TEXTURE_ENV,
                      GL_TEXTURE_ENV_MODE,
                      GL_REPLACE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                            GL_CLAMP_TO_EDGE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                            GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                            GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                            GL_LINEAR);
            blendTex_->release();
        }
        auto& _blendMask = tuning().blendMask();
        auto _ptr = _blendMask.strokeBufferData();

        auto _fullRect = QRect(0, 0, tuning_.width(), tuning_.height());


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
      if (!cursor_) return;

      auto _rect = tuningRect();

      visual::with_current_context([&](QOpenGLFunctions& _)
      {
        _.glDisable(GL_LINE_SMOOTH);
        _.glEnable(GL_BLEND);

        glColor4f(0.0, 0.0, 0.0, 1.0);
        _.glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR);

        float _r = tuning_.blendMask().brushSize() * 0.5 / tuning_.width();
        cursor_->drawLine(_pos, _r, _r * (_rect.height() / _rect.width()));
        _.glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        _.glEnable(GL_LINE_SMOOTH);
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
      if (!blendShader_ || !warpGridBuffer_) return;

      GLuint _inputTexId = warpGridBuffer_->texture();

      visual::with_current_context([&](QOpenGLFunctions& _)
      {
        warpGrid_->draw();

        auto& _mask = tuning().blendMask();
        glBlendColor(1.0, 1.0, 1.0, 1.0);
        _.glEnable(GL_DEPTH_TEST);
        _.glDepthFunc(GL_LEQUAL);
        _.glEnable(GL_BLEND);
        _.glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

        blendShader_->bind();
        {
          blendShader_->setUniformValue("top",
                                        _mask.topWidth());
          blendShader_->setUniformValue("right",
                                        _mask.
                                        rightWidth());
          blendShader_->setUniformValue("bottom",
                                        _mask.
                                        bottomWidth());
          blendShader_->setUniformValue("left",
                                        _mask.leftWidth());
          blendShader_->setUniformValue("edge_gamma",    _mask.gamma());
          blendShader_->setUniformValue("input_tex",     0);
          _.glActiveTexture(GL_TEXTURE0 + 0);
          _.glBindTexture(GL_TEXTURE_2D, _inputTexId);
          blendShader_->setUniformValue("input_opacity", _inputOpacity);
          blendShader_->setUniformValue("color",         _color.redF(),
                                        _color.greenF(),
                                        _color.blueF());
          blendShader_->setUniformValue("gray_output",   _grayscale);

          auto& _cC = tuning().colorCorrection();

          if (_cC.isUsed()) {
            blendShader_->setUniformValue("cc_gamma", QVector4D(
                                            _cC.red().gamma(),
                                            _cC.green().gamma(),
                                            _cC.blue().gamma(),
                                            _cC.all().gamma()));
            blendShader_->setUniformValue("cc_brightness", QVector4D(
                                            _cC.red().brightness(),
                                            _cC.green().brightness(),
                                            _cC.blue().brightness(),
                                            _cC.all().brightness()));
            blendShader_->setUniformValue("cc_contrast", QVector4D(
                                            _cC.red().contrast(),
                                            _cC.green().contrast(),
                                            _cC.blue().contrast(),
                                            _cC.all().contrast()));
            blendShader_->setUniformValue("cc_multiplier", QVector4D(
                                            _cC.red().multiplier(),
                                            _cC.green().multiplier(),
                                            _cC.blue().multiplier(),
                                            _cC.all().multiplier()));
          } else {
            QVector4D _null(0.0, 0.0, 0.0, 0.0);
            blendShader_->setUniformValue("cc_multiplier", _null);
            blendShader_->setUniformValue("cc_contrast",   _null);
            blendShader_->setUniformValue("cc_brightness", _null);
            blendShader_->setUniformValue("cc_gamma",      _null);
          }

          blendShader_->setUniformValue("mask",
                                        GLfloat(_blendMaskOpacity));
          warpGrid_->draw();
        }
        _.glBindTexture(GL_TEXTURE_2D, 0);
        blendShader_->release();

        if (_blendMaskOpacity > 0.0) {
          _.glDisable(GL_BLEND);
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

          _.glEnable(GL_BLEND);
          _.glEnable(GL_TEXTURE_2D);
          _.glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
          blendBrushShader_->bind();
          {
            blendBrushShader_->setUniformValue("blend_tex",1);
            blendTex_->bind(1);
            Rectangle::drawFlipped();
          }
          blendTex_->release(1);
          blendBrushShader_->release();
          _.glDisable(GL_TEXTURE_2D);
        }
      });
    }

    void Tuning::updateWarpGrid() {
      if (!warpGrid_) return;

      warpGrid_->update();
    }

    /// Update warp buffer which contains image of projector perspective
    void Tuning::updateWarpBuffer(visual::Session const *_vizSession)
    {
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
        warpGridBuffer_.reset(new QOpenGLFramebufferObject(
                                tuning().width(),
                                tuning().height()));
        warpGridBuffer_->setAttachment(QOpenGLFramebufferObject::Depth);
      }

      // Draw projector's perspective on framebuffer texture
      with_current_context([&](QOpenGLFunctions& _)
      {
        draw_on_framebuffer(warpGridBuffer_,
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

    void Tuning::free()
    {
      if (!!blendTex_)
      {
        blendTex_->destroy();
        blendTex_.reset();
      }

      blendShader_.reset();
      testCardShader_.reset();
    }

    void Tuning::generateCalibrationData() {
      tuning_.renderCalibration(calibration_);

      with_current_context([&](QOpenGLFunctions& _) {
        if (calibrationTexId_ != 0) {
          _.glDeleteTextures(1, &calibrationTexId_);
        }

        _.glGenTextures(1, &calibrationTexId_);
        _.glBindTexture(GL_TEXTURE_2D, calibrationTexId_);
        _.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
        _.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
        _.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        _.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        _.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F,
                       calibration_.renderSize().width(),
                       calibration_.renderSize().height(), 0,
                       GL_RGBA, GL_FLOAT, calibration_.buffer().ptr());
        _.glBindTexture(GL_TEXTURE_2D, 0);
      });
    }

    QVector4D Tuning::channelCorrectionAsVec(Channel _channel) const {
      QVector4D _vec(0.0, 0.0, 0.0, 0.0);
      auto*_channelCorrection = calibration_.colorCorrection().correction(
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
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        QMatrix4x4 _m;
        _m.ortho(-0.5, 0.5, -0.5, 0.5, -1.0, 1.0);
        glMultMatrixf(_m.constData());

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        calibrationShader_->bind();
        {
          calibrationShader_->setUniformValue("uv_map", GLint(1));
          _.glActiveTexture(GL_TEXTURE0 + 1);
          _.glBindTexture(GL_TEXTURE_2D, calibrationTexId_);

          calibrationShader_->setUniformValue("image", GLint(2));
          _.glActiveTexture(GL_TEXTURE0 + 2);
          _.glBindTexture(GL_TEXTURE_2D, _currentInput->textureId());

          calibrationShader_->setUniformValue("cc_red_vec",
                                              channelCorrectionAsVec(
                                                Channel::RED));
          calibrationShader_->setUniformValue("cc_green_vec",
                                              channelCorrectionAsVec(Channel::
                                                                     GREEN));
          calibrationShader_->setUniformValue("cc_blue_vec",
                                              channelCorrectionAsVec(
                                                Channel::BLUE));
          calibrationShader_->setUniformValue("cc_all_vec",
                                              channelCorrectionAsVec(
                                                Channel::ALL));

          Rectangle::draw(-0.5, 0.5, 0.5, -0.5);
        }
        calibrationShader_->release();
        _.glBindTexture(GL_TEXTURE_2D, 0);
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
