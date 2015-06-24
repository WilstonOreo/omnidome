#include <omni/mapping/Cylindrical.h>

#include <omni/util.h>

namespace omni
{
  namespace mapping
  {
    Cylindrical::Cylindrical(input::List const& _list) :
      Rotatable(_list)
    {
    }

    Cylindrical::~Cylindrical()
    {
    }
 
    IdSet Cylindrical::inputIDs() const
    {
      return IdSet({ "side", "top", "bottom" });
    }
      
    void Cylindrical::bindShader()
    {
      Rotatable::bindShader();
      this->shader_->setUniformValue("height",GLfloat(height_));
      this->shader_->setUniformValue("radius",GLfloat(radius_));
    }

    MappingMode Cylindrical::mode() const
    {
      return CYLINDRICAL;
    }

    qreal Cylindrical::radius() const
    {
      return radius_;
    }

    void Cylindrical::setRadius(qreal _radius)
    {
      radius_=_radius;
    }

    qreal Cylindrical::height() const
    {
      return height_;
    }

    void Cylindrical::setHeight(qreal _height)
    {
      height_=_height;
    }
      
    void Cylindrical::fromStream(QDataStream& _stream)
    {
      Rotatable::fromStream(_stream);
      _stream >> radius_ >> height_;
    }

    void Cylindrical::toStream(QDataStream& _stream) const
    {
      Rotatable::toStream(_stream);
      _stream << radius_ << height_;
    }
    
    bool operator==(Cylindrical const& _lhs, Cylindrical const& _rhs)
    {
      const Rotatable& _blhs(_lhs);
      const Rotatable& _brhs(_rhs);
      return (_blhs == _brhs) && 
        OMNI_TEST_MEMBER_EQUAL(radius_) &&
        OMNI_TEST_MEMBER_EQUAL(height_);
    }
  }
}

