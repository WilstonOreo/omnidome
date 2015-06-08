#ifndef OMNI_BLENDMASK_H_
#define OMNI_BLENDMASK_H_

#include <vector>
#include <QRectF>
#include <omni/BlendBrush.h>

namespace omni
{
  namespace proj
  {
    class Tuning;
  }

  /**@brief A blend mask consists of an edge mask and stroke mask with a brush
   * @detail Edge mask and stroke mask are stored in diffrent buffers
   */
  class BlendMask
  {
  public:
    BlendMask(proj::Tuning const& _tuning);

    /// Clears stroke buffer
    void clear();

    /// Updates the mask buffer 
    void update();

    /**@brief Sets rect of blends
      *@detail Clamps rect to borders 0,0,1,1 if its overlapping
     **/
    void setRect(QRectF const&, bool _update = true);

    /// Returns rectangle
    QRectF rect() const;

    /// Returns distance to top edge
    float topWidth() const;

    /// Returns distance to bottom edge
    float bottomWidth() const;

    /// Returns distance to left edge
    float leftWidth() const;

    /// Returns distance to right edge
    float rightWidth() const;

    /// Sets gamma value for edge mask
    void setGamma(float _gamma);

    /// Return gamma value
    float gamma() const;

    /// Return reference blend brush
    BlendBrush& brush();
    
    /// Return reference blend brush (const version)
    BlendBrush const& brush() const;

    /// Stamp on stroke buffer with current brush at position x y
    void stamp(const QPointF& _pos);

    /**@brief Draws a line on stroke buffer 
     * @returns new left over value 
     */
    float drawLine(QPointF const& _p0, QPointF const& _p1, float _leftOver = 0.0);

    /// Returns stroke buffer (read only)
    BlendBuffer const& strokeBuffer() const;
    
    /// Returns mask buffer (read only)
    BlendBuffer const& maskBuffer() const;

  private:
    /// Returns blend value for mask buffer at position x y
    float borderValue(float _x, float _y) const;
    
    proj::Tuning const& tuning_;
    QRectF rect_;
    float gamma_;
    BlendBrush brush_;
    BlendBuffer strokeBuffer_, maskBuffer_;
  };
}

/// Deserialize from stream
QDataStream& operator>>(QDataStream&, omni::BlendMask&);

/// Serialize to stream
QDataStream& operator<<(QDataStream&, omni::BlendMask const&);

#endif /* OMNI_BLEND_MASK_H_ */
