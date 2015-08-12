#ifndef OMNI_CANVAS_BOX_H_
#define OMNI_CANVAS_BOX_H_

#include <omni/canvas/Envelope.h>
#include <omni/visual/Box.h>

namespace omni
{
  namespace canvas
  {
    /**@brief A box-shaped canvas. 
      *@detail Can be used for rooms or boxes
     **/
    class Box : 
      public Envelope,
      Registrar<Box>
    {
    public:
      OMNI_TYPEID("Box")
  
      /// Default constructor
      Box();

      /// Destructor
      ~Box();

      /// Draw box
      void draw() const;

      /// Generate box vertices
      void update();

      /// Return size of the box
      QVector3D size() const;

      /// Set size of the box
      void setSize(QVector3D const&);

      /// Deserialize from stream
      void fromStream(QDataStream&);
      
      /// Serialize to stream
      void toStream(QDataStream&) const;
    
    private:
      visual::Box vizBox_;
    };
  }
}


#endif /* OMNI_CANVAS_BOX_H_ */
