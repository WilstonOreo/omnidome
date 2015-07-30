#include <omni/visual/Box.h>

namespace omni
{
  namespace visual
  {
    Box::Box()
    {
    }

    Box::Box(omni::Box const& _box, bool _wireframe) :
      box_(_box),
      wireframe_(_wireframe)
    {
    }

    Box::~Box()
    {
    }

    void Box::draw(omni::Box const& _box, bool _wireframe)
    {
      float x = _box.min().x(), y = _box.min().y(), z = _box.min().z();
      float xs = _box.max().x(), ys = _box.max().y(), zs = _box.max().z();
      GLuint _visualMode = _wireframe ? GL_LINE_LOOP : GL_QUADS;

      glBegin(_visualMode);
      {
        // top side
        glNormal3f(0.0,1.0,0.0);
        glTexCoord2f(0,0);
        glVertex3f(x , ys, z );
        glTexCoord2f(1,0);
        glVertex3f(xs, ys, z );
        glTexCoord2f(1,1);
        glVertex3f(xs, ys, zs);
        glTexCoord2f(0,1);
        glVertex3f(x , ys, zs);
      }
      glEnd();
      glBegin(_visualMode);
      {
        // bottom side
        glNormal3f(0.0,-1.0,0.0);
        glTexCoord2f(0,0);
        glVertex3f(x , y, z );
        glTexCoord2f(1,0);
        glVertex3f(xs, y, z );
        glTexCoord2f(1,1);
        glVertex3f(xs, y, zs);
        glTexCoord2f(0,1);
        glVertex3f(x , y, zs);
      }
      glEnd();
      glBegin(_visualMode);
      {
        // east side
        glNormal3f(0.0,0.0,-1.0);
        glTexCoord2f(0,0);
        glVertex3f(x , y , z);
        glTexCoord2f(1,0);
        glVertex3f(xs, y , z);
        glTexCoord2f(1,1);
        glVertex3f(xs, ys, z);
        glTexCoord2f(0,1);
        glVertex3f(x , ys, z);
      }
      glEnd();
      glBegin(_visualMode);
      // west side
      {
        glNormal3f(0.0,0.0,1.0);
        glTexCoord2f(0,0);
        glVertex3f(x , y ,zs);
        glTexCoord2f(1,0);
        glVertex3f(xs, y ,zs);
        glTexCoord2f(1,1);
        glVertex3f(xs, ys,zs);
        glTexCoord2f(0,1);
        glVertex3f(x , ys,zs);
      }
      glEnd();
      glBegin(_visualMode);
      {
        // north side
        glNormal3f(-1.0,0.0,0.0);
        glTexCoord2f(0,0);
        glVertex3f(x , y , z );
        glTexCoord2f(1,0);
        glVertex3f(x , y , zs);
        glTexCoord2f(1,1);
        glVertex3f(x , ys, zs);
        glTexCoord2f(0,1);
        glVertex3f(x , ys, z );
      }
      glEnd();
      glBegin(_visualMode);
      {
        // south side
        glNormal3f( 1.0,0.0,0.0);
        glTexCoord2f(0,0);
        glVertex3f( xs, y , z );
        glTexCoord2f(1,0);
        glVertex3f( xs, y , zs);
        glTexCoord2f(1,1);
        glVertex3f( xs, ys, zs);
        glTexCoord2f(0,1);
        glVertex3f( xs, ys, z );
      }
      glEnd();
    }

    void Box::draw() const
    {
      draw(box_,wireframe_);
    }
  }
}

