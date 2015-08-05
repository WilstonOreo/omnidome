#ifndef OMNI_UI_FULLSCREEN_H_
#define OMNI_UI_FULLSCREEN_H_ 

#include <map>
#include <memory>
#include <QWidget>

namespace omni
{
  namespace ui
  {
    /**@brief Widget fills an entire screen
       @detail for triple heads etc, it has several subwidgets
     **/
    class Fullscreen : public QWidget
    {
      Q_OBJECT
    public:
      Fullscreen(const proj::Screen& _screen);
      ~Fullscreen();

      void detach(size_t _subIndex);
      void place(size_t _subIndex, QWidget* _widget);

      /// Remove all widget
      void clear();

      void transparent(bool _t);
      void identify(bool _identify);

      TBD_PROPERTY_REF_RO(proj::Screen,screen)
      TBD_PROPERTY_RO(bool,transparent)
    protected:
      void closeEvent(QCloseEvent* _event);

    private:
      std::map<size_t,QWidget*> widgets_;
    };
  }
}

#endif /* OMNI_UI_FULLSCREEN_H_ */
