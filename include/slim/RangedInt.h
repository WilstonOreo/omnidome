#ifndef SLIM_RANGEDINT_H_
#define SLIM_RANGEDINT_H_

#include <QSpinBox>
#include "AbstractInputWidget.h"
#include "mixin/RangedValue.hpp"

namespace slim
{
  class RangedInt :
    public AbstractInputWidget,
    public mixin::RangedValue<RangedInt,int>
  {
    Q_OBJECT
  public:
    typedef QSpinBox editor_type;
    typedef mixin::RangedValue<RangedInt,int> mixin_range_type;

    RangedInt(QWidget* = nullptr);
    RangedInt(QString const& _label, int _value, int _min, int _max, QWidget* = nullptr);
    ~RangedInt();

  public slots:
    void setValue(int);
    void setMinimum(int);
    void setMaximum(int);
    void setRange(int,int);
    void setEditorVisible(bool);
  signals:
    void valueChanged();
    void rangeChanged();

  protected:
    void paintEvent(QPaintEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void wheelEvent(QWheelEvent*);

    void valueChangedEvent();
    void rangeChangedEvent();

  private:
    void init();
    int valueFromPos(double) const;
    int valueToPos() const;

    bool moving_;
  };
}

#endif /* SLIM_RANGEDINT_H_ */
