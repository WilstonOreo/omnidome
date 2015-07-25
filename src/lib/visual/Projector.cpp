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
        _.glLineWidth(1.0);
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

    void Projector::drawHalo() const
    {
      glPushMatrix(); 
      {
        glLoadIdentity();
        // Apply matrix to OpenGL
        glMultMatrixf(proj_.matrix().data());
        glBegin(GL_TRIANGLE_FAN);
        Interface::color(color_,0.15);
        this->vertex3(eye_);
        Interface::color(color_,0.0);
        this->vertex3(topLeft_*3.0);
        this->vertex3(topRight_*3.0);
        this->vertex3(bottomRight_*3.0);
        this->vertex3(bottomLeft_*3.0);
        glEnd();
      }
      glPopMatrix();
    }
  }
}
