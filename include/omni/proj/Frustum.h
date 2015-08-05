#ifndef OMNI_PROJ_FRUSTUM_H_
#define OMNI_PROJ_FRUSTUM_H_

#include <QVector3D>

namespace omni
{
  namespace proj
  {
    class Projector;

    struct Frustum
    {
      Frustum(const Projector& _proj);

      QVector3D screenDir(const QVector2D& _screenCoord) const;

      const QVector3D& eye() const;
      const QVector3D& lookAt() const;
      const QVector3D& up() const;
      const QVector3D& side() const;
      const QVector3D& topLeft() const;
      const QVector3D& topRight() const;
      const QVector3D& bottomLeft() const;
      const QVector3D& bottomRight() const;

    private:
      QVector3D eye_;
      QVector3D lookAt_;
      QVector3D up_;
      QVector3D side_;
      QVector3D topLeft_;
      QVector3D topRight_;
      QVector3D bottomLeft_;
      QVector3D bottomRight_;
    };
  }

  using proj::Frustum;
}

#endif /* OMNI_PROJ_FRUSTUM_H_ */
