#ifndef OMNI_MAPPING_PLANAR_H_
#define OMNI_MAPPING_PLANAR_H_

#include <QVector2D>
#include "Interface.h"


namespace omni
{
  namespace mapping
  {
    class Planar : 
      public Interface,
      Registrar<Planar>
    {
    public:
      OMNI_TYPEID("Planar")
      Planar();

      ~Planar();

      Mode mode() const;
      void bind(OutputMode);

      QVector2D& offset();
      QVector2D const& offset() const;
      void setOffset(QVector2D const&);

      QVector2D& stretch();
      QVector2D const& stretch() const;
      void setStretch(QVector2D const&);

      bool tileHorizontal() const;
      void setTileHorizontal(bool);
      
      bool tileVertical() const;
      void setTileVertical(bool);

      void fromStream(QDataStream&);
      void toStream(QDataStream&) const;

    private:
      QVector2D offset_;
      QVector2D stretch_;
      bool tileHorizontal_, tileVertical_;
    };
  }
}

#endif /* OMNI_MAPPING_PLANAR_H_ */
