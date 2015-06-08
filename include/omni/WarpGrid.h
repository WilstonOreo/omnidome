#ifndef OMNI_WARPGRID_H_
#define OMNI_WARPGRID_H_

#include <set>
#include <QVector2D>
#include <omni/WarpPoint.h>

namespace omni
{
  /**@brief A warp grid is a 2D bezier grid with MxN points 
     @detail Default size 6x6 points. Selected points are also stored:
   **/
  class WarpGrid
  {
  public:
    /// Default constructor 
    WarpGrid();

    /// Resets all points to form a regular grid
    void reset();

    /// Resize grid with given horizontal and vertical resolution
    void resize(int _horz, int _vert);

    /// Return vertical resolution
    int vertical() const;

    /// Return horizontal resolution
    int horizontal() const;

    /// Returns true if all warp points are in regular position
    bool isReset() const;

    /// Select all points
    void selectAll();

    /// Select nearest point, does not clear selection, returns pointer to nearest
    WarpPoint* selectNearest(const QPointF& _p);

    /// Clear selection
    void selectNone();

    /// Returns pointer set of selected points
    std::set<WarpPoint*> getSelected();
    
    /// Returns pointer set of selected points (const version)
    std::set<WarpPoint const*> getSelected() const;

    /// Get point with x and y index
    WarpPoint* getPoint(int x, int y);
    
    /// Get point with x and y index (const version)
    WarpPoint const* getPoint(int x, int y) const;

    /// Return texture coordinate on x,y index
    QVector2D getTexCoord(int _x, int _y) const;

    /// Return const reference to warp points
    std::vector<WarpPoint> const& points() const;

  private:
    /// Return index to nearest point
    size_t getNearest(const QPointF& _p) const;

    int horizontal_ = 6;
    int vertical_ = 6;
    std::vector<WarpPoint> points_;
  };
}

/// Serialize omni::WarpGrid to stream
QDataStream& operator<<(QDataStream& _os, const omni::WarpGrid& _p);

/// Deserialize omni::WarpGrid from stream
QDataStream& operator>>(QDataStream& _is, omni::WarpGrid& _p);

#endif /* OMNI_WARPGRID_H_ */
