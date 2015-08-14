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

#include <omni/warp/Grid.h>
#include <omni/Mapping.h>

#include <gex/index.hpp>
#include <omni/debug.hpp>

namespace omni
{
  namespace warp
  {
    Point::Point() : 
      pos_(0,0),
      c1_(0,0),
      c2_(0,0),
      selected_(false)
    {
    }
    
    Point::Point(const Point2& _pos) :
      pos_(_pos),
      c1_(0,0),
      c2_(0,0),
      selected_(false)
    {
    }

    Grid::Grid() : use_bezier_(false)
    {
      resize(6,6);
    }
    
    Vec2 Grid::getTexCoord(size_t _x, size_t _y) const
    {
      return Vec2(float(_x)/(horizontal()-1),float(_y)/(vertical()-1));
    }

    void Grid::reset()
    {
      points_.clear();
      points_.reserve(vertical()*horizontal());
      for (size_t y = 0; y < vertical(); ++y)
        for (size_t x = 0; x < horizontal(); ++x)
          points_.emplace_back(getTexCoord(x,y)-Point2(0.5,0.5));
    
      std::cout << vertical() << "\t" << horizontal() << std::endl;
    }

    void Grid::resize(size_t _horz, size_t _vert)
    {
      if (_horz < 2 || _vert < 2) return;
      horizontal_ = _horz;
      vertical_ = _vert;
      reset();
    }

    void Grid::selectAll()
    {
      for (auto& _point : points_)
        _point.selected(true);
    }
      
    bool Grid::load(const ConfigPath& _path, const Config& _cfg)
    {
      bool _update = Serializer<Grid>::load(_path,_cfg);
      resize(horizontal_,vertical_);
      _update |= Serializer<Grid>::load(_path,_cfg);
      return _update;
    }

    Point* Grid::selectNearest(const Point2& _p)
    {
      size_t _nearestIdx = getNearest(_p);
      auto _nearest = (_nearestIdx == -1) ? nullptr : &points_[_nearestIdx];
      return _nearest;
    }
    
    void Grid::selectNone()
    {
      for (auto& _point : points_)
        _point.selected(false);
    }

    std::set<Point*> Grid::getSelected()
    {
      std::set<Point*> _selection;
      for (auto& _point : points_)
        if (_point.selected())
          _selection.insert(&_point);
      return _selection;
    }
    
    std::set<Point const*> Grid::getSelected() const
    {
      std::set<Point const*> _selection;
      for (auto& _point : points_)
        if (_point.selected())
          _selection.insert(&_point);
      return _selection;
    }
      
    Point* Grid::getPoint(int x, int y)
    {
      if (x < 0 || y < 0) return nullptr;
      if (x >= horizontal_ || y >= vertical_) return nullptr;
      return &points_[y*horizontal_+x];
    }

    Point const* Grid::getPoint(int x, int y) const
    {
      if (x < 0 || y < 0) return nullptr;
      if (x >= horizontal_ || y >= vertical_) return nullptr;
      return &points_[y*horizontal_+x];
    }

    std::vector<Triangle> Grid::fetchTriangles() const
    {
      std::vector<Triangle> _triangles;
      for (int y = 0; y < vertical_-1; ++y)
      {
        for (int x = 0; x < horizontal_-1; ++x)
        {
          auto _a = getPoint(x,y)->pos();
          Vec2 _aTex = getTexCoord(x,y);
          auto _b = getPoint(x,y+1)->pos();
          Vec2 _bTex = getTexCoord(x,y+1);
          auto _c = getPoint(x+1,y)->pos();
          Vec2 _cTex = getTexCoord(x+1,y);
          auto _d = getPoint(x+1,y+1)->pos();
          Vec2 _dTex = getTexCoord(x+1,y+1);
          
          _triangles.emplace_back(_a,_aTex,_b,_bTex,_d,_dTex);
          _triangles.emplace_back(_a,_aTex,_c,_cTex,_d,_dTex);
        }
      }
      return _triangles;
    }

    gex::Bounds2 Triangle::bounds() const
    {
      gex::Bounds2 _bounds;
      _bounds.extend(a_);
      _bounds.extend(b_);
      _bounds.extend(c_);
      return _bounds;
    }

    bool Triangle::pointIntersect(const Point2& _p, Vec2* _texCoord) const
    {
      /// Taken from http://www.blackpawn.com/texts/pointinpoly/
      Vec2 _v0 = c_ - a_;
      Vec2 _v1 = b_ - a_;
      Vec2 _v2 = _p - a_;

      Scalar _dot00 = _v0.dot(_v0);
      Scalar _dot01 = _v0.dot(_v1);
      Scalar _dot02 = _v0.dot(_v2);
      Scalar _dot11 = _v1.dot(_v1);
      Scalar _dot12 = _v1.dot(_v2);

      Scalar _invDenom = 1.0 / (_dot00 * _dot11 - _dot01 * _dot01);

      Scalar _u = (_dot11 * _dot02 - _dot01 * _dot12) * _invDenom;
      Scalar _v = (_dot00 * _dot12 - _dot01 * _dot02) * _invDenom;

      float _eps = 0.001;

      bool _result = (_u >= -_eps) && (_v >= -_eps) && (_u + _v <= 1.0 + _eps);
      if (!_texCoord || !_result) return _result;
      *_texCoord = texA_ + _u * (texC_ - texA_) + _v * (texB_ - texA_); 
      return _result;
    }

    std::vector<Vec2> Grid::generateTexCoords(size_t _width, size_t _height) const
    {
      std::vector<Vec2> _texCoords;
      _texCoords.reserve(_height * _width);
      std::vector<Triangle>&& _triangles = fetchTriangles();
     
      gex::RTree2<Triangle const*> _rtree;

      typedef gex::RTree2Node<Triangle const*> node_type;
      
      for (auto& _triangle : _triangles)
      {
        _rtree.insert({_triangle.bounds(),&_triangle});
      }

      // Store a pointer to the last triangle to speed up computation
      Triangle const* _lastTriangle = nullptr;

      std::vector<node_type> _nodes;
      _nodes.reserve(10);

      for (int y = 0; y < _height; ++y)
      {
        for (int x = 0; x < _width; ++x)
        {
          _texCoords.emplace_back(-1,-1);
          Point2 _p(float(x)/_width - 0.5,1.0 - float(y)/_height - 0.5 );
          auto& _texCoord = _texCoords.back();
          if (_lastTriangle)
          {
            if (_lastTriangle->pointIntersect(_p,&_texCoord))
            {
              continue;
            }
          }
          _nodes.clear();
          _rtree.query(gex::index::intersects(_p),std::back_inserter(_nodes));
          if (_nodes.empty()) continue;
          
          for (auto& _node : _nodes)
          {
            auto& _triangle = *_node.second;
            if (_triangle.pointIntersect(_p,&_texCoord))
            {
              _lastTriangle = &_triangle;
              break;
            }
          }
        }
      }
      return _texCoords;
    }

    size_t Grid::getNearest(const Point2& _p) const
    {
      Scalar _nearestDist = gex::inf();
      size_t _nearestIdx = -1;
      for (size_t i = 0; i < points_.size(); ++i)
      {
        auto& _point = points_[i];
        auto _dist = gex::sqrDistance(_p,_point.pos());
        if (_dist < _nearestDist)
        {
          _nearestIdx = i;
          _nearestDist = _dist;
        }
      }
      return _nearestIdx;
    }

    bool Grid::isReset() const
    {
      for (size_t y = 0; y < vertical(); ++y)
        for (size_t x = 0; x < horizontal(); ++x)
        {
          if (getPoint(x,y)->pos() != (getTexCoord(x,y)-Point2(0.5,0.5)))
            return false;
        }

      return true;
    }
  }
}
