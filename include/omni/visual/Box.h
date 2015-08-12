#ifndef OMNI_VISUAL_BOX_H_
#define OMNI_VISUAL_BOX_H_

#include <omni/visual/Interface.h>
#include <omni/visual/VBO.h>
#include <omni/Vertex.h>
#include <omni/Box.h>

namespace omni
{
  namespace visual
  {
    class Box : public Interface
    {
    public:
      Box(omni::Box const& _box, bool _wireframe = false);
      ~Box();

      void draw() const;

      static void draw(omni::Box const& _box, bool _wireframe = false);

      void update();

    private:
      omni::Box const& box_;
      bool wireframe_ = false;
      std::vector<Vertex> vertices_;
      std::vector<int> indices_;
      VBO vertexVbo_, indexVbo_;
    };
  }
}

#endif /* OMNI_VISUAL_BOX_H_ */
