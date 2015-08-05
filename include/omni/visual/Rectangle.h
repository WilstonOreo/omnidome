#ifndef OMNI_VISUAL_RECTANGLE_H_
#define OMNI_VISUAL_RECTANGLE_H_

#include <QRectF>

namespace omni
{
  namespace visual
  {
    /// Visualizer helper class for drawing a rectangle in an OpenGL context
    struct Rectangle
    {
      /// Draw rectangle from border coordinates
      static void draw(float _left = -0.5, float _right = 0.5, float _top = 0.5, float _bottom = -0.5);
 
      /// Draw rectangle from given QRectF
      static void draw(QRectF const&);
    
      /// Draw rectangle from border coordinates, flipped on y axis version
      static void drawFlipped(float _left = -0.5, float _right = 0.5, float _top = 0.5, float _bottom = -0.5);
    
      /// Draw rectangle from given QRectF, flipped on y axis version
      static void drawFlipped(QRectF const&);
    };
  }
}

#endif /* OMNI_VISUAL_RECTANGLE_H_ */
