#ifndef OMNI_MAPPING_CYLINDRICAL_H_
#define OMNI_MAPPING_CYLINDRICAL_H_

#include "Rotatable.h"

#include <QOpenGLShaderProgram>

namespace omni
{
  namespace mapping
  {
    class Cylindrical : public Rotatable
    {
    public:
      OMNI_TYPEID("Cylindrical")
      
      Cylindrical(input::List const& _list);
      ~Cylindrical();

      IdSet inputIDs() const;

      void bindShader();

      MappingMode mode() const;

      /// Returns radius of cylinder (default = 0.5)
      qreal radius() const;

      /// Sets radius of cylinder
      void setRadius(qreal);

      /// Return height of cylinder (default = 1.0) 
      qreal height() const;

      /// Set height of cylinder
      void setHeight(qreal);

      void fromStream(QDataStream&);
      void toStream(QDataStream&) const;
      
      friend bool operator==(Cylindrical const&,Cylindrical const&);
    
    private:
      qreal radius_ = 0.5, height_ = 1.0;
    };
  }
}

OMNI_DECL_STREAM_OPERATORS(omni::mapping::Cylindrical)

#endif /* OMNI_MAPPING_CYLINDRICAL_H_ */
