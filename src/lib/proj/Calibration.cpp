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

#include <omni/proj/Tuning.h>
#include <omni/proj/Calibration.h>

#include <omni/visual/Session.h>
#include <omni/visual/Tuning.h>

namespace omni {
  namespace proj {
    Calibration::Calibration(CalibrationMode _mode) {}

    Calibration::Calibration(Tuning const& _tuning, CalibrationMode _mode) :
      mode_(_mode) {
        render(_tuning,mode_);
      }

    void Calibration::render(Tuning const& _tuning) {
      int _w = buffer_.width() <= 0 ? _tuning.width() : buffer_.width();
      int _h = buffer_.height() <= 0 ? _tuning.height() : buffer_.height();

      colorCorrection_ = _tuning.colorCorrection();
      screenGeometry_ = _tuning.screenGeometry();
      contentGeometry_ = _tuning.contentGeometry();

      /// 1st Step: Render projectors view to texture
      RenderBuffer _projBuffer(_w, _h);
      visual::Session _sessionViz(_tuning.session());

      renderToBuffer(_projBuffer,
        // Projection operation
                     [&](QOpenGLFunctions& _)
      {
        glMultMatrixf(_tuning.projector().projectionMatrix().constData());
      },

      // Model view operation
                     [&](QOpenGLFunctions& _)
      {
        // Draw canvas with
        _sessionViz.update();
        _sessionViz.drawCanvas(_tuning.session().exportSettings().mappingOutputMode());
      }
        );

      GLuint _projTex = 0;

      // 2nd step: Update render buffer as floating point texture
      visual::with_current_context([&](QOpenGLFunctions& _)
      {
        _.glGenTextures(1, &_projTex);
        _.glBindTexture(GL_TEXTURE_2D, _projTex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        _.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F,
                       _projBuffer.width(), _projBuffer.height(), 0,
                       GL_RGBA, GL_FLOAT, _projBuffer.ptr());
        _.glBindTexture(GL_TEXTURE_2D, 0);
      });

      // 3rd step: Render warp grid
      RenderBuffer _warpBuffer(_w, _h);

      visual::Tuning _tuningViz(const_cast<proj::Tuning&>(_tuning));
      _tuningViz.update();
      _tuningViz.updateWarpGrid();
      _tuningViz.updateWarpBuffer(&_sessionViz);
      _tuningViz.updateBlendTexture();

      renderToBuffer(_warpBuffer,
        // Projection operation
                     [&](QOpenGLFunctions& _)
      {
        QMatrix4x4 _m;
        _m.ortho(-0.5, 0.5, -0.5, 0.5, -1.0, 1.0);
        glMultMatrixf(_m.constData());
      },

      // Model view operation
                     [&](QOpenGLFunctions& _)
      {
        _.glBindTexture(GL_TEXTURE_2D, _projTex);
        _tuningViz.drawWarpPatch();
        _.glBindTexture(GL_TEXTURE_2D, 0);
      });

      // 4th Step: Render blend mask
      RenderBuffer _blendBuffer(_w, _h);
      renderToBuffer(_blendBuffer,

        // Projection operation
                     [&](QOpenGLFunctions& _)
      {
        QMatrix4x4 _m;
        _m.ortho(-0.5, 0.5, -0.5, 0.5, -1.0, 1.0);
        glMultMatrixf(_m.constData());
      },

      // Model view operation
                     [&](QOpenGLFunctions& _)
      {
        _.glDisable(GL_TEXTURE_2D);
        _tuningViz.drawOutput();
      });

      // 5th step: Merge blend and warp buffer
      buffer_.resize(_w, _h);
      for (int i = 0; i < buffer_.data().size(); ++i)
      {
        auto& _wP = _warpBuffer[i]; // Pixel from warp grid buffer
        buffer_[i] = RGBAFloat(_wP.r, _wP.g, _wP.b, _blendBuffer[i].r);
      }
    }

    void Calibration::render(Tuning const& _tuning, CalibrationMode _mode) {
      mode_=_mode;
//      outputGeometry_ = _tuning.rectangle
      render(_tuning);
    }

    QImage Calibration::toImage() const {
      QImage _image(buffer_.width(), buffer_.height(), QImage::Format_ARGB32);
/*
      int _pos = 0;

      for (int y = 0; y < _image.height(); ++y)
      {
        uchar *_line = _image.scanLine(y);

        for (int x = 0; x < _image.width() * 4; x += 4)
        {
          RGBAFloat _imagePixel(
            _line[x + 2] / 255.0,
            _line[x + 1] / 255.0,
            _line[x + 0] / 255.0,
            _line[x + 3] / 255.0);

          RGBAFloat _pixel = _f(_imagePixel, buffer_.data()[_pos + x / 4]);
          _line[x + 2] = qBound(0, int(_pixel.r * 255), 255);
          _line[x + 1] = qBound(0, int(_pixel.g * 255), 255);
          _line[x + 0] = qBound(0, int(_pixel.b * 255), 255);
          _line[x + 3] = qBound(0, int(_pixel.a * 255), 255);
        }
        _pos += _image.width();
      }*/
      return _image;
    }

    CalibrationMode Calibration::mode() const {
      return mode_;
    }

    RenderBuffer const& Calibration::buffer() const {
      return buffer_;
    }

    QRect const& Calibration::screenGeometry() const {
        return screenGeometry_;
    }

    QRect const& Calibration::contentGeometry() const {
        return contentGeometry_;
    }

    QSize Calibration::renderSize() const {
      return QSize(buffer_.width(),buffer_.height());
    }

    ColorCorrection const& Calibration::colorCorrection() const {
      return colorCorrection_;
    }

    bool Calibration::virtualScreen() const {
      return virtualScreen_;
    }

    template<typename PROJECTION, typename MODELVIEW>
    void Calibration::renderToBuffer(RenderBuffer& _buffer,
                                  PROJECTION    _proj,
                                  MODELVIEW     _mv)
    {
      int _w = _buffer.width();
      int _h = _buffer.height();

      visual::with_current_context([&](QOpenGLFunctions& _)
      {
        GLuint fb = 0, _colorTex = 0, _depthRb = 0;

        // RGBA8 2D texture, 24 bit depth texture, 256x256
        _.glGenTextures(1, &_colorTex);
        _.glBindTexture(GL_TEXTURE_2D, _colorTex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // NULL means reserve texture memory, but texels are undefined
        _.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, _w, _h, 0, GL_RGBA,
                       GL_FLOAT, NULL);

        // -------------------------
        _.glGenFramebuffers(1, &fb);
        _.glBindFramebuffer(GL_FRAMEBUFFER, fb);

        //   Attach 2D texture to this FBO
        _.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                 GL_TEXTURE_2D, _colorTex, 0);

        // -------------------------
        _.glGenRenderbuffers(1, &_depthRb);
        _.glBindRenderbuffer(GL_RENDERBUFFER, _depthRb);
        _.glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, _w,
                                _h);

        // -------------------------
        // Attach depth buffer to FBO
        _.glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                                    GL_DEPTH_ATTACHMENT,
                                    GL_RENDERBUFFER, _depthRb);

        // -------------------------
        // Does the GPU support current FBO configuration?
        GLenum status;
        status = _.glCheckFramebufferStatus(GL_FRAMEBUFFER);

        switch (status)
        {
        case GL_FRAMEBUFFER_COMPLETE:
          qDebug() << "good";
          break;

        default:
          qDebug() << "bad";
        }

        // -------------------------
        // and now you can render to GL_TEXTURE_2D
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        _.glBindFramebuffer(GL_FRAMEBUFFER, fb);
        {
          _.glClearColor(0.0, 0.0, 0.0, 1.0);
          _.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
                    GL_STENCIL_BUFFER_BIT);

          // -------------------------
          glViewport(0, 0, _w, _h);
          glMatrixMode(GL_PROJECTION);
          glLoadIdentity();
          _proj(_); // Projection operation

          glMatrixMode(GL_MODELVIEW);
          glLoadIdentity();
          glEnable(GL_DEPTH_TEST);
          _mv(_); // Model view operation

          // -------------------------
          _.glReadPixels(0, 0, _w, _h, GL_RGBA, GL_FLOAT, _buffer.ptr());
        }
        _.glBindFramebuffer(GL_FRAMEBUFFER, 0);


        // Delete resources
        _.glDeleteTextures(1, &_colorTex);
        _.glDeleteRenderbuffers(1, &_depthRb);

        // Bind 0, which means render to back buffer, as a result, fb is
        // unbound
        _.glBindFramebuffer(GL_FRAMEBUFFER, 0);
        _.glDeleteFramebuffers(1, &fb);
        glPopAttrib();
      });
    }

    template<typename OPERATION>
    void Calibration::bufferToImage(RenderBuffer const& _buffer,
                                 QImage& _image,
                                 OPERATION           _f)
    {
      if ((_image.width() != _buffer.width()) &&
          (_image.height() != _buffer.height())) {
        _image = QImage(_buffer.width(), _buffer.height(), QImage::Format_ARGB32);
      }

      int _pos = 0;

      for (int y = 0; y < _image.height(); ++y)
      {
        uchar *_line = _image.scanLine(y);

        for (int x = 0; x < _image.width() * 4; x += 4)
        {
          RGBAFloat _imagePixel(
            _line[x + 2] / 255.0,
            _line[x + 1] / 255.0,
            _line[x + 0] / 255.0,
            _line[x + 3] / 255.0);

          RGBAFloat _pixel = _f(_imagePixel, _buffer.data()[_pos + x / 4]);
          _line[x + 2] = qBound(0, int(_pixel.r * 255), 255);
          _line[x + 1] = qBound(0, int(_pixel.g * 255), 255);
          _line[x + 0] = qBound(0, int(_pixel.b * 255), 255);
          _line[x + 3] = qBound(0, int(_pixel.a * 255), 255);
        }
        _pos += _image.width();
      }
    }

    void Calibration::getUpper8bit(QImage& _image) const
    {
      _image = QImage(buffer_.width(), buffer_.height(), QImage::Format_ARGB32);

      int _pos = 0;

      auto convUpper = [](float _v)
                       {
                         int i = _v * (1 << 8);

                         return qBound(0, i, 255);
                       };

      for (int y = 0; y < _image.height(); ++y)
      {
        uchar *_line = _image.scanLine(y);

        for (int x = 0; x < _image.width() * 4; x += 4)
        {
          RGBAFloat _pixel = buffer_.data()[_pos + x / 4];

          if ((_pixel.r == 0.0) && (_pixel.g == 0.0) && (_pixel.b == 0.0)) {
            _line[x + 2] = 128;
            _line[x + 1] = 128;
            _line[x + 0] = 128;
            _line[x + 3] = 255;
            continue;
          }
          _line[x + 2] = convUpper(_pixel.r);
          _line[x + 1] = convUpper(_pixel.g);
          _line[x + 0] = convUpper(_pixel.b);
          _line[x + 3] = 255;
        }
        _pos += _image.width();
      }
    }

    void Calibration::getLower8bit(QImage& _image) const
    {
      _image = QImage(buffer_.width(), buffer_.height(), QImage::Format_ARGB32);

      int  _pos      = 0;
      auto convLower = [](float _v)
                       {
                         int i = _v * (1 << 16);

                         return i & 255;
                       };

      for (int y = 0; y < _image.height(); ++y)
      {
        uchar *_line = _image.scanLine(y);

        for (int x = 0; x < _image.width() * 4; x += 4)
        {
          RGBAFloat _pixel = buffer_.data()[_pos + x / 4];

          _line[x + 2] = convLower(_pixel.r);
          _line[x + 1] = convLower(_pixel.g);
          _line[x + 0] = convLower(_pixel.b);
          _line[x + 3] = 255;
        }
        _pos += _image.width();
      }
    }

    void Calibration::getAlphaMask(QImage& _image,
                                  Channel _channel) const
    {
      bufferToImage(buffer_, _image,
                    [&](RGBAFloat const& _input, RGBAFloat const& _pixel)
      {
        RGBAFloat _output = _input;

        switch (_channel) {
        case Channel::RED: _output.r = _pixel.a; break;
        case Channel::GREEN: _output.g = _pixel.a; break;
        case Channel::BLUE: _output.b = _pixel.a; break;
        case Channel::ALL: return RGBAFloat(_pixel.a, _pixel.a, _pixel.a);
        }
        return _output;
      });
    }

    void Calibration::encodeColorCorrection(
      QImage& _image, Channel _channel) const {
      for (int y = 0; y < _image.height(); ++y)
      {
        uchar *_line = _image.scanLine(y);

        // Channel correction index (0 = ALL, 1 = RED, 2 = GREEN, 3 = BLUE)
        int _ccIndex = y * 4 / _image.height();

        // Component index (0 = Gamma, 1 = brightness, 2 = contrast, 3 =
        // multiplier)
        int _componentIndex = (y * 16 / _image.height()) % 4;

        proj::ChannelCorrection const *_channelCorrection =
          colorCorrection_.correction(util::intToEnum<proj::Channel>(
                                        _ccIndex));

        float _component = 0.0;
        int   _result    = 0;

        // Get channel correction component, based in given component index
        if (_channelCorrection) {
          switch (_componentIndex) {
          case 0: _component = _channelCorrection->gamma(); break;
          case 1: _component = _channelCorrection->brightness(); break;
          case 2: _component = _channelCorrection->contrast(); break;
          case 3: _component = _channelCorrection->multiplier(); break;
          }

          // Convert float to byte
          _result = qBound(0, int((_component + 1.0) * 0.5 * 255), 255);
        }

        for (int x = 0; x < _image.width() * 4; x += 4)
        {
          // Insert result value into destination color channel
          switch (_channel) {
          case Channel::ALL:
            _line[x + 0] = _result;
            _line[x + 1] = _result;
            _line[x + 2] = _result;
            break;

          default:
          case Channel::GREEN:
            _line[x + 1] = _result;
            break;

          case Channel::BLUE:
            _line[x + 0] = _result;
            break;

          case Channel::RED:
            _line[x + 2] = _result;
          }
        }
      }
    }


  }
}
