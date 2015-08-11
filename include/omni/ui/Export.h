#ifndef OMNI_UI_EXPORT_H_
#define OMNI_UI_EXPORT_H_

#include <memory>
#include <QWidget>
#include <omni/RenderOptions.h>

namespace omni
{
  class Session;

  namespace ui
  {
    namespace Ui
    {
      class Export;
    }

    class Export : public QWidget
    {
      Q_OBJECT
    public:
      Export(QWidget* = nullptr);
      Export(Session* _session, QWidget* = nullptr);
      ~Export();

      Session const* session() const;
      void setSession(Session* _session);
      
    public slots:
      void exportToFile();

    private:
      omni::RenderOptions getRenderOptions();

      Session* session_ = nullptr;
      std::unique_ptr<Ui::Export> ui_;
    };
  }
}

#endif /* OMNI_UI_EXPORT_H_ */
