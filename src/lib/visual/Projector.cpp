#include <omni/visual/Projector.h>

#include <omni/proj/Frustum.h>
#include <omni/visual/util.h>

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
      
    bool Projector::isSelected() const
    {
      return selected_;
    }
    
    void Projector::setSelected(bool _selected)
    {
      selected_ = _selected;
    }
 
    QColor Projector::color() const
    {
      return color_; 
    }

    void Projector::setColor(QColor _color) 
    {
      color_=_color;
    }

    void Projector::update()
    {   
      qreal _a = proj_.fov().radians() *0.5;
      qreal _height = tan(_a);
      qreal _width = _height * proj_.aspectRatio();
      eye_ = QVector3D(0,0,0);
      topLeft_ = QVector3D(1.0,-_width,_height);
      topRight_ = QVector3D(1.0,_width,_height);
      bottomLeft_ = QVector3D(1.0,-_width,-_height);
      bottomRight_ = QVector3D(1.0,_width,-_height);
    }

    void Projector::draw() const
    {
      Interface::color(color_);

      with_current_context([this](QOpenGLFunctions& _)
      {
        _.glLineWidth(selected_ ? 2.0 : 1.0);
      });

      glPushMatrix(); 
      {
        glLoadIdentity();
        // Apply matrix to OpenGL
        glMultMatrixf(proj_.matrix().data());
        this->visualLine(eye_,topLeft_);
        this->visualLine(eye_,topRight_);
        this->visualLine(eye_,bottomLeft_);
        this->visualLine(eye_,bottomRight_);
        this->visualLine(topLeft_,topRight_);
        this->visualLine(topLeft_,bottomLeft_);
        this->visualLine(topRight_,bottomRight_);
        this->visualLine(bottomLeft_,bottomRight_);

      }
      glPopMatrix();
    }

    void Projector::drawPositioning(QVector3D const& _center) const
    {
      Interface::color(color_); 
      with_current_context([this](QOpenGLFunctions& _)
      {
        _.glLineWidth(selected_ ? 4.0 : 1.5);
      });
      auto _p = proj_.pos();
      this->visualLine(_center,QVector3D(_p.x(),_center.y(),_center.z()));
      this->visualLine(
          QVector3D(_p.x(),_center.y(),_center.z()),
          QVector3D(_p.x(),_p.y(),_center.z()));
      this->visualLine(
          QVector3D(_p.x(),_p.y(),_center.z()),_p);
    }

    void Projector::drawHalo() const
    {
      glPushMatrix(); 
      {
        glLoadIdentity();
        // Apply matrix to OpenGL
        glMultMatrixf(proj_.matrix().data());
        glBegin(GL_TRIANGLE_FAN);
        Interface::color(color_,selected_ ? 0.45 : 0.15);
        this->vertex3(eye_);
        Interface::color(color_,0.0);
        float _scale = size_ * 2.0;
        if (selected_) _scale *= 2.0;
        this->vertex3(topLeft_*_scale);
        this->vertex3(topRight_*_scale);
        this->vertex3(bottomRight_*_scale);
        this->vertex3(bottomLeft_*_scale);
        glEnd();
      }
      glPopMatrix();
    }
  }
}
