#include <omni/canvas/Planar.h>

#include <QVector2D>

namespace omni
{
  namespace canvas
  {
    Planar::Planar()
    {
    }

    Planar::~Planar()
    {
    }

    void Planar::draw() const
    {
      glPushMatrix();

      glLoadIdentity();
      glMultMatrixf(angles_.matrix().constData());
      glTranslatef(center_.x(),center_.y(),center_.z());
      
      glBegin(GL_QUADS);
      float _h = width_ * 0.5;
      float _w = height_ * 0.5;
      glVertex3f(-_w, -_h, 0.0);
      glVertex3f( _w, -_h, 0.0);
      glVertex3f( _w,  _h, 0.0);
      glVertex3f(-_w,  _h, 0.0);
      glEnd();
      glPopMatrix();
    }

    void Planar::drawAux() const
    {
    }
      
    float Planar::width() const
    {
      return width_;
    }

    void Planar::setWidth(float _width)
    {
      width_=_width;
    }
  
    float Planar::height() const
    {
      return height_;
    }

    void Planar::setHeight(float _height)
    {
      height_=_height;
    }

    EulerAngles& Planar::angles()
    {
      return angles_;
    }

    EulerAngles const& Planar::angles() const
    {
      return angles_;
    }

    QVector3D Planar::center() const
    {
      return center_;
    }

    void Planar::setCenter(QVector3D const& _center)
    {
      center_=_center;
    }
      
    Box Planar::bounds() const
    {
      QVector2D _vec(width_,height_);
      _vec *= 0.5;
      float _radius = _vec.length();
      QVector3D _v(_radius,_radius,_radius);
      return Box(-_v,_v);
    }
      
    void Planar::fromStream(QDataStream& _stream)
    {
      _stream >> width_ >> height_ >> center_ >> angles_;
    }

    void Planar::toStream(QDataStream& _stream) const
    {
      _stream << width_ << height_ << center_ << angles_; 
    }
  }
}
