#ifndef OMNI_MAPPING_EQUIRECTANGULAR_H_
#define OMNI_MAPPING_EQUIRECTANGULAR_H_

#include "Rotatable.h"

namespace omni
{
  namespace mapping
  {
    /// Equirectangular spherical mapping
    class Equirectangular : 
      public Rotatable,
      Registrar<Equirectangular>
    {
    public:
      OMNI_TYPEID("Equirectangular")

      Equirectangular(input::List const&); 
      ~Equirectangular();
      
      MappingMode mode() const;

      void bindShader();

      /// Value between 0.0 and 1.0 for top strip position
      qreal stripTop() const;
      
      /// Top border position of strip. Value is clamped between 0.0 and 1.0 
      void setStripTop(qreal _stripTop);
      
      /// Value between 0.0 and 1.0 to for bottom strip position 
      qreal stripBottom() const;
      
      /// Bottom border position of strip. Value is clamped between 0.0 and 1.0 
      void setStripBottom(qreal _stripBottom);

      void fromStream(QDataStream&);
      void toStream(QDataStream&) const;

    private:
      /// Assures that strip bottom is always lower then strip top
      void validate();
      
      qreal stripBottom_ = 0.0, stripTop_ = 1.0;
    };
  }
}

#endif /* OMNI_MAPPING_EQUIRECTANGULAR_H_ */
