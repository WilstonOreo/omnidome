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

#include <omni/Session.h>

#include <QDataStream>

#include <omni/util.h>

#include <omni/proj/FreeSetup.h>
#include <omni/proj/PeripheralSetup.h>
#include <omni/proj/RingArray.h>

#include <omni/Renderer.h>

namespace omni
{
  Session::Session() : screenSetup_(this)
  {
    // Register Projector Setups
    {
      using namespace proj;
      SetupFactory::reg<FreeSetup>();
      SetupFactory::reg<PeripheralSetup>();

      MultiSetupFactory::reg<RingArray>();
    }
    // END Register Projector Setups
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
    mapping_.reset(MappingFactory::create(_id));
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
    canvas_.reset(canvas::Factory::create(_id));
    return canvas();
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

  Session::Mode Session::mode() const
  {
    return mode_;
  }

  void Session::setMode(Mode _mode)
  {
    mode_=_mode;
  }

  Session::BlendMode Session::blendMode() const
  {
    return blendMode_;
  }

  void Session::setBlendMode(BlendMode _blendMode)
  {
    blendMode_ = _blendMode;
  }

  float Session::blendMaskInputOpacity() const
  {
    return blendMaskInputOpacity_;
  }

  float Session::sceneSize() const {
      return sceneSize_;
  }

  void Session::setSceneSize(float _size) {
      sceneSize_ = _size;
  }

  void Session::setBlendMaskInputOpacity(float _blendMaskInputOpacity)
  {
    blendMaskInputOpacity_ = _blendMaskInputOpacity;
  }

  bool Session::hasOutput() const
  {
    return inputs().current() && canvas() && mapping();
  }

  /// Export calibration data of session to a file
  void Session::renderToFile(QString const& _filename, RenderOptions const& _options)
  {
    Renderer _renderer(*this,_options);
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

  bool operator==(Session const& _lhs,Session const& _rhs)
  {
    return
      OMNI_TEST_MEMBER_EQUAL(tunings_) &&
      OMNI_TEST_PTR_MEMBER_EQUAL(mapping_) &&
      OMNI_TEST_MEMBER_EQUAL(inputs_) &&
      OMNI_TEST_PTR_MEMBER_EQUAL(canvas_);
  }
}

QDataStream& operator<<(QDataStream& _stream, omni::Session const& _session)
{
  using namespace omni::util;

  _stream << _session.tunings();

  serializePtr(_stream,_session.mapping());

  _stream << _session.inputs();

  serializePtr(_stream,_session.canvas());
  return _stream;
}

QDataStream& operator>>(QDataStream& _stream, omni::Session& _session)
{
  using namespace omni::util;
  _stream >> _session.tunings();

  deserializePtr(_stream,[&](omni::Id const& _id) ->
      omni::mapping::Interface*
  {
    return _session.setMapping(_id);
  });

  _stream >> _session.inputs();

  deserializePtr(_stream,[&](omni::Id const& _id) ->
      omni::canvas::Interface*
  {
    return _session.setCanvas(_id);
  });

  return _stream;
}
