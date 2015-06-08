#include <omni/visual/Sphere.h>

#include <omni/visual/util.h>

namespace omni
{
  namespace visual
  {
    Sphere::Sphere(qreal _radius) :
      radius_(_radius)
    {
      update();
    }

    Sphere::~Sphere()
    {
    }

    qreal Sphere::radius() const
    {
      return radius_;
    }

    void Sphere::setRadius(qreal _radius)
    {
      radius_=_radius;
    }

    QVector3D Sphere::scale() const
    {
      return scale_;
    }

    void Sphere::setScale(QVector3D const& _scale)
    {
      scale_ = _scale;
    }

    int Sphere::stacks() const
    {
      return stacks_;
    }

    void Sphere::setStacks(int _stacks)
    {
      stacks_ = _stacks;
      update();
    }

    int Sphere::slices() const
    {
      return slices_;
    }

    void Sphere::setSlices(int _slices)
    {
      slices_ = _slices;
      update();
    }

    void Sphere::draw() const
    {
      glPushMatrix();
      {
        // Move offset
        glScalef(radius_ * scale_.x(),radius_ * scale_.y(),radius_ * scale_.z());

        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_VERTEX_ARRAY);

        glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex),(void*)Vertex::texCoordOffset());
        glNormalPointer(GL_FLOAT, sizeof(Vertex), (void*)Vertex::normalOffset());
        glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (void*)Vertex::posOffset());

        const_cast<Sphere*>(this)->glDrawElements(GL_TRIANGLES,indices_.size(),GL_UNSIGNED_INT,const_cast<int*>(indices_.data()));

        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
      }
      glPopMatrix();
    }

    void Sphere::update()
    {
      vertices_.clear();
      vertices_.reserve(2 * slices_ * stacks_);
      indices_.clear();
      indices_.reserve(6 * slices_ * stacks_);


      // If there are M lines of latitude (horizontal) and
      // N lines of longitude (vertical), then put dots at
      // (x, y, z) = (sin(Pi * m/M) cos(2Pi * n/N), sin(Pi * m/M) sin(2Pi * n/N), cos(Pi * m/M))
      auto stackRadius = [this](size_t index)
      {
        return sin(M_PI * float(index) / stacks_);
      };
      auto stackPos = [this](size_t index)
      {
        return cos(M_PI * float(index) / stacks_);
      };

      /// Top cone
      generateCone(1.0,stackPos(1),stackRadius(1));
      /// Bottom cone
      generateCone(-1.0,stackPos(stacks_ - 1),stackRadius(1));

      for (size_t i = 1; i < stacks_-1; ++i)
      {
        if (stackPos(i) < top_) return;
        if (stackPos(i+1) > bottom_) return;

        generateStack(stackPos(i),stackPos(i+1),
                            stackRadius(i),stackRadius(i+1));
      }
    }

    void Sphere::generateStack(float _top, float _bottom,
                               float _topRadius, float _bottomRadius)
    {
      size_t _startIndex = vertices_.size();
      for (size_t i = 0; i <= slices_; ++i)
      {
        /// Generate vertices
        float _m = 2.0 * M_PI * float(i) / slices_;
        float _cos = cos(_m), _sin = sin(_m);
        QVector3D _topPoint(_cos * _topRadius,_sin * _topRadius,_top);
        QVector3D _bottomPoint(_cos * _bottomRadius,_sin * _bottomRadius,_bottom);
        QVector3D _normalTop(_topPoint.normalized());
        QVector3D _normalBottom(_bottomPoint.normalized());
        vertices_.emplace_back(_topPoint,_normalTop,QVector2D(float(i)/slices_,acos(_normalTop.z()) / M_PI));
        vertices_.emplace_back(_bottomPoint,_normalBottom,QVector2D(float(i)/slices_,acos(_normalBottom.z()) / M_PI));

        /// Top triangle
        indices_.push_back(_startIndex + 2 * i);
        indices_.push_back(_startIndex + 2 * i + 1);
        indices_.push_back(_startIndex + 2 * (i + 1) );

        /// Bottom triangle
        indices_.push_back(_startIndex + 2 * i + 1);
        indices_.push_back(_startIndex + 2 * (i + 1) +1 );
        indices_.push_back(_startIndex + 2 * (i + 1) );
      }
    }

    void Sphere::generateCone(float _top, float _bottom, float _radius)
    {
      size_t _startIndex = vertices_.size();

      QVector3D _p(0,0,_top);
      vertices_.emplace_back(_p,_p.normalized(),QVector2D(0.5,0.0));

      util::for_each_circle_point(slices_,_radius,[&](size_t i, const QPointF& _p2)
      {
        _p = QVector3D(_p2.x(),_p2.y(),_bottom);
        QVector3D _normal = _p.normalized();
        vertices_.emplace_back(_p,_normal,QVector2D(float(i)/slices_,acos(_normal.z()) / M_PI));
        if (_top > _bottom)
        {
          indices_.push_back(_startIndex);
          indices_.push_back(_startIndex+1 + i);
          indices_.push_back(_startIndex+1 + (i + 1) % slices_);
        }
        else
        {
          indices_.push_back(_startIndex+1 + (i + 1) % slices_);
          indices_.push_back(_startIndex+1 + i);
          indices_.push_back(_startIndex);
        }
      });
    }
  }
}
