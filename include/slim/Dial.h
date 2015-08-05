#ifndef FUTON_UI_DIAL_H_
#define FUTON_UI_DIAL_H_

#include <QWidget>
#include <QLabel>
#include <slim/ColorSet.h>
#include <slim/mixin/RangedValue.hpp>

namespace slim
{
  class Dial :
    public QWidget,
    public mixin::RangedValue<Dial,double>
  {
    Q_OBJECT
  public:
    typedef mixin::RangedValue<Dial,double> mixin_range_type;
    typedef typename mixin_range_type::value_type value_type;

    Dial(QWidget* = nullptr);
    Dial(value_type _value, value_type _minimum, value_type _maximum,QWidget* = nullptr);
    ~Dial();

    QString const& suffix() const;

    QChar letter() const;

    ColorSet const& colorSet() const;

    bool isMoving() const;
    bool showTicks() const;

  public slots:
    void setShowTicks(bool);
    void setSuffix(QString const&);
    void setLetter(QChar);

    void setSingleStep(double);
    void setPageStep(double);
    void setMinimum(double);
    void setMaximum(double);
    void setRange(double,double);
    void setValue(int);
    void setValue(double);

  signals:
    void valueChanged();
    void rangeChanged();

  protected:
    void resizeEvent(QResizeEvent*);
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);

    void valueChangedEvent();
    void rangeChangedEvent();

  private:
    bool isMoving_ = false;
    bool showTicks_ = true;
    QChar letter_ = 'X';
    QString suffix_;
    QLabel* valueLabel_;
    ColorSet colorSet_;

    void init();
    QPointF pos_;

    double radius() const;
    double getValue(QPoint const&);
    void paintTick(QPainter&, double _value, double _length);
  };
}


#endif /* FUTON_UI_DIAL_H_ */
