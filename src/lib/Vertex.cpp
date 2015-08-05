#include <omni/Vertex.h>

namespace omni
{
  Vertex::Vertex()
  {
  }

  Vertex::Vertex(
    const QVector3D& _pos,
    const QVector3D& _normal,
    const QVector2D& _texCoord) :
    texCoord_(_texCoord),
    normal_(_normal),
    pos_(_pos) 
  {
  }

  void Vertex::setPos(QVector3D const& _pos)
  {
    pos_=_pos;
  }

  QVector3D& Vertex::pos()
  {
    return pos_;
  }

  QVector3D const& Vertex::pos() const
  {
    return pos_;
  }
  
  void Vertex::setNormal(QVector3D const& _normal)
  {
    normal_=_normal;
  }

  QVector3D& Vertex::normal()
  {
    return normal_;
  }

  QVector3D const& Vertex::normal() const
  {
    return normal_;
  }

  void Vertex::setTexCoord(QVector2D const& _texCoord)
  {
    texCoord_=_texCoord;
  }

  QVector2D& Vertex::texCoord()
  {
    return texCoord_;
  }

  QVector2D const& Vertex::texCoord() const
  {
    return texCoord_;
  }


}
