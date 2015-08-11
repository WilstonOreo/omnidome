#ifndef OMNI_MAPPING_FISHEYE_H_
#define OMNI_MAPPING_FISHEYE_H_

#include "Rotatable.h"

namespace omni
{
  namespace mapping
  {
    class Fisheye : 
      public Rotatable,
      Registrar<Fisheye>
    {
    public:
      OMNI_TYPEID("Fisheye")

      Fisheye();
      ~Fisheye();

      MappingMode mode() const;

      void bind(OutputMode = MAPPED_INPUT);

      qreal stretch() const;
      void setStretch(qreal _stretch);
      
      void fromStream(QDataStream&);
      void toStream(QDataStream&) const;

    private:
      qreal stretch_ = 0.0;
    };
  }
}

#endif /* OMNI_MAPPING_FISHEYE_H_ */
