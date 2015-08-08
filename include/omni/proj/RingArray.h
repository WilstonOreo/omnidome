#ifndef OMNI_PROJ_RINGARRAY_H_
#define OMNI_PROJ_RINGARRAY_H_

#include <omni/proj/Template.h>
#include <omni/Angle.h>

namespace omni
{
  namespace proj
  {
    class RingArray : 
      public Template,
      private TemplateRegistrar<RingArray>
    {
    public:
      OMNI_TYPEID("RingArray");

      RingArray();
      ~RingArray();

      int numberOfProjectors() const;
      void setNumberOfProjectors(int);

      float distanceCenter() const;
      void setDistanceCenter(float);

      float towerHeight() const;
      void setTowerHeight(float);

      Angle pitch() const;
      void setPitch(Angle const&);
      
      Angle yaw() const;
      void setYaw(Angle const&);
      
      Angle fov() const;
      void setFov(Angle const&);

      std::vector<Projector> projectors() const;
      

    private:
      int numberOfProjectors_ = 4;
      float distanceCenter_ = 4.0;
      float towerHeight_ = 0.0;
      Angle pitch_, yaw_;
      Angle fov_;
    };
  }
}

#endif /* OMNI_PROJ_RINGARRAY_H_ */
