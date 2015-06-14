#include <omni/ui/GLDisplay.h>

#include <QMatrix4x4>
#include <QDebug>

namespace omni
{
  namespace ui
  {
    GLDisplay::GLDisplay(QWidget* _parent) :
      QOpenGLWidget(_parent)
    {
    }

    GLDisplay::~GLDisplay()
    {
      makeCurrent();
      {
      }
      doneCurrent();
    }

    void GLDisplay::initializeGL()
    {
      initializeOpenGLFunctions();
      glEnable(GL_DEPTH_TEST);
      glEnable(GL_POINT_SMOOTH);
      glEnable(GL_LINE_SMOOTH);
      glEnable(GL_TEXTURE);
      glEnable(GL_TEXTURE_2D);

      QImage _image(QString(":/spherical.jpg"));

      _image = _image.mirrored();
      
      glGenTextures(1, &texId_);
      glBindTexture(GL_TEXTURE_2D, texId_);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
          _image.width(), 
          _image.height(), 0, 
          GL_RGBA, GL_UNSIGNED_BYTE, _image.bits());
    }

    void GLDisplay::resizeGL(int _w, int _h)
    {
      _w = _w & ~1;
      _h = _h & ~1;
      glViewport(0, 0, (GLint)_w, (GLint)_h);
      glClearColor(0.0,0.0,0.0,1.0);
    }

    void GLDisplay::paintGL()
    {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glDisable( GL_CULL_FACE );

      /// Setup orthogonal projection
      glMatrixMode(GL_PROJECTION);
      {
        glLoadIdentity();
        QMatrix4x4 _m;
        _m.ortho(-0.5,0.5,-0.5,0.5,-1.0,1.0);
        glMultMatrixf(_m.constData());
      }

      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();

      glBindTexture(GL_TEXTURE_2D, texId_);
      {
        glBegin(GL_QUADS);
        {
          glTexCoord2f(0.0f,0.0f);
          glVertex2f(-0.5f,-0.5f);
          glTexCoord2f(1.0f,0.0f);
          glVertex2f(0.5f,-0.5f);
          glTexCoord2f(1.0f,1.0f);
          glVertex2f(0.5f,0.5f);
          glTexCoord2f(0.0f,1.0f);
          glVertex2f(-0.5f,0.5f);
        }
        glEnd();
      }
    }
  }
}

