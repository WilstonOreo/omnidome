#include <omni/visual/Rectangle.h>

#include <QOpenGLFunctions>

namespace omni
{
  namespace visual
  {
    void Rectangle::draw(float _left, float _right, float _top, float _bottom)
    {
      glBegin(GL_QUADS);
      {
        glTexCoord2f(0.0f,1.0f);
        glVertex2f(_left,_bottom);
        glTexCoord2f(1.0f,1.0f);
        glVertex2f(_right,_bottom);
        glTexCoord2f(1.0f,0.0f);
        glVertex2f(_right,_top);
        glTexCoord2f(0.0f,0.0f);
        glVertex2f(_left,_top);
      }
      glEnd();
    }

    void Rectangle::draw(QRectF const& _rect)
    {
      Rectangle::draw(_rect.left(),_rect.right(),_rect.top(),_rect.bottom());
    }
  }
}
