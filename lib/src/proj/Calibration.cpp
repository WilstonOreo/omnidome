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

#include <omni/proj/Tuning.h>
#include <omni/proj/Calibration.h>

#include <omni/visual/Session.h>
#include <omni/visual/Tuning.h>
#include <omni/visual/Rectangle.h>
#include <omni/visual/Shader.h>


#include <omnic/qt/wrapper.h>

#include <QPainter>

namespace omni
{
  namespace proj
  {
    Calibration::Calibration() {}

    Calibration::Calibration(Tuning const& _tuning)
    {
      render(_tuning);
    }

    void Calibration::render(Tuning const& _tuning, ContextBoundPtr<omnic::gl::FramebufferRGBA16>& _framebuffer)
    {
      using namespace visual;

      typedef omnic::gl::FramebufferRGBA16 framebuffer_type;

      int _w = data_.width() <= 0 ? _tuning.width() : data_.width();
      int _h = data_.height() <= 0 ? _tuning.height() : data_.height();
      QSize _size(_w,_h);

      virtualScreen_ = !_tuning.screen(); // Screen is virtual when tuning has
      // no screen
      colorCorrection_ = _tuning.colorCorrection();
      screenGeometry_  = _tuning.screenGeometry();
      contentGeometry_ = _tuning.contentGeometry();
      data_.resize(_w, _h);

      visual::Session *_sessionViz =
        const_cast<omni::Session&>(_tuning.session()).makeVisualizer();
      visual::Tuning *_tuningViz = const_cast<proj::Tuning&>(_tuning).makeVisualizer();

      static ContextBoundPtr<framebuffer_type> _warpBuffer;
      static ContextBoundPtr<framebuffer_type> _blendBuffer;

      static ContextBoundPtr<QOpenGLShaderProgram> _mergeShader;
      static ContextBoundPtr<QOpenGLShaderProgram> _shader;

      primaryContextSwitch([&](QOpenGLFunctions& _)
      {

        /// Update visualizers
        _sessionViz->update();
        _tuningViz->update();
        _tuningViz->updateWarpBuffer();
        _tuningViz->updateBlendTexture();
        _.glFlush();

        /// Initialize shaders
        visual::initShader(_shader, "texture");
        visual::initShader(_mergeShader, "merge");

        /// Initialize framebuffers

        /// Init framebuffer only if it does not exist yet or if size has changed
        auto _initFramebuffer = [&](ContextBoundPtr<framebuffer_type>& _fb)
        {
          bool _reset = _fb ? QSize(_fb->width(),_fb->height()) != _size : true;
          if (_reset)
          {
            _fb.reset(new framebuffer_type(_w,_h));
          }
        };
        _initFramebuffer(_warpBuffer);
        _initFramebuffer(_blendBuffer);
        _initFramebuffer(_framebuffer);

        _.glFlush();
      });


      /// 1st Step: Render projectors view to framebuffer texture
      draw_on_framebuffer(_framebuffer.get(), [&](QOpenGLFunctions& _)
      {
        // Projection operation
        glMultMatrixf(_tuning.projector().projectionMatrix().constData());
      }, [&](QOpenGLFunctions& _)
      {
        // Model view operation
        _.glClearColor(0.0,0.0,1.0,1.0);
        _.glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        _.glEnable(GL_BLEND);
        _.glEnable(GL_DEPTH_TEST);
        _sessionViz->drawCanvas(_tuning.session().exportSettings().
                                mappingOutputMode());
        _.glClearColor(0.0,0.0,0.0,0.0);
        _.glFlush();
      });


      // 3th Step: Render blend mask
      draw_on_framebuffer(_blendBuffer.get(), [&](
                            QOpenGLFunctions& _)
      {
        // Projection operation
        QMatrix4x4 _m;
        _m.ortho(-0.5, 0.5, -0.5, 0.5, -1.0, 1.0);
        glMultMatrixf(_m.constData());
      }, [&](QOpenGLFunctions& _)
      {
        _.glEnable(GL_BLEND);
        _.glDisable(GL_DEPTH_TEST);
        _.glClear(
          GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        // Model view operation
        _tuningViz->drawOutput();
        _.glFlush();
      });

      // 4rd step: Render warp grid
      draw_on_framebuffer(_warpBuffer.get(), [&](
                            QOpenGLFunctions& _)
      {
        // Projection operation
        QMatrix4x4 _m;
        _m.ortho(-0.5, 0.5, -0.5, 0.5, -1.0, 1.0);
        glMultMatrixf(_m.constData());
      }, [&](QOpenGLFunctions& _)
      {
        _.glEnable(GL_BLEND);
        _.glDisable(GL_DEPTH_TEST);
        _.glClear(
          GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        // Model view operation
        useShader(*_shader, [&](visual::UniformHandler& _h)
        {
          _h.texUniform("tex", _framebuffer->texture());
          _tuningViz->drawWarpPatch();
        });
        _.glFlush();
      });

      // 5th step: Merge blend and warp buffer
      draw_on_framebuffer(_framebuffer.get(),[&](QOpenGLFunctions& _)
      {
        // Projection operation
        QMatrix4x4 _m;
        _m.ortho(-0.5, 0.5, -0.5, 0.5, -1.0, 1.0);
        glMultMatrixf(_m.constData());

      }, [&](QOpenGLFunctions& _)
      {
        _.glEnable(GL_BLEND);
        _.glDisable(GL_DEPTH_TEST);
        _.glClear(
          GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        // Model view operation
        useShader(*_mergeShader, [&](visual::UniformHandler& _h)
        {
          _h.texUniform("warp", _warpBuffer->texture());
          _h.texUniform("blend", _blendBuffer->texture());
          visual::Rectangle::draw();
        });
        _.glFlush();
      });
    }

    void Calibration::render(Tuning const& _tuning)
    {
      ContextBoundPtr<omnic::gl::FramebufferRGBA16> _framebuffer;

      render(_tuning,_framebuffer);
      withCurrentContext([&](QOpenGLFunctions& _)
      {
        _framebuffer->bind();
        _.glViewport(0, 0, _framebuffer->width(), _framebuffer->height());
        //  Read merged output to calibration data buffer
        _.glReadPixels(0, 0, data_.width(), data_.height(), GL_RGBA, GL_UNSIGNED_SHORT, data_.ptr());
        _framebuffer->release();
      });

      primaryContextSwitch([&](QOpenGLFunctions& _)
      {
        _framebuffer.reset();
      });
    }

    QImage Calibration::toImage() const
    {
      int _w = data_.width();
      int _h = data_.height();
      if (!_w || !_h) return QImage();

      QImage _upper8bit(_w, _h, QImage::Format_ARGB32);
      getUpper8bit(_upper8bit);
      QImage _lower8bit(_w, _h, QImage::Format_ARGB32);
      getLower8bit(_lower8bit);

      // Encode color correction information into the green channel
      
      encodeAlphaMask(_upper8bit);
      QImage _image(_w, _h * 2, QImage::Format_RGB32);
      encodeColorCorrection(_lower8bit, Channel::BLUE);
      QPainter _p(&_image);
      _p.drawImage(QPoint(0, 0),                  _upper8bit);
      _p.drawImage(QPoint(0, _h),     _lower8bit);
      _p.end();
      return _image;
    }

    QImage Calibration::toPreviewImage() const
    {
      int _w = data_.width();
      int _h = data_.height();

      if (!_w || !_h) return QImage();

      QImage _upper8bit(_w, _h, QImage::Format_ARGB32);
      getUpper8bit(_upper8bit);

      // Encode color correction information into the green channel

      encodeAlphaMask(_upper8bit);
      QImage   _image(_w, _h, QImage::Format_RGB32);
      QPainter _p(&_image);
      _p.drawImage(QPoint(0, 0),                  _upper8bit);
      _p.end();
      return _upper8bit;
    }

    omnic::PixelData const& Calibration::pixelData() const
    {
      return data_;
    }

    QRect const& Calibration::screenGeometry() const
    {
      return screenGeometry_;
    }

    QRect const& Calibration::contentGeometry() const
    {
      return contentGeometry_;
    }

    QSize Calibration::renderSize() const
    {
      return QSize(data_.width(), data_.height());
    }

    void Calibration::setRenderSize(QSize const& _size)
    {
      data_.resize(_size.width(), _size.height());
    }

    ColorCorrection const& Calibration::colorCorrection() const
    {
      return colorCorrection_;
    }

    bool Calibration::virtualScreen() const
    {
      return virtualScreen_;
    }

    template<typename OPERATION>
    void Calibration::bufferToImage(pixeldata_type const& _buffer,
                                    QImage& _image,
                                    OPERATION           _f)
    {
      if ((_image.width() != _buffer.width()) &&
          (_image.height() != _buffer.height()))
      {
        _image = QImage(_buffer.width(), _buffer.height(), QImage::Format_ARGB32);
      }

      int _pos = 0;

      for (int y = 0; y < _image.height(); ++y)
      {
        uchar *_line = _image.scanLine(y);

        for (int x = 0; x < _image.width() * 4; x += 4)
        {
          auto& _bufferPixel = _buffer.data()[_pos + x / 4];
          RGBAFloat _pixel(
            _bufferPixel.u() / 65535.0,
            _bufferPixel.v() / 65535.0,
            _bufferPixel.d() / 65535.0,
            _bufferPixel.b() / 65535.0);

          _f(_buffer.data()[_pos + x / 4],_pixel);

          _line[x + 2] = qBound(0, int(_pixel.r / 256.0), 255);
          _line[x + 1] = qBound(0, int(_pixel.g / 256.0), 255);
          _line[x + 0] = qBound(0, int(_pixel.b / 256.0), 255);
          _line[x + 3] = qBound(0, int(_pixel.a / 256.0), 255);
        }
        _pos += _image.width();
      }
    }

    void Calibration::getUpper8bit(QImage& _image) const
    {
      _image = QImage(data_.width(), data_.height(), QImage::Format_ARGB32);

      int _pos = 0;

      for (int y = 0; y < _image.height(); ++y)
      {
        uchar *_line = _image.scanLine(y);

        for (int x = 0; x < _image.width() * 4; x += 4)
        {
          omnic::UVDBPixel const& _pixel = data_.data()[_pos + x / 4];
          _line[x + 2] = _pixel.u() / 256;
          _line[x + 1] = _pixel.v() / 256;
          _line[x + 0] = _pixel.d() / 256;
          _line[x + 3] = 255;
        }
        _pos += _image.width();
      }
    }

    void Calibration::getLower8bit(QImage& _image) const
    {
      _image = QImage(data_.width(), data_.height(), QImage::Format_ARGB32);

      int  _pos      = 0;
      for (int y = 0; y < _image.height(); ++y)
      {
        uchar *_line = _image.scanLine(y);

        for (int x = 0; x < _image.width() * 4; x += 4)
        {
          omnic::UVDBPixel const& _pixel = data_.data()[_pos + x / 4];
          _line[x + 2] = _pixel.u() & 255;
          _line[x + 1] = _pixel.v() & 255;
          _line[x + 0] = _pixel.d() & 255;
          _line[x + 3] = 255;
        }
        _pos += _image.width();
      }
    }

    void Calibration::encodeAlphaMask(QImage& _image,
                                   Channel _channel) const
    {
      int  _pos      = 0;
      for (int y = 0; y < _image.height(); ++y)
      {
        uchar *_line = _image.scanLine(y);

        for (int x = 0; x < _image.width() * 4; x += 4)
        {
          omnic::UVDBPixel const& _pixel = data_.data()[_pos + x / 4];
          _line[x + 0] = _pixel.b() / 256;
        }
        _pos += _image.width();
      }
    }

    void Calibration::encodeColorCorrection(
      QImage& _image, Channel _channel) const
    {
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
        if (_channelCorrection)
        {
          switch (_componentIndex)
          {
          case 0:
            _component = _channelCorrection->gamma();
            break;

          case 1:
            _component = _channelCorrection->brightness();
            break;

          case 2:
            _component = _channelCorrection->contrast();
            break;

          case 3:
            _component = _channelCorrection->multiplier();
            break;
          }

          // Convert float to byte
          _result = qBound(0, int((_component + 1.0) * 0.5 * 255), 255);
        }

        for (int x = 0; x < _image.width() * 4; x += 4)
        {
          // Insert result value into destination color channel
          switch (_channel)
          {
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

    omnic::CalibratedProjector Calibration::calibratedProjector() const
    {
      omnic::CalibratedProjector _proj;

      _proj.setVirtualScreen(this->virtualScreen());

      omnic::Rect _screenRect;
      omnic::qt::wrap(screenGeometry_,_screenRect);
      _proj.setScreenGeometry(_screenRect);

      omnic::Rect _contentRect;
      omnic::qt::wrap(contentGeometry_,_contentRect);
      _proj.setContentGeometry(_contentRect);

      omnic::ColorCorrectionLOT _colorCorrection;
      _proj.setColorCorrection(colorCorrection_.calculateLookUpTable());

      _proj.setPixelData(this->pixelData());

      return _proj;
    }
  }
}
