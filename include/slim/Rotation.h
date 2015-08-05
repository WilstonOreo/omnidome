#ifndef SLIM_ROTATION_H_
#define SLIM_ROTATION_H_

#include "Dial.h"

namespace slim
{
  class Rotation : public QWidget
  {
    Q_OBJECT
  public:
    Rotation(QWidget* = nullptr);
    Rotation(double _x, double _y, double _z, QWidget* = nullptr);
    ~Rotation();

    double x() const;
    double y() const;
    double z() const;

  public slots:
    void setX(double);
    void setY(double);
    void setZ(double);

  signals:
    void xChanged();
    void yChanged();
    void zChanged();
  
  private:

    void init(double,double,double);
    Dial* x_;
    Dial* y_;
    Dial* z_;
  };
}


#endif /* SLIM_ROTATION_H_ */
