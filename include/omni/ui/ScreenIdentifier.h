#ifndef OMNI_UI_SCREENIDENTIFIER_H_
#define OMNI_UI_SCREENIDENTIFIER_H_

#include <QWidget>
#include <omni/proj/Screen.hpp>

namespace omni
{
  namespace ui
  {
    class ScreenIdentifier : public QWidget
    {
      Q_OBJECT
    public:
      ScreenIdentifier(size_t _index, size_t _subIndex);
      ~ScreenIdentifier(); 
      
      TBD_PROPERTY_RO(size_t,index)
      TBD_PROPERTY_RO(size_t,subIndex)
    protected:
      void paintEvent ( QPaintEvent * event );
    };
  }
}

#endif /* OMNI_UI_SCREENIDENTIFIER_H_ */
