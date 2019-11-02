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

#include <omni/Session.h>

#include <QFile>
#include <QDataStream>

#include <omni/util.h>
#include <omni/serialization/PropertyMap.h>
#include <omni/visual/Tuning.h>
#include <omni/visual/Session.h>

#include <omni/render/Renderer.h>

namespace omni
{
  Session::Session() :
    tunings_(*this),
    screenSetup_(this)
  {
  }

  Session::~Session()
  {
  }

  proj::TuningList& Session::tunings()
  {
    return tunings_;
  }

  proj::TuningList const& Session::tunings() const
  {
    return tunings_;
  }

  Mapping* Session::setMapping(Id const& _id)
  {
    mapping_.reset(Mapping::factory().create(_id));
    if (mapping_) {
      mapping_->setScene(&scene_);
    }
    return mapping();
  }

  Mapping* Session::mapping()
  {
    return mapping_.get();
  }

  Mapping const* Session::mapping() const
  {
    return mapping_.get();
  }

  InputList& Session::inputs()
  {
    return inputs_;
  }

  InputList const& Session::inputs() const
  {
    return inputs_;
  }

  Canvas* Session::setCanvas(Id const& _id)
  {
    canvas_.reset(Canvas::factory().create(_id));
    return canvas_.get();
  }

  Canvas* Session::canvas()
  {
    return canvas_.get();
  }

  Canvas const* Session::canvas() const
  {
    return canvas_.get();
  }

  ScreenSetup& Session::screenSetup()
  {
    return screenSetup_;
  }

  ScreenSetup const& Session::screenSetup() const
  {
    return screenSetup_;
  }

  /// Return reference to blend settings
  BlendSettings& Session::blendSettings() {
      return blendSettings_;
  }

  /// Return reference to blend settings
  BlendSettings const& Session::blendSettings() const {
      return blendSettings_;
  }

  Session::Mode Session::mode() const
  {
    return mode_;
  }

  void Session::setMode(Mode _mode)
  {
    mode_=_mode;
  }

  Scene& Session::scene() {
      return scene_;
  }

  Scene const& Session::scene() const {
      return scene_;
  }

  ExportSettings& Session::exportSettings() {
      return exportSettings_;
  }

  ExportSettings const& Session::exportSettings() const {
      return exportSettings_;
  }

  bool Session::hasOutput() const
  {
    return inputs().current() && canvas() && mapping() && (tunings().size() > 0) &&
        (mode() != Mode::SCREENSETUP);
  }

  /// Export calibration data of session to a file
  void Session::renderToFile(QString const& _filename)
  {
    render::Renderer _renderer(*this);
    _renderer.renderToFile(_filename);
  }

  void Session::save(QString const& _filename) const
  {
    QFile _file(_filename);
    _file.open(QIODevice::WriteOnly);
    QDataStream _stream(&_file);
    _stream << *this;
  }

  void Session::load(QString const& _filename)
  {
    QFile _file(_filename);
    _file.open(QIODevice::ReadOnly);
    QDataStream _stream(&_file);
    _stream >> *this;
  }

  void Session::toStream(QDataStream& _os) const {
      PropertyMap _map;
      _map("version",OMNI_VERSION_STRING);
      _map("scene",scene_)
          ("tunings",tunings_)
          ("mapping",mapping_)
          ("inputs",inputs_)
          ("canvas",canvas_)
          ("mode",mode_)
          ("blendSettings",blendSettings_)
          ("exportSettings",exportSettings_);
      _os << _map;
  }

  void Session::fromStream(QDataStream& _is) {
      /// Reset visualizer
      viz_.reset();

      PropertyMap _map;
      _is >> _map;
      _map.get("scene",scene_);
      _map.get("tunings",tunings_);
      _map.getPtr("mapping",[&](Id const& _id) {
         return setMapping(_id);
      });
      _map.get("inputs",inputs_);
      _map.getPtr("canvas",[&](Id const& _id) {
          return setCanvas(_id);
      });
      _map.get("mode",mode_);
      _map.get("blendSettings",blendSettings_);
      _map.get("exportSettings",exportSettings_);
  }

  Session::visualizer_type* Session::makeVisualizer() {
    if (!viz_ && QOpenGLContext::currentContext()) {
      viz_.reset(new visualizer_type(*this));
    }
    return visualizer();
  }


  Session::visualizer_type* Session::visualizer() {
    return viz_.get();
  }

  Session::visualizer_type const* Session::visualizer() const {
    return viz_.get();
  }


  bool operator==(Session const& _lhs,Session const& _rhs)
  {
    return
      OMNI_TEST_MEMBER_EQUAL(tunings_) &&
      OMNI_TEST_PTR_MEMBER_EQUAL(mapping_) &&
      OMNI_TEST_MEMBER_EQUAL(inputs_) &&
      OMNI_TEST_PTR_MEMBER_EQUAL(canvas_) &&
      OMNI_TEST_MEMBER_EQUAL(mode_) &&
      OMNI_TEST_MEMBER_EQUAL(scene_) &&
      OMNI_TEST_MEMBER_EQUAL(blendSettings_) &&
      OMNI_TEST_MEMBER_EQUAL(exportSettings_)
      ;
  }
}
