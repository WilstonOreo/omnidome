#include <tbd/gl.hpp>

#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

float pitch_angle = 0.0;
float yaw_angle = 0.0;

namespace tbd
{
  namespace gl
  {
    template<typename F>
    void cubemap(size_t _width, size_t _height, F f)
    {
      for (int i = 0; i < 6; i++)
      {
        glViewport(_width * i, 0, (GLint)_width , (GLint)_height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(90.0, 1.0,0.001, 100.0);
      
        if (i < 4)
        {
          glRotatef(i * 90,1.0,0.0,0.0);
        } else
        {
          glRotatef(90,0.0,1.0,0.0);
          glRotatef((i == 5 ? 180 : 0.0),0.0,0.0,1.0);
        }
        glRotatef(pitch_angle,1.0,0.0,0.0);
        glRotatef(yaw_angle,0.0,1.0,0.0);

        // realize coordinates
        int c = i*9;
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        f();
      }
    }
  }
}


void timerFunc(int value) {
	glutPostRedisplay();
}
 
 

using namespace std;




void keyboard(int key, int x, int y)
{
  std::cout << pitch_angle << " " << yaw_angle << std::endl;
switch(key) {
case  GLUT_KEY_DOWN:
    pitch_angle -= 0.5;
break;
case GLUT_KEY_UP:
    pitch_angle += 0.5;
break;
case  GLUT_KEY_LEFT:
    yaw_angle -= 0.5;
break;
case GLUT_KEY_RIGHT:
    yaw_angle += 0.5;
break;

default:
break;
}
glutPostRedisplay();
}



void renderScene(void) 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //Mat4 _rot;
  tbd::gl::cubemap(256,256,[&]()
  {
    glPushMatrix();
	  glBegin(GL_QUADS);
      /// Front = Red
		  glColor3f(1.0,0.0,0.0);
      glVertex3f(1.0,1.0,1.0);
		  glVertex3f(1.0,-1.0,1.0);
		  glVertex3f(-1.0,-1.0,1.0);
		  glVertex3f(-1.0,1.0,1.0);
      
      /// Back = Aqua
		  glColor3f(0.0,1.0,1.0);
      glVertex3f(1.0,1.0,-1.0);
		  glVertex3f(1.0,-1.0,-1.0);
		  glVertex3f(-1.0,-1.0,-1.0);
		  glVertex3f(-1.0,1.0,-1.0);
      
      /// Left = Green
		  glColor3f(0.0,1.0,0.0);
      glVertex3f(1.0,1.0,1.0);
		  glVertex3f(1.0,1.0,-1.0);
		  glVertex3f(-1.0,1.0,-1.0);
		  glVertex3f(-1.0,1.0,1.0);
      
      /// Back = Magenta
		  glColor3f(1.0,0.0,1.0);
      glVertex3f(1.0,-1.0,1.0);
		  glVertex3f(1.0,-1.0,-1.0);
		  glVertex3f(-1.0,-1.0,-1.0);
		  glVertex3f(-1.0,-1.0,1.0);
      
      /// Top = Blue
		  glColor3f(0.0,0.0,1.0);
      glVertex3f(1.0,1.0,1.0);
		  glVertex3f(1.0,1.0,-1.0);
		  glVertex3f(1.0,-1.0,-1.0);
		  glVertex3f(1.0,-1.0,1.0);
      
      /// Bottom = Yellow
		  glColor3f(1.0,1.0,0.0);
      glVertex3f(-1.0,1.0,1.0);
		  glVertex3f(-1.0,1.0,-1.0);
		  glVertex3f(-1.0,-1.0,-1.0);
		  glVertex3f(-1.0,-1.0,1.0);
	  glEnd();
    glPopMatrix();
  });

	glutSwapBuffers();
}

int main(int argc, char* argv[])
{
	// init GLUT and create Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(1536,256);
	glutCreateWindow("Lighthouse3D- GLUT Tutorial");


	// register callbacks
	glutDisplayFunc(renderScene);
	glutTimerFunc(10,timerFunc,0);
	glutSpecialFunc(keyboard);
  // enter GLUT event processing cycle
	glutMainLoop();

	

	return 1;
}
