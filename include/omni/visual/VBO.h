#ifndef OMNI_VISUAL_VBO_H
#define OMNI_VISUAL_VBO_H

#include <QOpenGLContext>

namespace omni
{
  namespace visual
  {
    /// A small Vertex Buffer Object class
    struct VBO 
    {
      VBO(QOpenGLContext* = nullptr);
      
      /// Destructor. Frees VBO if necessary
      ~VBO();
        
      /// Generate a new VBO and get the associated ID
      void gen();
      
      /// Free the existing buffer generates a new one
      void freeAndGen();

      /// Free VBO via glDeleteBuffers
      void free();

      /// Return buffer id
      GLuint id() const;

    private:
      GLuint id_;
    };
  }
}

#endif /* OMNI_VISUAL_VBO_H */
