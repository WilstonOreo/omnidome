#include <omni/visual/Interface.h>

#include <QColor>
#include <QVector3D>

namespace omni
{
  namespace visual
  { 
    void Interface::vertex3(QVector3D const& _v)
    {
      glVertex3f(_v.x(),_v.y(),_v.z());
    }

    void Interface::visualLine(QVector3D const& _from, QVector3D const& _to)
    {
      glBegin(GL_LINES);
      vertex3(_from);
      vertex3(_to);
      glEnd();
    }

    void Interface::color(QColor _color, float _alpha)
    {
      glColor4f(_color.red() / 255.0,_color.green() / 255.0,_color.blue() / 255.0,_alpha);
    }
  }
}
