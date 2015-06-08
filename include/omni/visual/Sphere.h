#ifndef OMNI_VISUAL_SPHERE_H_
#define OMNI_VISUAL_SPHERE_H_

#include <omni/Vertex.h>
#include <omni/visual/Interface.h>

namespace omni
{
  namespace visual
  {
    /// Draw Handler for visualing a centered sphere 
    class Sphere : public Interface
    {
    public:
      Sphere(qreal _radius = 1.0);
      ~Sphere();

      qreal radius() const;
      void setRadius(qreal);

      QVector3D scale() const;
      void setScale(QVector3D const&);

      /// Return number of stacks (in Z direction)
      int stacks() const;
      
      /// Set number of stacks
      void setStacks(int _stacks);

      int slices() const;
      void setSlices(int _slices);

      void draw() const;

      void update();

    private:
      void generateCone(float _top, float _bottom, float _radius);
      void generateStack(
          float _top, float _bottom,
          float _topRadius, float _bottomRadius);

      int stacks_ = 64;
      int slices_ = 128;

      float top_ = -1.0; 
      float bottom_ = 1.0;

      qreal radius_ = 1.0;
      QVector3D scale_;

      std::vector<Vertex> vertices_;
      std::vector<int> indices_;
    };
  }
}

#endif /* OMNI_VISUAL_SPHERE_H_ */
