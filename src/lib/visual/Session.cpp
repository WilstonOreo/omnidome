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

namespace omni
{
  namespace visual
  {
    std::unique_ptr<QOpenGLShaderProgram> Session::frustumShader_;

    Session::Session(omni::Session& _session) :
      session_(_session)
    {
    }

    omni::Session& Session::session()
    {
      return session_;
    }

    omni::Session const& Session::session() const
    {
      return session_;
    }

    void Session::drawCanvas(mapping::OutputMode _mode, bool _displayInput) const
    {
      auto _canvas = session_.canvas();

      if (!_canvas) return;

      with_current_context([&](QOpenGLFunctions& _)
      {
        auto* _input = session_.inputs().current();
        auto* _mapping = session_.mapping();

        if (session_.hasOutput() && _displayInput)
        {
          GLuint _texId = _input ? _input->textureId() : 0;
          _.glEnable(GL_TEXTURE_2D);

          _mapping->bind(_mode);
          _.glBindTexture(GL_TEXTURE_2D, _texId);
          _canvas->draw();

          _.glBindTexture(GL_TEXTURE_2D, 0);

          _mapping->release();
        }
        else
        {
          // Render canvas with lighting when there is no input
          glColor4f(1.0,1.0,1.0,1.0);
          _.glEnable(GL_LIGHTING);
          _canvas->draw();
          glPopMatrix();
          _.glDisable(GL_LIGHTING);
        }
      });
    }

    void Session::drawCanvasWithFrustumIntersections(ProjectorViewMode _projectorViewMode) const
    {
      for (auto& _tuning : session_.tunings())
      {
        drawFrustumIntersection(_tuning->projector(),_tuning->color(),_projectorViewMode);
      }
    }

    void Session::drawFrustumIntersection(
        proj::Projector const& _proj,
        QColor const& _color,
        ProjectorViewMode _projectorViewMode) const
    {
      auto _canvas = session_.canvas();
      if (!frustumShader_ || !_canvas) return;
      frustumShader_->bind();
      auto _invMatrix = _canvas->matrix().inverted();
      auto _m = _invMatrix * _proj.matrix();
      auto _rot = _invMatrix;
      _rot.setColumn(3,QVector4D(0,0,0,1));

      frustumShader_->setUniformValue("color",_color.redF(),_color.greenF(),_color.blueF());

      /// Construct frustum
      qreal _a = _proj.fov().radians() *0.5;
      qreal _height = tan(_a);
      qreal _width = _height * _proj.aspectRatio();
      QVector3D _eye = _m.column(3).toVector3D();
      QVector3D _topLeft = _m * QVector3D(1.0,-_width,_height) - _eye;
      QVector3D _topRight = _m * QVector3D(1.0,_width,_height) - _eye;
      QVector3D _bottomLeft = _m * QVector3D(1.0,-_width,-_height) - _eye;
      QVector3D _bottomRight = _m * QVector3D(1.0,_width,-_height) - _eye;
      QVector3D _lookAt = _m * QVector3D(1.0,0.0,0.0) - _eye;

      // Setup frustum uniforms for intersection test
      frustumShader_->setUniformValue("eye",_eye);
      frustumShader_->setUniformValue("look_at",_lookAt);
      frustumShader_->setUniformValue("top_left",_topLeft);
      frustumShader_->setUniformValue("top_right",_topRight);
      frustumShader_->setUniformValue("bottom_left",_bottomLeft);
      frustumShader_->setUniformValue("bottom_right",_bottomRight);
      frustumShader_->setUniformValue("matrix",_rot);
      frustumShader_->setUniformValue("view_mode",int(_projectorViewMode));

      glDisable(GL_DEPTH_TEST);
      _canvas->draw();
      glEnable(GL_DEPTH_TEST);

      frustumShader_->release();
    }

    bool Session::needsUpdate() const
    {
      return session_.tunings().size() != projectors_.size();
    }


    void Session::drawProjectors() const
    {
      if (needsUpdate()) return;

      glDisable(GL_DEPTH_TEST);
      for (auto& _proj : projectors_)
      {
        _proj.draw();
      }

      if (session_.canvas())
      {
        for (auto& _proj : projectors_)
          _proj.drawPositioning(session_.canvas()->center());
      }
      glEnable(GL_DEPTH_TEST);
    }

    void Session::drawProjectorHalos() const
    {
      if (needsUpdate()) return;

      for (auto& _proj : projectors_)
      {
        _proj.drawHalo();
      }
    }

    void Session::update()
    {
      if (!QOpenGLContext::currentContext()) return;

      auto* _canvas = session_.canvas();
      if (!_canvas) return;

      _canvas->update();

      auto* _input = session_.inputs().current();
      if (_input) _input->update();

      // Update projector visualizers
      projectors_.clear();
      for (int i = 0; i < session_.tunings().size(); ++i)
      {
        projectors_.emplace_back(session_.tunings()[i]->projector());
        projectors_.back().setColor(session_.tunings()[i]->color());
        projectors_.back().setSelected(i == session_.tunings().currentIndex());
        projectors_.back().update();
      }

      // Setup frustum/canvas intersection shader
      if (!frustumShader_)
      {
        using omni::util::fileToStr;
        static QString _vertSrc = fileToStr(":/shaders/frustum.vert");
        static QString _fragmentSrc = fileToStr(":/shaders/frustum.frag");

        frustumShader_.reset(new QOpenGLShaderProgram());
        frustumShader_->addShaderFromSourceCode(QOpenGLShader::Vertex,_vertSrc);
        frustumShader_->addShaderFromSourceCode(QOpenGLShader::Fragment,_fragmentSrc);
        frustumShader_->link();
      }
    }

    void Session::free()
    {
      auto* _input = session_.inputs().current();
      if (_input) _input->free();
    }
  }
}
