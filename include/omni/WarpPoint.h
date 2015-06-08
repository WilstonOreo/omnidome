#ifndef OMNI_WARPPOINT_H_
#define OMNI_WARPPOINT_H_

#include <QPointF>

namespace omni
{
  /**@brief A Warp defines a bezier point with one position and two handles
  / *@detail Also stores flag if it is selected
  **/
  class WarpPoint
  {
  public:
    /// Default constructor
    WarpPoint();

    /// Constructor from point
    WarpPoint(
        QPointF const& _pos,
        QPointF const& _c1 = QPointF(0.0,0.0),
        QPointF const& _c2 = QPointF(0.0,0.0));

    /// Set new position
    void setPos(QPointF const& _pos);
    
    /// Returns reference to position
    QPointF& pos();

    /// Returns const reference to position
    QPointF const& pos() const;
   
    /// Return x coordinate of position
    float x() const;
    
    /// Return y coordinate of position
    float y() const;

    /// Set first bezier handle 
    void setC1(QPointF const& _c1);
    
    /// Returns reference to first bezier handle
    QPointF& c1();
    
    /// Returns const reference to first bezier handle
    QPointF const& c1() const;
    
    /// Set second bezier handle 
    void setC2(QPointF const& _c2);
    
    /// Returns reference to second bezier handle
    QPointF& c2();
    
    /// Returns const reference to second bezier handle
    QPointF const& c2() const;

    /// Set selection flag
    void setSelected(bool);

    /// Return selection flag
    bool selected() const;

  private:
    QPointF pos_, c1_, c2_;
    bool selected_ = false;
  };
}

/// Serialize omni::WarpPoint to stream
QDataStream& operator<<(QDataStream& _os, const omni::WarpPoint& _p);

/// Deserialize omni::WarpPoint from stream
QDataStream& operator>>(QDataStream& _is, omni::WarpPoint& _p);

#endif /* OMNI_WARPPOINT_H_ */
