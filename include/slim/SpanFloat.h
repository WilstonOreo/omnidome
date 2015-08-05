#ifndef SLIM_SPANFLOAT_H_
#define SLIM_SPANFLOAT_H_

#include <QSpinBox>
#include "AbstractInputWidget.h"
#include "mixin/Span.hpp"

namespace slim
{
  class SpanFloat :
    public AbstractInputWidget,
    public mixin::Span<SpanFloat,double>
  {
    Q_OBJECT
  public:
    typedef QSpinBox editor_type;
    typedef mixin::Span<SpanFloat,double> mixin_span_type;

    SpanFloat(QWidget* = nullptr);
    SpanFloat(QString const& _label, double _lower, double _upper, double _min, double _max, QWidget* = nullptr);
    ~SpanFloat();

  public slots:
    void setLowerValue(double);
    void setUpperValue(double);
    void setMinimum(double);
    void setMaximum(double);
    void setRange(double,double);
    void setLowerEditorVisible(bool);
    void setUpperEditorVisible(bool);
  signals:
    void lowerChanged();
    void upperChanged();
    void rangeChanged();

  protected:
    void paintEvent(QPaintEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);

    void lowerChangedEvent();
    void upperChangedEvent();
    void rangeChangedEvent();

  private:
    void init();
    double fromPos(double) const;
    double lowerToPos() const;
    double upperToPos() const;

    enum MoveState
    {
      NONE,
      LOWER_MOVING,
      UPPER_MOVING
    } moveState_;
  };
}

#endif /* SLIM_SPANFLOAT_H_ */
