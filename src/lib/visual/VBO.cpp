#include <omni/visual/VBO.h>

namespace omni
{
  namespace visual
  {
    VBO::VBO() : id_(0)
    {
    }

    VBO::~VBO()
    {
      free();
    }

    void VBO::gen()
    {
      if (!id_)
        glGenBuffers(1, &id_);
    }
      
    void VBO::freeAndGen()
    {
      if (id_) free();

      glGenBuffers(1,&id_);
    }

    void VBO::free()
    {
      if (id_)
        glDeleteBuffers(1,&id_);
    }

    GLuint VBO::id() const
    {
      return id_;
    }
  }
}
