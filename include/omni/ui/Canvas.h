#ifndef OMNI_UI_CANVAS_H_
#define OMNI_UI_CANVAS_H_

#include <memory>
#include <QGroupBox>

namespace omni
{
  class Session;
  
  namespace ui
  {
    namespace Ui
    {
      class Canvas;
    }

    class Canvas : public QGroupBox
    {
      Q_OBJECT
    public:
      Canvas(QWidget* = nullptr);
      ~Canvas();

      Session const* session() const;
      void setSession(Session* _session);
      
    private:
      Session* session_ = nullptr;
      std::unique_ptr<Ui::Canvas> ui_;
    };
  }
}

#endif /* OMNI_UI_CANVAS_H_ */
