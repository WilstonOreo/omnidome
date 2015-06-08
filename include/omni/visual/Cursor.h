#ifndef OMNI_VISUAL_CURSOR_H_
#define OMNI_VISUAL_CURSOR_H_

namespace omni
{
  namespace visual
  {
    /**@brief Draw Handler for Cursor
      *@detail
     **/
    class Cursor : public Interface
    {
    public:
      QPointF position() const;
      void setPosition(QPointF);
      
    private:
      QPointF position_;

      bool crossHair_ = false;
      bool circle_ = false;
    };
  }
}



#endif /* OMNI_VISUAL_CURSOR_H_ */
