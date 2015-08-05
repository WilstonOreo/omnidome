#ifndef OMNI_PROJ_FREESETUP_H_
#define OMNI_PROJ_FREESETUP_H_

#include <QVector3D>
#include "AngleSetup.h"

namespace omni
{
  namespace proj
  {
    class FreeSetup : 
      public AngleSetup,
      SetupRegistrar<FreeSetup>
    {
    public:
      OMNI_TYPEID("FreeSetup")

      FreeSetup();

      void toStream(QDataStream&) const;
      void fromStream(QDataStream&);
      void setup(Projector&);

      QVector3D pos() const;
      void setPos(QVector3D const&);
      void setPos(float _x, float _y, float _z);
      
    private:
      QVector3D pos_;
    };
  }
}

#endif /* OMNI_PROJ_FREESETUP_H_ */
