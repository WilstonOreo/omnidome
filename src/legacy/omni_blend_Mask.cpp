
#include <omni/blend/Mask.h>

#include <gex/raster/draw_circle.hpp>
#include <gex/algorithm/for_each.hpp>

#include <omni/Mapping.h>
#include <omni/Session.h>
#include <omni/debug.hpp>

using namespace omni::blend;

Buffer Mask::generateBrush(size_t _bufferDim, Scalar _feather)
{
  Buffer _buf(_bufferDim,_bufferDim);
  Scalar _r = _bufferDim * 0.5;

  Scalar _innerRadius = _feather * (1.0 - _r)  + _r;
  using namespace gex::raster;

  for_each(_buf,strategy::for_each::Pixel(),[&](uint8_t& _pix, size_t _x, size_t _y)
  {
    Point2 _d(_x - _r,_y - _r);
    Scalar _distance = _d.norm();
    Scalar _v = 256.0*(_distance - _innerRadius) / (_r - _innerRadius);
    _pix = clamp(_v,0.0,255.0);
  });
  return _buf;
}

void Mask::generateBrush()
{
  brushBuffer_ = generateBrush(int(brush_size_ * 0.5)+1,feather_);
}

Mask::Mask(Mapping const* _mapping) :
  serializer_type(*this),
  top_width_(0.1),
  bottom_width_(0.0),
  left_width_(0.1),
  right_width_(0.1),
  gamma_(2.0),
  brush_size_(100.0),
  feather_(1.0),
  invert_brush_(false),
  mapping_(_mapping)
{
}

void Mask::changeBrushSize(Scalar _delta)
{
  brush_size_ += _delta;
  if (brush_size_ < 1.0) brush_size_ = 1.0;
  generateBrush();
}

void Mask::invert_brush(bool _invert_brush)
{
  invert_brush_ = _invert_brush;
}

void Mask::clear()
{
  strokeBuffer_.clear();
  update();
}

void Mask::update()
{
  generate();
}

Buffer Mask::generateImage() const
{
  if (!mapping_) return Buffer();
  Buffer _buf(mapping_->width(),mapping_->height());

  using namespace gex::raster;
  for_each(_buf,strategy::for_each::Pixel(),[&](uint8_t& _pix, size_t _x, size_t _y)
  {
    _pix = 255.0 * clamp(borderValue(float(_x) / mapping_->width(),float(_y) / mapping_->height()),0.0,1.0);
  });

  return _buf;

}

void Mask::maskSettings(Scalar _top_width, Scalar _bottom_width, Scalar _left_width, Scalar _right_width, Scalar _gamma, bool _update)
{
  top_width_ = _top_width;
  bottom_width_ = _bottom_width;
  left_width_ = _left_width;
  right_width_ = _right_width;
  gamma_ = _gamma;
  if (_update)
    update();
}

void Mask::brushSettings(Scalar _brush_size, Scalar _feather, bool _invert_brush)
{
  brush_size_=_brush_size;
  feather_=_feather;
  invert_brush_=_invert_brush;
  generateBrush();
  if (feather_ < 0.05) feather_ = 0.05;
}

float Mask::borderValue(float _x, float _y) const
{
  float edgeValue = 1.0;
  if (_x <= left_width_)
  {
    edgeValue *= std::min(_x / left_width_, 1.0f);
  }
  else if (_x >= 1.0f - right_width_)
  {
    edgeValue *= std::min((1.0f - _x) / right_width_, 1.0f);
  }
  if (_y <= top_width_)
  {
    edgeValue *= std::min(_y / top_width_, 1.0f);
  }
  if (_y >= 1.0f - bottom_width_)
  {
    edgeValue *= std::min((1.0f - _y) / bottom_width_, 1.0f);
  }
  return 1.0f - pow(clamp(edgeValue,0.0f,1.0f),gamma_);
}

bool Mask::load(ConfigPath const& _path, Config const& _config)
{
  bool _result = serializer_type::load(_path,_config);
  if (_result) update();
  return _result;
}

void Mask::generate()
{
  strokeBuffer_.resize(mapping_->width(),mapping_->height());
  strokeBuffer_.clear(0);
  generateBrush();
  maskBuffer_ = generateImage();
}

void Mask::stamp(Point2 const& _p)
{
  float r = brush_size()*0.5;
  int size = brushBuffer_.width();

  for (int i = 0; i < size; ++i)
  {
    int _posx = int(i + _p.x() - r*0.5);
    if ((_posx < 0) || (_posx >= mapping_->width())) continue;
    for (int j = 0; j < size; ++j)
    {
      int _posy = int(j + _p.y()-r*0.5);
      if ((_posy < 0) || (_posy >= mapping_->height())) continue;
      float _v = brushBuffer_(i,j) / 255.0;
      int _pix = 0;
      if (invert_brush())
      {
        _pix = strokeBuffer_(_posx,_posy)*_v;
      }
      else
      {
        _pix = 255.0 - 255.0 * _v + strokeBuffer_(_posx,_posy) * _v;
      }
      strokeBuffer_(_posx,_posy) = _pix;
    }
  }
}
