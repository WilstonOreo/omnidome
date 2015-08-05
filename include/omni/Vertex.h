#ifndef OMNI_VERTEX_H_
#define OMNI_VERTEX_H_

#include <QVector2D>
#include <QVector3D>

namespace omni
{
  /// A vertex is geometry entiry with a position, normal and texture coordinates
  struct Vertex
  {
    /// Default constructor
    Vertex();

    /// Construct from given position, normal and optional texture coordinates
    Vertex(
        const QVector3D& _pos,
        const QVector3D& _normal,
        const QVector2D& _texCoord = QVector2D()); 

    /// Set new position
    void setPos(QVector3D const& _pos);
    
    /// Return reference to position 
    QVector3D& pos();

    /// Return const reference to position
    QVector3D const& pos() const;
    
    /// Sets new normal (is not normalized!)
    void setNormal(QVector3D const& _normal);
    
    /// Return reference to normal
    QVector3D& normal();

    /// Return const reference to normal
    QVector3D const& normal() const;
   
    /// Set new texture coordinates 
    void setTexCoord(QVector2D const& _texCoord);
    
    /// Return reference to texture coordinates
    QVector2D& texCoord();

    /// Return const reference to texture coordinates
    QVector2D const& texCoord() const;
    
    inline static constexpr size_t texCoordOffset()
    {
      return 0;
    }
    
    inline static constexpr size_t normalOffset()
    {
      return sizeof(QVector2D);
    }

    inline static constexpr size_t posOffset()
    {
      return sizeof(QVector2D) + sizeof(QVector3D);
    }
 
  private:
    QVector2D texCoord_;
    QVector3D normal_;
    QVector3D pos_;
  };
}

#endif /* OMNI_VERTEX_H_ */
