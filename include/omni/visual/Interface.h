#ifndef OMNI_VISUAL_INTERFACE_H_
#define OMNI_VISUAL_INTERFACE_H_

#include <QOpenGLFunctions>

namespace omni
{
  namespace visual
  {
    /// Basic visual interface
    class Interface 
    {
    public:
      virtual ~Interface() {}
      
      /// Abstract method for visualing
      virtual void draw() const = 0;
    
      /// Update geometry
      inline virtual void update() 
      {
      }

    protected:
      /// glVertex3f from QVector3D
      static void vertex3(QVector3D const&);
      
      /// Draws a line
      static void visualLine(QVector3D const& _from, QVector3D const& _to);
  
      /// glColor4f from QColor
      static void color(QColor _color, float _alpha = 1.0);
    };
  }
}


#endif /* OMNI_VISUAL_INTERFACE_H_ */
