#include <omni/visual/Projector.h>

#include <omni/proj/Frustum.h>

namespace omni
{
  namespace visual
  {
    Projector::Projector(const proj::Projector& _proj) :
      proj_(_proj)
    {
    }

    qreal Projector::size() const
    {
      return size_;
    }

    void Projector::setSize(qreal _size) 
    {
      size_=_size;
      update();
    }

    void Projector::update()
    {
      proj::Frustum _f(proj_);
      eye_ = _f.eye();
      topLeft_ = eye_ + size_ * _f.topLeft();
      topRight_ = eye_ + size_ * _f.topRight();
      bottomLeft_ = eye_ + size_ * _f.bottomLeft();
      bottomRight_ = eye_ + size_ * _f.bottomRight();
    }

    void Projector::draw() const
    {
      this->color(color_);
      const_cast<Projector*>(this)->glLineWidth(1.0);
      this->visualLine(eye_,topLeft_);
      this->visualLine(eye_,topRight_);
      this->visualLine(eye_,bottomLeft_);
      this->visualLine(eye_,bottomRight_);
      this->visualLine(topLeft_,topRight_);
      this->visualLine(topLeft_,bottomLeft_);
      this->visualLine(topRight_,bottomRight_);
      this->visualLine(bottomLeft_,bottomRight_);

      if (visualHalo_)
      {
        visualHalo();
      }
    }

    void Projector::visualHalo() const
    {
      glBegin(GL_TRIANGLE_FAN);
      this->color(color_,0.15);
      this->vertex3(eye_);
      this->color(color_,0.0);
      this->vertex3(topLeft_);
      this->vertex3(topRight_);
      this->vertex3(bottomRight_);
      this->vertex3(bottomLeft_);
      glEnd();
    }
  }
}
