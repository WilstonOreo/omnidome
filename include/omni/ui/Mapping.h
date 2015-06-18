#ifndef OMNI_UI_MAPPING_H_
#define OMNI_UI_MAPPING_H_

#include <memory>
#include <QGroupBox>

namespace omni
{
  class Session;

  namespace ui
  {
    namespace Ui
    {
      class Mapping;
    }

    class Mapping : public QGroupBox
    {
      Q_OBJECT
    public:
      Mapping(QWidget* = nullptr);
      ~Mapping();

      Session const* session() const;
      void setSession(Session* _session);
      
    private:
      Session* session_ = nullptr;
      std::unique_ptr<Ui::Mapping> ui_;
    };
  }
}

#endif /* OMNI_UI_MAPPING_H_ */
