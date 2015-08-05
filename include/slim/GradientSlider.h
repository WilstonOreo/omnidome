#ifndef SLIM_GRADIENTSLIDER_H_
#define SLIM_GRADIENTSLIDER_H_

#include <QLinearGradient>
#include "mixin/RangedValue.hpp"
#include "Widget.h"

namespace slim
{
  /** A slider with a value and a gradient **/
  class GradientSlider : 
    public Widget,
    private mixin::RangedValue<GradientSlider,double>
  {
    Q_OBJECT
  public:
    typedef mixin::RangedValue<GradientSlider,double> mixin_range_type;

    GradientSlider(QWidget* = nullptr);
    GradientSlider(QString const& _label, QWidget* = nullptr);
    ~GradientSlider();

    QLinearGradient& gradient();
    QLinearGradient const& gradient() const;
    void setGradient(QLinearGradient const&);
 
    using mixin_range_type::value;

    QColor color() const;
    bool showColorOnly() const;

  public slots:
    void setValue(double);
    void setShowColorOnly(bool);
  signals:
    void valueChanged();


  protected:
    void paintEvent(QPaintEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void wheelEvent(QWheelEvent*);

  private:    
    bool showColorOnly_ = false;
    bool moving_ = false;
    
    /// Calculate position from value
    double valueToPos() const;

    /// Calculate position from value
    double valueToPos(double) const;

    /// Get value from x mouse position
    double valueFromPos(double) const;
    
    QLinearGradient gradient_;
  };
}

#endif /* SLIM_GRADIENTSLIDER_H_ */
