#include <omni/proj/Frustum.h>

#include <QVector2D>
#include <omni/util.h>
#include <omni/Angle.h>
#include <omni/proj/Projector.h>

namespace omni
{
  namespace proj
  {
    Frustum::Frustum(const Projector& _proj)
    {
      qreal _a = _proj.fov().radians() *0.5;
      qreal _height = tan(_a);
      qreal _width = _height * _proj.aspectRatio();
      eye_ = _proj.pos();
      auto& _m = _proj.matrix();
      topLeft_ = _m * QVector3D(1.0,-_width,_height);
      topRight_ = _m * QVector3D(1.0,_width,_height);
      bottomLeft_ = _m * QVector3D(1.0,-_width,-_height);
      bottomRight_ = _m * QVector3D(1.0,_width,-_height);
      lookAt_ = _m * QVector3D(1.0,0.0,0.0);
      up_      = _m * QVector3D(0.0,0.0,1.0);
      side_    = _m * QVector3D(0.0,1.0,0.0);
    }

    QVector3D Frustum::screenDir(const QVector2D& _screenCoord) const
    {
      return
        mix(mix(topLeft(),topRight(),1.0 - _screenCoord.x()),
            mix(bottomLeft(),bottomRight(),1.0 - _screenCoord.x()),1.0 - _screenCoord.y());
    }


    const QVector3D& Frustum::eye() const
    {
      return eye_;
    }

    const QVector3D& Frustum::lookAt() const
    {
      return lookAt_;
    }

    const QVector3D& Frustum::up() const
    {
      return up_;
    }

    const QVector3D& Frustum::side() const
    {
      return side_;
    }

    const QVector3D& Frustum::topLeft() const
    {
      return topLeft_;
    }

    const QVector3D& Frustum::topRight() const
    {
      return topRight_;
    }

    const QVector3D& Frustum::bottomLeft() const
    {
      return bottomLeft_;
    }

    const QVector3D& Frustum::bottomRight() const
    {
      return bottomRight_;
    }
  }
}
