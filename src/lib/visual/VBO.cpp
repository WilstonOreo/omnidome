#include <omni/visual/VBO.h>

#include <omni/visual/util.h>

namespace omni
{
  namespace visual
  {
    VBO::VBO(QOpenGLContext* _context) : 
      id_(0)
    {
    }

    VBO::~VBO()
    {
      free();
    }

    void VBO::gen()
    {
      with_current_context([this](QOpenGLFunctions& _)
      {
        if (!id_)
          _.glGenBuffers(1, &id_);
      });
    }
      
    void VBO::freeAndGen()
    {
      with_current_context([this](QOpenGLFunctions& _)
      {
        if (id_) free();
        _.glGenBuffers(1,&id_);
      });
    }

    void VBO::free()
    {
      with_current_context([this](QOpenGLFunctions& _)
      {
        if (id_)
          _.glDeleteBuffers(1,&id_);
      });
    }

    GLuint VBO::id() const
    {
      return id_;
    }
  }
}
