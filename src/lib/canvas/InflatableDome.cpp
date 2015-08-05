#include <omni/canvas/InflatableDome.h>

namespace omni
{
  namespace canvas
  {
    InflatableDome::InflatableDome() 
    {
    }

    InflatableDome::~InflatableDome()
    {
    }

    void InflatableDome::draw() const
    {

    }
  
    void InflatableDome::update()
    {
    }

    float InflatableDome::squeeze() const
    {
      return squeeze_;
    }
    
    void InflatableDome::setSqueeze(float _squeeze)
    {
      if (_squeeze <= 0.1) _squeeze = 0.1;
      if (_squeeze > 10.0) _squeeze = 10.0;
      squeeze_ = _squeeze;
    }
  }
}
