#ifndef SLIM_ROTATIONDIAL_H_
#define SLIM_ROTATIONDIAL_H_

#include <QWidget>
#include <QLabel>
#include <slim/ColorSet.h>
#include <slim/mixin/RangedValue.hpp>

namespace slim
{
  class RotationDial :
    public QWidget,
    public mixin::Range<double>,
    public mixin::Step<double>
  {
    Q_OBJECT
  public:
    typedef mixin::Range<double> mixin_range_type;
    typedef mixin::Step<double> mixin_step_type;

    RotationDial(QWidget* = nullptr);
    RotationDial(double _x, double _y, double _z, QWidget* = nullptr);
    ~RotationDial();

    ColorSet const& colorSet() const;

    double x() const;
    double y() const;
    double z() const;

    QColor xColor() const;
    QColor yColor() const;
    QColor zColor() const;

    bool isMoving() const;
    bool showTicks() const;

  public slots:
    void setShowTicks(bool);

    void setX(double);
    void setY(double);
    void setZ(double);

    void setXColor(QColor);
    void setYColor(QColor);
    void setZColor(QColor);

  signals:
    void xChanged();
    void yChanged();
    void zChanged();

  protected:
    void resizeEvent(QResizeEvent*);
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);

    void valueChangedEvent();

  private:
    enum MoveState
    {
      MOVE_NONE,
      MOVE_X,
      MOVE_Y,
      MOVE_Z
    } moveState_ = MOVE_NONE;

    bool showTicks_ = true;
    QLabel* valueLabel_;
    ColorSet colorSet_;

    void init(double = 0.0,double = 0.0,double = 0.0);
    QPointF pos_;

    double radius() const;

    /// Set value according to move state
    void setValue(double);

    /// Get angle from point
    double getValue(QPoint const&);
    void paintTick(QPainter&, double _inner, double _outer, double _value);

    void paintPie(QPainter&, double _innerRadius, double _outerRadius, double _value, QColor _color);
    void paintPies(QPainter&);
    
    static QString formatDegrees(double);
    static QString moveStateToText(MoveState);


    double x_, y_, z_;
    QColor xColor_ = "#2A76AC";
    QColor yColor_ = "#89CBF9";
    QColor zColor_ = "#E8F0F5";
  };
}


#endif /* FUTON_UI_DIAL_H_ */
