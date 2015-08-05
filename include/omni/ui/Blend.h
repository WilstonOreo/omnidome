#ifndef OMNI_UI_BLEND_H_
#define OMNI_UI_BLEND_H_

#include <memory>
#include <QGroupBox>

namespace omni
{
  class Session;
  
  namespace ui
  {
    namespace Ui
    {
      class Blend;
    }

    class Blend : public QGroupBox
    {
      Q_OBJECT
    public:
      Blend(QWidget* = nullptr);
      ~Blend();

      Session const* session() const;
      void setSession(Session* _session);
      
    private:
      Session* session_ = nullptr;
      std::unique_ptr<Ui::Blend> ui_;
    };
  }
}

#endif /* OMNI_UI_BLEND_H_ */
