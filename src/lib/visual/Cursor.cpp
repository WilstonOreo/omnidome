#include <omni/visual/Cursor.h>

namespace omni
{
  namespace visual
  {
    Cursor::Cursor()
    {
    }

    Cursor::~Cursor()
    {
    }
      
    void Cursor::draw() const
    {
    }

    void Cursor::update()
    {
    }

    QPointF Cursor::position() const
    {
      return position_;
    }
    
    void Cursor::setPosition(QPointF _position)
    {
      position_ = _position;
    } 
  }
}
