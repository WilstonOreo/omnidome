#ifndef OMNI_VISUAL_BOX_H_
#define OMNI_VISUAL_BOX_H_

#include <omni/visual/Interface.h>
#include <omni/Box.h>

namespace omni
{
  namespace visual
  {
    class Box : public Interface
    {
    public:
      Box();
      Box(omni::Box const& _box, bool _wireframe = false);
      ~Box();

      void draw() const;

      static void draw(omni::Box const& _box, bool _wireframe = false);

    private:
      omni::Box box_;
      bool wireframe_ = false;
    };
  }
}

#endif /* OMNI_VISUAL_BOX_H_ */
