#include <omni/visual/Camera.h>

#include <QMatrix4x4>

namespace omni
{
  namespace visual
  {
    Camera::Camera() :
      up_(0.0,0.0,1.0)
    {
    }

    Camera::Camera(const Tracker& _tracker,
                   qreal _near,
                   qreal _far,
                   qreal _fov,
                   const QVector3D& _up) :
      Tracker(_tracker),
      up_(_up),
      near_(_near),
      far_(_far)
    {
    }

    QVector3D Camera::right() const
    {
      return QVector3D::crossProduct(Tracker::direction().vec(),up_);
    }

    void Camera::strafe( qreal _right )
    {
      Tracker::setCenter( Tracker::center() + right().normalized() * _right );
    }

    void Camera::lift( qreal _up )
    {
      QVector3D _orthoUp = QVector3D::crossProduct(right(),Tracker::direction().vec());
      Tracker::setCenter( Tracker::center() + _orthoUp.normalized() * _up );
    }

    QVector3D Camera::up() const
    {
      return up_;
    }

    void Camera::setUp(QVector3D const& _up)
    {
      up_ = _up.normalized();
    }

    qreal Camera::near() const
    {
      return near_;
    }

    void Camera::setNear(qreal _near)
    {
      near_ = _near;
    }

    qreal Camera::far() const
    {
      return far_;
    }

    void Camera::setFar(qreal _far)
    {
      far_ = _far;
    }

    /// Setup perspective camera in OpenGL
    void Camera::setup(qreal _fov, qreal _aspect)
    {
      QMatrix4x4 _m;

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      // perspective projection
      _m.perspective(_fov, _aspect, near(), far());

      _m.lookAt(eye(),center(),up());

      // Apply matrix to OpenGL
      glMultMatrixf(_m.constData());
    }
  }
}
