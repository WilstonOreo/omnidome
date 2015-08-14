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

#include <omni/Mapping.h>
#include <omni/canvas.hpp>
#include <omni/blend/Mask.h>
#include <omni/warp/Grid.h>

#include <omni/ui/util.hpp>

#include <tbd/serialize_unique_ptr.h>
#include <omni/calibration.hpp>

#include <omni/debug.hpp>
#include <omni/assert.hpp>
#include <omni/proj/Setup.hpp>

#include <omni/proj/Single.h>
#include <omni/proj/RingArray.h>
#include <omni/proj/RingArrayOneTop.h>

using namespace omni;

Image4ub Session::loadImage(const std::string& _filename)
{
  Image4ub _image;
  QImage _qImage(QString().fromStdString(_filename));
  adapt(_qImage,_image);
  return _image;
}

Session::Session() :
  mode_(CANVAS_PROJECTORS),
  separate_(Separate::SCREENS),
  mapMode_(MapMode::NONE),
  generateShader_(true),
  testImage_(nullptr),
  canvas_(nullptr)
{
  testImage_.reset(new Image4ub(loadImage(":/rc/spherical.jpg")));
}

void Session::clear()
{
  mappings_.clear();
}

void Session::exportSession(const std::string& _filename) const
{
  OMNI_DBG("Number of mappings: " << numMappings())

  std::map<Mapping const*,Image4f> _images;
  for (auto& _mapping : mappings_)
  {
    _images[_mapping.get()] = calibration::generate(_mapping.get(),canvas_.get(),mapMode_);
  }

  calibration::write_images(_images,_filename,separate_);

  if (generateShader())
  {
    calibration::write_shader(_filename,mapMode_);
  }
}

Session::image_type* Session::addImage(std::string const& _filename, MapMode _mapMode)
{
  if (images_.count(_filename) > 0) return nullptr;
  images_.insert({_filename,ImageWithMapMode(loadImage(_filename),_mapMode)});
  return &images_[_filename].image_;
}

Session::image_type* Session::getImage(std::string const& _filename)
{
  if (images_.count(_filename) == 0) return nullptr;
  return &images_[_filename].image_;
}

Session::image_type const* Session::getImage(std::string const& _filename) const
{
  if (images_.count(_filename) == 0) return nullptr;
  return &images_.at(_filename).image_;
}

MapMode Session::getMapMode(std::string const& _filename) const
{
  if (images_.count(_filename) == 0) return MapMode::NONE;
  return images_.at(_filename).mapMode_;
}

void Session::setMapMode(std::string const& _filename, MapMode _mapMode)
{
  if (images_.count(_filename) == 0) return;
  images_[_filename].mapMode_ = _mapMode;
}


bool Session::removeImage(std::string const& _filename)
{
  return images_.erase(_filename) > 0;
}

bool Session::hasCanvas() const
{
  return bool(canvas_);
}

std::vector<Mapping const*> Session::fetchMappings() const
{
  std::vector<Mapping const*> _mappings;
  for (auto& _mapping : mappings_)
  {
    _mappings.push_back(_mapping.get());
  }
  return _mappings;
}


void Session::clearScreens()
{
  screens_.clear();
  for (auto& _mapping : mappings_)
    _mapping->screen(nullptr);
}



void Session::addScreen(size_t _width, size_t _height, size_t _x, size_t _y)
{
  screens_.emplace_back(new proj::Screen(_width,_height,_x,_y,screens_.size()));
}


size_t Session::numMappings() const
{
  return mappings_.size();
}

void Session::changeCanvas(std::string const& _canvasType)
{
  canvas_ = canvas::Registry::create(_canvasType);
}

void Session::load(std::string const& _filename)
{
  clear();
  Config _cfg(_filename);
  load(_cfg);
}

void Session::load(const Config& _cfg)
{
  using namespace tbd;
  size_t _number = _cfg.get<size_t>(ConfigPath("mappings")/"number");
  if (_number == 0) return;
  mappings_.clear();
  mappings_.reserve(_number);
  Serialize<std::unique_ptr<Canvas>>::load(canvas_,"canvas",_cfg);
  Serialize<std::unique_ptr<proj::Setup>>::load(projSetup_,"proj_setup",_cfg);

  for (size_t i = 0; i < _number; ++i)
  {
    mappings_.emplace_back(new Mapping(mappingColors()[i],i));
    Serialize<Mapping>::load(*mappings_.back(),
                             ConfigPath("mappings") / ConfigPath(std::to_string(i)),_cfg);
  }

  int i = 0;
  Serialize<int>::load(i,"mode",_cfg);
  mode_ = Mode(i);
  OMNI_DBG("Mode:" << int(mode_))

  i = 0;
  Serialize<int>::load(i,"map_mode",_cfg);
  mapMode_ = MapMode(i);
  OMNI_DBG("MapMode:" << int(mapMode_))

  i = 0;
  Serialize<int>::load(i,"separate",_cfg);
  separate_ = Separate(i),
  OMNI_DBG("Separate:" << int(separate_))

  Serialize<std::string>::load(exportFilename_,"export_filename",_cfg);
  Serialize<bool>::load(generateShader_,"generate_shader",_cfg);

  images_.clear();

  auto _imageCount = _cfg.get_optional<size_t>(ConfigPath("images") / "number");
  if (_imageCount)
  {
    for (size_t i = 0; i < _imageCount.get(); ++i)
    {
      auto&& _path = ConfigPath("images") / ConfigPath(std::to_string(i));
      auto&& _filename = _cfg.get<std::string>(_path / "filename");
      auto&& _mapMode = MapMode(_cfg.get<int>(_path / "mapmode"));
      addImage(_filename,_mapMode);
    }
  }
}

Config Session::save() const
{
  using namespace tbd;
  Config _cfg;
  _cfg.put(ConfigPath("mappings") / "number",mappings_.size());
  for (size_t i = 0; i < mappings_.size(); ++i)
  {
    Serialize<Mapping>::save(*mappings_[i],
                             ConfigPath("mappings") / ConfigPath(std::to_string(i)),_cfg);
  }
  Serialize<std::unique_ptr<Canvas>>::save(canvas_,"canvas",_cfg);
  Serialize<std::unique_ptr<proj::Setup>>::save(projSetup_,"proj_setup",_cfg);
  Serialize<int>::save(int(mode_),"mode",_cfg);
  Serialize<int>::save(int(mapMode_),"map_mode",_cfg);
  Serialize<int>::save(int(separate_),"separate",_cfg);
  Serialize<std::string>::save(exportFilename_,"export_filename",_cfg);
  Serialize<bool>::save(generateShader_,"generate_shader",_cfg);

  _cfg.put(ConfigPath("images") / "number",images_.size());

  size_t _index = 0;
  for (auto& _image : images_)
  {
    auto&& _path = ConfigPath("images") / ConfigPath(std::to_string(_index));
    _cfg.put(_path / "filename",_image.first);
    _cfg.put(_path / "mapmode",int(_image.second.mapMode_));
    ++_index;
  }
  return _cfg;
}

void Session::save(std::string const& _filename) const
{
  Config&& _cfg = save();
  _cfg.save(_filename);
}

void Session::projectorsInitialPosition()
{
  if (!canvas_) return;

  auto&& _projectors = projSetup_->setup(canvas_.get());
  OMNI_ASSERT(_projectors.size() == mappings_.size());

  /// Reset position of each projector but not its width, height or FOV
  int _index = 0;
  for (auto& _mapping : mappings_)
  {
    auto& _mapProj = _mapping->projector();
    auto& _proj = _projectors[_index];
    _mapProj.adjust(
      _proj.yaw(),
      _proj.distance_center(),
      _proj.tower_height(),
      _proj.shift(),
      _proj.pitch(),
      _proj.roll(),
      _proj.delta_yaw());
    ++_index;
  }
}

void Session::initialize(std::unique_ptr<Canvas> & _canvas,
                         std::unique_ptr<proj::Setup> & _projSetup)
{
  clear();
  canvas_.swap(_canvas);
  projSetup_.swap(_projSetup);

  auto&& _projectors = projSetup_->setup(canvas_.get());

  for (size_t i = 0; i < _projectors.size(); ++i)
  {
    mappings_.emplace_back(new Mapping(mappingColors()[i],i));
    mappings_.back()->projector() = _projectors[i];
  }
}

proj::Screen const* Session::getScreen(id_type _index) const
{
  return (_index >= screens().size()) ? nullptr : screens()[_index].get();
}

Mapping* Session::getMapping(size_t _id)
{
  if (_id >= mappings_.size()) return nullptr;
  return mappings_[_id].get();
}

Mapping const* Session::getMapping(size_t _id) const
{
  if (_id >= mappings_.size()) return nullptr;
  return mappings_[_id].get();
}

std::vector<Color4f> Session::mappingColors()
{
  static std::vector<Color4f> _mappingColors;
  if (_mappingColors.empty())
  {
    _mappingColors.resize(maxMappings());
    size_t i = 0;
    auto add = [&](float r, float g, float b)
    {
      // Color with full saturation
      _mappingColors[i] = Color4f(r,g,b,1.0);
      // Color with less saturation
      _mappingColors[i+8] = Color4f(0.5 + 0.5*r,0.5 + 0.5*g,0.5 + 0.5*b,1.0);
      ++i;
    };
    add(1,0,0); // RED
    add(0,1,0); // GREEN
    add(0,0,1); // BLUE
    add(1,1,0); // YELLOW
    add(0,1,1); // AQUA
    add(1,0,1); // FUCHSIA
    add(1,0.5,0); // ORANGE
    add(0,0.5,1); // AZURE
  }
  return _mappingColors;
}

