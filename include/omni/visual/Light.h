#ifndef OMNI_VISUAL_LIGHT_H_
#define OMNI_VISUAL_LIGHT_H_

#include <array>
#include <QColor>
#include <QOpenGLFunctions>
#include "Tracker.h"

namespace omni
{
  namespace visual
  {
    /// A light with position and phong shader attributes
    struct Light : Tracker
    {
      Light( const Tracker& _tracker = Tracker(),
          qreal const& _intensity = 1.0);
      
      Light( const Tracker& _tracker,
             const QColor& _ambient,
             const QColor& _diffuse,
             const QColor& _specular,
             const qreal& _intensity = 1.0);


      /// Setup light in OpenGL
      void setup(GLuint _index = GL_LIGHT0);

      QColor ambient() const;
      void setAmbient(QColor);

      QColor diffuse() const;
      void setDiffuse(QColor);
      
      QColor specular() const;
      void setSpecular(QColor);

      qreal intensity() const;
      void setIntensity(qreal);

    private:
      typedef std::array<GLfloat,4> param_type;
      param_type colorParam(QColor) const;
      param_type eye4() const;

      QColor ambient_, diffuse_, specular_;
      qreal intensity_;
    };
  }
}

#endif /* OMNI_VISUAL_LIGHT_H_ */

