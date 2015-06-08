#include <omni/mapping/Planar.h>

#include <QOpenGLShaderProgram>
#include <QDataStream>

namespace omni
{
  namespace mapping
  {
    Planar::Planar(input::List const& _list) : 
      Interface(_list),
      offset_(0.0,0.0),
      stretch_(1.0,1.0),
      tileHorizontal_(false),
      tileVertical_(false)
    {
    }

    Planar::~Planar()
    {
    }
      
    MappingMode Planar::mode() const
    {
      return PLANAR;
    }

    void Planar::bindShader()
    {
      Interface::bindShader();
      this->shader_->setUniformValue("offset",offset_);
      this->shader_->setUniformValue("stretch",stretch_);
      this->shader_->setUniformValue("tile_horizontal",GLint(tileHorizontal_));
      this->shader_->setUniformValue("tile_vertical",GLint(tileVertical_));
    }
  
    QVector2D& Planar::offset()
    {
      return offset_;
    }

    QVector2D const& Planar::offset() const
    {
      return offset_;
    }
      
    void Planar::setOffset(QVector2D const& _offset)
    {
      offset_=_offset;
    }

    QVector2D& Planar::stretch()
    {
      return stretch_;
    }
    
    QVector2D const& Planar::stretch() const
    {
      return stretch_;
    }

    void Planar::setStretch(QVector2D const& _stretch)
    {
      stretch_ = _stretch;
    }

    bool Planar::tileHorizontal() const
    {
      return tileHorizontal_;
    }

    void Planar::setTileHorizontal(bool _tileHorizontal) 
    {
      tileHorizontal_ = _tileHorizontal;
    }
    
    bool Planar::tileVertical() const
    {
      return tileVertical_;
    }

    void Planar::setTileVertical(bool _tileVertical) 
    {
      tileVertical_ = _tileVertical;
    }

    void Planar::fromStream(QDataStream& _stream)
    {
      Interface::fromStream(_stream);
      _stream >> offset_ >> stretch_ >> tileHorizontal_ >> tileVertical_;
    }

    void Planar::toStream(QDataStream& _stream) const
    {
      Interface::toStream(_stream);
      _stream << offset_ << stretch_ << tileHorizontal_ << tileVertical_;
    }
  }
}
