#include <omni/proj/AngleSetup.h>

#include <QMatrix4x4>
#include <omni/proj/Projector.h>

namespace omni
{
  namespace proj
  {
    AngleSetup::AngleSetup() 
    {
    }

    AngleSetup::~AngleSetup()
    {
    }

    void AngleSetup::toStream(QDataStream& _stream) const
    {
      _stream << static_cast<EulerAngles const&>(*this);
    }

    void AngleSetup::fromStream(QDataStream& _stream) 
    {
      _stream >> static_cast<EulerAngles&>(*this); 
    }

    void AngleSetup::setup(Projector& _proj)
    {
      _proj.setMatrix(EulerAngles::matrix());
    }
  }
}
