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

#include <omni/visual/Session.h>

#include <QOpenGLShaderProgram>
#include <QOpenGLShader>

#include <omni/util.h>
#include <omni/visual/util.h>
#include <omni/proj/Frustum.h>

namespace omni {
  namespace visual {
    std::unique_ptr<QOpenGLShaderProgram> Session::frustumShader_;

    Session::Session(omni::Session const& _session) :
      session_(_session)
    {}

    omni::Session const& Session::session() const
    {
      return session_;
    }

    void Session::drawCanvasWithMatrix() const {
      auto _canvas = session_.canvas();

      if (!_canvas) return;

      glPushMatrix();
      {
        glLoadIdentity();
        glMultMatrixf(_canvas->matrix().constData());
        _canvas->draw();
      }
      glPopMatrix();
    }

    void Session::drawCanvasFor3DView() {
      auto _canvas = session_.canvas();

      if (!_canvas) return;

      with_current_context([&](QOpenGLFunctions& _)
      {
        auto *_input = session_.inputs().current();
        auto *_mapping = const_cast<mapping::Interface *>(session_.mapping());

        bool _displayInput = session_.scene().displayInput() &&
                             _mapping && _input;

        if (_input) {
          /// There is also no input to be displayed when input has no texture
          _displayInput &= _input->textureId() != 0;
        }

        if (_displayInput)
        {
          _.glDisable(GL_LIGHTING);
          _.glEnable(GL_DEPTH_TEST);
          _.glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
          _.glActiveTexture(GL_TEXTURE0);
          _.glBindTexture(GL_TEXTURE_RECTANGLE, _input->textureId());
          glPushMatrix();
          {
            glLoadIdentity();
            glMultMatrixf(_canvas->matrix().constData());

            _mapping->bind(session_.inputs().current(),
                           session_.scene().insideOutside());
            {
              _canvas->draw();
            }
            _mapping->release();

            _.glDisable(GL_DEPTH_TEST);
            _.glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            _mapping->bind(session_.inputs().current(),
                           1.0 - session_.scene().insideOutside());
            {
              _canvas->draw();
            }
            _mapping->release();
            _.glBindTexture(GL_TEXTURE_RECTANGLE, 0);
            glPopMatrix();
          }
          _.glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
          _.glEnable(GL_DEPTH_TEST);
        }
        else
        {
          _.glEnable(GL_LIGHTING);

          // Render canvas with lighting when there is no input
          glColor4f(1.0, 1.0, 1.0, 1.0);
          drawCanvasWithMatrix();
        }
      });
    }

    void Session::drawCanvas(mapping::OutputMode _mode, bool _grayscale) const
    {
      auto _canvas = session_.canvas();

      if (!_canvas) return;

      with_current_context([&](QOpenGLFunctions& _)
      {
        auto *_input = session_.inputs().current();
        auto *_mapping = const_cast<mapping::Interface *>(session_.mapping());

        if (_input && session_.mapping() &&
            (_mode != mapping::OutputMode::LIGHTING_ONLY))
        {
          _.glDisable(GL_LIGHTING);
          _.glActiveTexture(GL_TEXTURE0);
          _.glBindTexture(GL_TEXTURE_RECTANGLE, _input->textureId());
          _mapping->bind(session_.inputs().current(), _mode, _grayscale);
          drawCanvasWithMatrix();
          _.glBindTexture(GL_TEXTURE_RECTANGLE, 0);
          _mapping->release();
        }
        else
        {
          // Render canvas with lighting when there is no input
          glColor4f(1.0, 1.0, 1.0, 1.0);
          drawCanvasWithMatrix();
        }
      });
    }

    void Session::drawCanvasWithFrustumIntersections(
      ProjectorViewMode _projectorViewMode,
      ProjectorSelectionMode _selectionMode) const
    {
      projectorDrawFunction(_selectionMode, [&](Projector const&, int i) {
        auto _tuning = session_.tunings()[i];
        drawFrustumIntersection(_tuning->projector(),
                                _tuning->color(), _projectorViewMode);
      });
    }

    void Session::drawFrustumIntersection(
      proj::Projector const& _proj,
      QColor const& _color,
      ProjectorViewMode      _projectorViewMode) const
    {
      auto _canvas = session_.canvas();

      if (!frustumShader_ || !_canvas) return;

      frustumShader_->bind();
      auto _invMatrix = _canvas->matrix().inverted();
      auto _m         = _invMatrix * _proj.matrix();
      auto _rot       = _invMatrix;
      _rot.setColumn(3, QVector4D(0, 0, 0, 1));

      frustumShader_->setUniformValue("color", _color.redF(),
                                      _color.greenF(), _color.blueF());

      /// Construct frustum
      proj::Frustum _frustum(_proj);
      QVector3D     _eye    = _m.column(3).toVector3D();
      QVector3D     _lookAt = _m * QVector3D(1.0, 0.0, 0.0) - _eye;

      // QVector3D _up = _m * QVector3D(0.0, 0.0, 1.0) - _eye;

      // Setup frustum uniforms for intersection test
      frustumShader_->setUniformValue("eye",          _eye);
      frustumShader_->setUniformValue("look_at",      _lookAt);
      frustumShader_->setUniformValue("top_left",     _frustum.topLeft(_m));
      frustumShader_->setUniformValue("top_right",    _frustum.topRight(_m));
      frustumShader_->setUniformValue("bottom_left",  _frustum.bottomLeft(_m));
      frustumShader_->setUniformValue("bottom_right", _frustum.bottomRight(_m));
      frustumShader_->setUniformValue("frame_width",
                                      GLfloat(0.005 * session_.scene().size()));
      frustumShader_->setUniformValue("matrix",       _rot);
      frustumShader_->setUniformValue("proj_matrix",  _proj.matrix());
      frustumShader_->setUniformValue("scale",
                                      GLfloat(_canvas->bounds().radius()));
      frustumShader_->setUniformValue("opacity", GLfloat(0.8));
      frustumShader_->setUniformValue("view_mode",
                                      int(_projectorViewMode));

      glDisable(GL_DEPTH_TEST);
      drawCanvasWithMatrix();
      glEnable(GL_DEPTH_TEST);

      frustumShader_->release();
    }

    bool Session::needsUpdate() const
    {
      return session_.tunings().size() != projectors_.size() || needsUpdate_;
    }

    template<typename F>
    void Session::projectorDrawFunction(ProjectorSelectionMode _selectionMode,
                                        F f) const {
      int i = 0;

      for (auto& _proj : projectors_) {
        switch (_selectionMode) {
        default:
        case ProjectorSelectionMode::ALL:
          f(_proj, i);
          break;

        case ProjectorSelectionMode::SELECTED:

          if (i == session_.tunings().currentIndex()) {
            f(_proj, i);
          }

        case ProjectorSelectionMode::NONE:
          break;
        }
        ++i;
      }
    }

    void Session::drawProjectors(ProjectorSelectionMode _selectedMode) const
    {
      if (needsUpdate()) return;

      glDisable(GL_DEPTH_TEST);

      projectorDrawFunction(_selectedMode, [&](Projector const& _proj, int) {
        _proj.draw();
      });

      if (session_.canvas())
      {
        projectorDrawFunction(_selectedMode, [&](Projector const& _proj, int) {
          _proj.drawPositioning(
            session_.canvas()->center());
        });
      }

      glEnable(GL_DEPTH_TEST);
    }

    void Session::drawProjectorHalos(ProjectorSelectionMode _selectionMode) const
    {
      if (needsUpdate()) return;

      projectorDrawFunction(_selectionMode, [&](Projector const& _proj, int i) {
        if (session_.tunings()[i]->outputEnabled()) {
          _proj.drawHalo();
        }
      });
    }

    void Session::update()
    {
      if (!QOpenGLContext::currentContext()) return;

      // Update projector visualizers
      projectors_.clear();

      for (int i = 0; i < session_.tunings().size(); ++i)
      {
        projectors_.emplace_back(session_.tunings()[i]->projector());
        projectors_.back().setColor(session_.tunings()[i]->color());
        projectors_.back().setSelected(
          i == session_.tunings().currentIndex());
        projectors_.back().setSize(session_.scene().size() / 10.0);
        projectors_.back().update();
      }

      // Setup frustum/canvas intersection shader
      if (!frustumShader_)
      {
        using omni::util::fileToStr;
        static QString _vertSrc     = fileToStr(":/shaders/frustum.vert");
        static QString _fragmentSrc = fileToStr(":/shaders/frustum.frag");

        frustumShader_.reset(new QOpenGLShaderProgram());
        frustumShader_->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                                _vertSrc);
        frustumShader_->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                                _fragmentSrc);
        frustumShader_->link();
      }
      needsUpdate_ = false;
    }
  }
}
