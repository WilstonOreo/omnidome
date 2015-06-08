#ifndef OMNI_BOX_H_
#define OMNI_BOX_H_

#include <QVector3D>

namespace omni
{
  /// A box defines a 3D bounding box with a min and max point 
  class Box
  {
  public:
    /// Default constructor
    Box();

    /// Constructor by min and max vector
    Box(QVector3D const& _min, QVector3D const& _max);

    /// Returns the size of the box (= max - min)
    QVector3D size() const;
    
    /// Returns radius of the box (= length of size vector)
    qreal radius() const;

    /// Returns the minimum point
    QVector3D min() const;

    /// Returns the maximum point 
    QVector3D max() const;

    /// Set minimum point of the box
    void setMin(QVector3D const&);
    
    /// Set maximum point of the box
    void setMax(QVector3D const&);

    /// Set minimum and maximum together
    void setMinMax(QVector3D const&, QVector3D const&);

  private:
    /// Check if min < max and swap if necessary
    void validate();

    QVector3D min_, max_;
  };
}


#endif /* OMNI_BOX_H_ */
