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

#ifndef OMNI_WARPGRID_H_
#define OMNI_WARPGRID_H_

#include <array>
#include <set>
#include <QVector2D>
#include <omni/WarpPoint.h>

namespace omni {
  /**@brief A warp grid is a 2D bezier grid with MxN points
     @detail Default size 6x6 points. Selected points are also stored:
   **/
  class OMNI_EXPORT WarpGrid {
    public:
      enum class Interpolation {
        BICUBIC,
        LINEAR
      };

      /// Default constructor
      WarpGrid();

      /// Resets all points to form a regular grid
      void reset();

      /// Resize grid with given horizontal and vertical resolution
      void resize(int _horz,
                  int _vert);

      /// Return vertical resolution
      int           vertical() const;

      /// Return horizontal resolution
      int           horizontal() const;

      /// Return interpolation type (BICUBIC is default)
      Interpolation interpolation() const;

      /// Interpolation value
      void setInterpolation(Interpolation);

      /// Returns true if all warp points are in regular position
      bool                       isReset() const;

      /// Return true if warp grid has changed
      bool                       hasChanged() const;

      /// Select all points
      void                       selectAll();

      /// Select nearest point, does not clear selection, returns pointer to
      // nearest
      WarpPoint                * selectNearest(const QPointF& _p);

      /// Clear selection
      void                       selectNone();

      /// Returns pointer set of selected points
      std::set<WarpPoint *>      getSelected();

      /// Returns pointer set of selected points (const version)
      std::set<WarpPoint const *>getSelected() const;

      /// Get point with x and y index
      WarpPoint                * getPoint(int x,
                                          int y);

      /// Get point with x and y index (const version)
      WarpPoint const* getPoint(int x,
                                int y) const;

      /// Return position of warp point
      QVector2D getWarpPointPos(int x,
                                int y) const;

      /// Return interpolated position of warp point x, y
      QVector2D getWarpPointPos(int x,
                                int y,
                                float u,
                                float v) const;

      /// Return texture coordinate on x,y index
      QVector2D getTexCoord(int _x,
                            int _y) const;

      /// Return const reference to warp points
      std::vector<WarpPoint>const& points() const;

      /// Test for equality (is equal if all warp points are equal
      friend bool                  operator==(WarpGrid const&,
                                              WarpGrid const&);

    private:
      /// Return index to nearest point
      size_t getNearest(const QPointF& _p) const;

      typedef std::array<QVector2D, 4>array4_type;

      /// Interpolate four points
      QVector2D cubicInterpolate(const array4_type& _points,
                                 float t) const;

      int  horizontal_             = 4;
      int  vertical_               = 4;
      bool hasChanged_             = true;
      Interpolation interpolation_ = Interpolation::BICUBIC;
      std::vector<WarpPoint> points_;
  };
}

/// Serialize omni::WarpGrid to stream
QDataStream& operator<<(QDataStream& _os,
                        const omni::WarpGrid& _p);

/// Deserialize omni::WarpGrid from stream
QDataStream& operator>>(QDataStream& _is,
                        omni::WarpGrid& _p);

#endif /* OMNI_WARPGRID_H_ */
