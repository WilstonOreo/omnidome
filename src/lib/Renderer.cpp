/* Copyright (c) 2014-2015 "Omnidome" by cr8tr
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

#include <omni/Renderer.h>

#include <QPainter>

#include <omni/util.h>
#include <omni/visual/util.h>
#include <omni/visual/Session.h>
#include <omni/visual/Tuning.h>
#include <omni/proj/ScreenSetup.h>

namespace omni {
    Renderer::Renderer(Session& _session, RenderOptions const& _options) :
        session_(_session),
        options_(_options)
    {}

    RenderOptions const& Renderer::options() const
    {
        return options_;
    }

    RenderOptions& Renderer::options()
    {
        return options_;
    }

    void Renderer::setOptions(RenderOptions const& _options)
    {
        options_ = _options;
    }

    std::map<QScreen const *, QImage>Renderer::stitchScreens(
        std::vector<const proj::Tuning *>const& _tunings) const
    {
        // Get all tunings for each screen
        std::map<QScreen const *,
                 std::vector<proj::Tuning const *> > _screenToTunings;

        for (auto& _tuning : _tunings)
        {
            auto *_screen = _tuning->screen();

            if (_screenToTunings.count(_screen) == 0)
            {
                _screenToTunings[_screen] = std::vector<proj::Tuning const *>(
                    { _tuning });
            }
            else
            {
                _screenToTunings[_screen].push_back(_tuning);
            }
        }

        /// Render screen images from tunings
        std::map<QScreen const *, QImage> _result;

        for (auto& _screenTunings : _screenToTunings)
        {
            auto *_screen  = _screenTunings.first;
            auto& _tunings = _screenTunings.second;

            auto   _screenRect = _screen->geometry();
            QImage _image(_screenRect.width(),
                          _screenRect.height() * 3, QImage::Format_RGB32); // Overall
                                                                           // screen
                                                                           // image

            QPainter _p(&_image);

            for (auto& _tuning : _tunings)
            {
                auto _rect = proj::ScreenSetup::subScreenRect(_screen,
                                                              _tuning->subScreenIndex());
                QImage _tuningImage;
                render(_tuning, _tuningImage);
                _p.drawImage(_rect.x(), 0, _tuningImage);
            }
            _p.end();
            _result[_screen] = _image;
        }
        return _result;
    }

    void Renderer::renderToFile(QString const& _filename)
    {
        std::vector<proj::Tuning const *> _tunings;

        QString _rawName = util::removeFileExt(_filename);

        // Get all tunings
        for (auto& _tuning : session_.tunings())
        {
            if (!_tuning->hasScreen() &&
                 options_.excludeUnassignedProjectors()) continue;
            _tunings.push_back(_tuning.get());
        }

        switch (options_.separationMode())
        {
        /// Merge all together
        case SeparationMode::NONE:
        {
            auto _desktopRect = proj::ScreenSetup::desktopRect();
            auto && _screens = stitchScreens(_tunings);
            QImage _stitchedImage(_desktopRect.width(),
                                  _desktopRect.height() * 3,
                                  QImage::Format_RGB32);
            QPainter _p(&_stitchedImage);

            for (auto& _screenImage : _screens)
            {
                auto *_screen = _screenImage.first;
                auto& _image  = _screenImage.second;
                _p.drawImage(
                    _screen->geometry().topLeft() - _desktopRect.topLeft(),
                    _image);
            }
            _stitchedImage.save(_filename);
            break;
        }

        /// A file for each screen
        case SeparationMode::SCREENS:
        {
            auto && _screens = stitchScreens(_tunings);

            for (auto& _screenImage : _screens)
            {
                auto *_screen = _screenImage.first;
                auto& _image  = _screenImage.second;
                auto && _screenRect = _screen->geometry();
                QString _screenDesc(QString("_%1_%2_%3x%4.png").
                                    arg(_screenRect.left()).
                                    arg(_screenRect.top()).
                                    arg(_screenRect.width()).
                                    arg(_screenRect.height()));
                _image.save(_rawName + _screenDesc);
            }
            break;
        }

        case SeparationMode::PROJECTORS:
        {
            int _tuningIndex = 0;

            for (auto& _tuning : _tunings)
            {
                QImage _image;
                render(_tuning, _image);
                _image.save(_rawName + QString("_%1.png").arg(_tuningIndex + 1));
            }
            break;
        }
        }
    }

    void Renderer::render(proj::Tuning const *_tuning,
                          RenderBuffer      & _buffer) const
    {
        int _w = _buffer.width() == 0 ? _tuning->width() : _buffer.width();
        int _h = _buffer.height() == 0 ? _tuning->height() : _buffer.height();

        /// 1st Step: Render projectors view to texture
        RenderBuffer _projBuffer(_w, _h);

        visual::Session _sessionViz(session_);

        renderToBuffer(_projBuffer,

// Projection operation
                       [&](QOpenGLFunctions& _)
        {
            glMultMatrixf(_tuning->projector().projectionMatrix().constData());
        },

// Model view operation
                       [&](QOpenGLFunctions& _)
        {
            // Draw canvas with
            _sessionViz.update();
            _sessionViz.drawCanvas(options_.mappingOutputMode());
        }
                       );

        GLuint _projTex = 0;

        // 2nd step: Update render buffer as floating point texture
        visual::with_current_context([&](QOpenGLFunctions& _)
        {
            _.glGenTextures(1, &_projTex);
            _.glBindTexture(GL_TEXTURE_2D, _projTex);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            _.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F,
                           _projBuffer.width(), _projBuffer.height(), 0,
                           GL_RGBA, GL_FLOAT, _projBuffer.ptr());
            _.glBindTexture(GL_TEXTURE_2D, 0);
        });

        // 3rd step: Render warp grid
        RenderBuffer _warpBuffer(_w, _h);

        visual::Tuning _tuningViz(*const_cast<proj::Tuning *>(_tuning));
        _tuningViz.update();

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
            _tuningViz.drawBlendMask();
        });

        // 5th step: Merge blend and warp buffer
        _buffer.resize(_w, _h);

        for (int i = 0; i < _buffer.data().size(); ++i)
        {
            auto& _wP = _warpBuffer[i]; // Pixel from warp grid buffer
            _buffer[i] = RGBAFloat(_wP.r, _wP.g, _wP.b, _blendBuffer[i].r);
        }
    }

    void Renderer::render(proj::Tuning const *_tuning, QImage& _image,
                          int _height) const
    {
        int _w = _image.width() == 0 ? _tuning->width() : _image.width();
        int _h = _image.height() == 0 ? _tuning->height() : _image.height();

        int _overallHeight = _height == 0 ? _h : _height;

        RenderBuffer _buffer(_w, _h);

        render(_tuning, _buffer);
        _image = QImage(_w, _overallHeight * 3, QImage::Format_RGB32);

        if (options_.mappingOutputMode() == mapping::OutputMode::MAPPED_INPUT)
        {
            bufferToImage(_buffer, _image, [&](RGBAFloat const& _pixel)
            {
                return _pixel;
            });
        }
        else
        {
            QImage _upper8bit(_w, _h, QImage::Format_ARGB32);
            getUpper8bit(_buffer, _upper8bit);
            QImage _lower8bit(_w, _h, QImage::Format_ARGB32);
            getLower8bit(_buffer, _lower8bit);
            QImage _blendMask(_w, _h, QImage::Format_ARGB32);
            getAlphaMask(_buffer, _blendMask);

            // Encode color correction information into the green channel
            encodeColorCorrection(
                _tuning->colorCorrection(), Channel::GREEN, _blendMask);

            QPainter _p(&_image);
            _p.drawImage(QPoint(0, 0),                  _upper8bit);
            _p.drawImage(QPoint(0, _overallHeight),     _lower8bit);
            _p.drawImage(QPoint(0, 2 * _overallHeight), _blendMask);
            _p.end();
        }
    }

    template<typename PROJECTION, typename MODELVIEW>
    void Renderer::renderToBuffer(RenderBuffer& _buffer,
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
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            // NULL means reserve texture memory, but texels are undefined
            _.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, _w, _h, 0, GL_RGBA,
                           GL_FLOAT, NULL);

            // -------------------------
            _.glGenFramebuffers(1, &fb);
            _.glBindFramebuffer(GL_FRAMEBUFFER_EXT, fb);

            //   Attach 2D texture to this FBO
            _.glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                                     GL_TEXTURE_2D, _colorTex, 0);

            // -------------------------
            _.glGenRenderbuffers(1, &_depthRb);
            _.glBindRenderbuffer(GL_RENDERBUFFER_EXT, _depthRb);
            _.glRenderbufferStorage(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, _w,
                                    _h);

            // -------------------------
            // Attach depth buffer to FBO
            _.glFramebufferRenderbuffer(GL_FRAMEBUFFER_EXT,
                                        GL_DEPTH_ATTACHMENT_EXT,
                                        GL_RENDERBUFFER_EXT, _depthRb);

            // -------------------------
            // Does the GPU support current FBO configuration?
            GLenum status;
            status = _.glCheckFramebufferStatus(GL_FRAMEBUFFER_EXT);

            switch (status)
            {
            case GL_FRAMEBUFFER_COMPLETE_EXT:
                qDebug() << "good";
                break;

            default:
                qDebug() << "bad";
            }

            // -------------------------
            // and now you can render to GL_TEXTURE_2D
            glPushAttrib(GL_ALL_ATTRIB_BITS);
            _.glBindFramebuffer(GL_FRAMEBUFFER_EXT, fb);
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
            _.glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);


            // Delete resources
            _.glDeleteTextures(1, &_colorTex);
            _.glDeleteRenderbuffers(1, &_depthRb);

            // Bind 0, which means render to back buffer, as a result, fb is
            // unbound
            _.glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
            _.glDeleteFramebuffers(1, &fb);
            glPopAttrib();
        });
    }

    template<typename OPERATION>
    void Renderer::bufferToImage(RenderBuffer const& _buffer,
                                 QImage            & _image,
                                 OPERATION           _f)
    {
        _image = QImage(_buffer.width(), _buffer.height(), QImage::Format_ARGB32);

        int _pos = 0;

        for (int y = 0; y < _image.height(); ++y)
        {
            uchar *_line = _image.scanLine(y);

            for (int x = 0; x < _image.width() * 4; x += 4)
            {
                RGBAFloat _pixel = _f(_buffer.data()[_pos + x / 4]);
                _line[x + 2] = qBound(0, int(_pixel.r * 255), 255);
                _line[x + 1] = qBound(0, int(_pixel.g * 255), 255);
                _line[x + 0] = qBound(0, int(_pixel.b * 255), 255);
                _line[x + 3] = qBound(0, int(_pixel.a * 255), 255);
            }
            _pos += _image.width();
        }
    }

    void Renderer::getUpper8bit(RenderBuffer const& _buffer, QImage& _image) const
    {
        _image = QImage(_buffer.width(), _buffer.height(), QImage::Format_ARGB32);

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
                RGBAFloat _pixel = _buffer.data()[_pos + x / 4];

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

    void Renderer::getLower8bit(RenderBuffer const& _buffer, QImage& _image) const
    {
        _image = QImage(_buffer.width(), _buffer.height(), QImage::Format_ARGB32);

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
                RGBAFloat _pixel = _buffer.data()[_pos + x / 4];

                _line[x + 2] = convLower(_pixel.r);
                _line[x + 1] = convLower(_pixel.g);
                _line[x + 0] = convLower(_pixel.b);
                _line[x + 3] = 255;
            }
            _pos += _image.width();
        }
    }

    void Renderer::getAlphaMask(RenderBuffer const& _buffer, QImage& _image) const
    {
        bufferToImage(_buffer, _image, [&](RGBAFloat const& _pixel)
        {
            return RGBAFloat(_pixel.a, _pixel.a, _pixel.a);
        });
    }

    void Renderer::encodeColorCorrection(
        ColorCorrection const& _colorCorrection,
        Channel                _channel,
        QImage               & _image) const {
        for (int y = 0; y < _image.height(); ++y)
        {
            uchar *_line = _image.scanLine(y);

            // Channel correction index (0 = ALL, 1 = RED, 2 = GREEN, 3 = BLUE)
            int _ccIndex = y * 4 / _image.height();

            // Component index (0 = Gamma, 1 = brightness, 2 = contrast, 3 =
            // multiplier)
            int _componentIndex = (y * 16 / _image.height()) % 4;

            proj::ChannelCorrection const *_channelCorrection =
                _colorCorrection.correction(util::intToEnum<proj::Channel>(
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
