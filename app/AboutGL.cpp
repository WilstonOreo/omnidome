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


#include "AboutGL.h"

#include <QDesktopServices>
#include <QUrl>
#include <QFile>
#include <QImage>
#include <QPainter>
#include <QTimer>
#include <chrono>
#include <type_traits>
#include <omni/util.h>
#include <omni/visual/Shader.h>
#include <omni/visual/util.h>
#include <omni/visual/Rectangle.h>


using namespace omni::ui;

AboutGL::AboutGL(QWidget *_parent) :
  GLView(_parent)
{
  QTimer *timer = new QTimer(this);

  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
  timer->start(20);

  /// "Random" start time
  startTime_ = visual::util::now() +
               double(reinterpret_cast<long long>(timer) & ((1 << 16) - 1));
}

AboutGL::~AboutGL()
{}

bool AboutGL::initialize()
{
  /// Initialize Shader
  static QString _vertSrc     = util::fileToStr(":/shaders/frustum.vert");
  static QString _fragmentSrc = util::fileToStr(":/shaders/slow_fractal.frag");

  primaryContextSwitch([&](QOpenGLFunctions& _) {
    shader_.reset(new QOpenGLShaderProgram(this));
    shader_->addShaderFromSourceCode(QOpenGLShader::Vertex, _vertSrc);
    shader_->addShaderFromSourceCode(QOpenGLShader::Fragment, _fragmentSrc);
    shader_->link();

    QImage _image(QString(":/omnicredits.png"));//, QImage::Format_ARGB32);
    tex_.reset(new QOpenGLTexture(_image.rgbSwapped().mirrored()));

    _.glFinish();
  });
  return true;
}

void AboutGL::resizeGL(int _w, int _h)
{
  _w = _w & ~1;
  _h = _h & ~1;
  glViewport(0, 0, (GLint)_w, (GLint)_h);
  glClearColor(0.0, 0.0, 0.0, 1.0);
}

void AboutGL::mousePressEvent(QMouseEvent *)
{
  QDesktopServices::openUrl(QUrl("http://omnido.me", QUrl::TolerantMode));
}

void AboutGL::paint()
{
  if (!shader_ || !tex_) {
    return;
  }

  makeCurrent();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDisable(GL_CULL_FACE);

  /// Setup orthogonal projection
  glMatrixMode(GL_PROJECTION);
  {
    glLoadIdentity();
    QMatrix4x4 _m;
    _m.ortho(-0.5, 0.5, -0.5, 0.5, -1.0, 1.0);
    glMultMatrixf(_m.constData());
  }

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  using namespace visual;

  viewport(this);

  double _time =
    std::chrono::high_resolution_clock::now().time_since_epoch().
    count() / 1000000000.0 - startTime_;

  useShader(*shader_, [&](UniformHandler& _handler) {
    _handler.uniform("time", GLfloat(_time));
    _handler.uniform("resolution", GLfloat(width()*devicePixelRatio()),
      GLfloat(height()*devicePixelRatio()));
    _handler.texUniform("tex", *tex_);
    _handler.uniform("tex_size", QVector2D(tex_->width(),tex_->height()));

    visual::Rectangle::draw();
  });
}
