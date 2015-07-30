#ifndef OMNI_UI_MAPPING_H_
#define OMNI_UI_MAPPING_H_

#include <memory>
#include <QGroupBox>
#include <omni/ui/SessionWidget.h>

namespace omni
{
  class Session;

  namespace ui
  {
    namespace Ui
    {
      class Mapping;
    }

    /**@brief Widget for selecting a mapping mode and the parameters
     **/
    class Mapping : 
      public QGroupBox,
      public SessionWidget
    {
      Q_OBJECT
    public:
      Mapping(QWidget* = nullptr);
      ~Mapping();
 
    signals:
      void mappingTypeChanged();
      void mappingChanged();

    public slots:
      void mappingTypeSelected(QString const&);
      void sessionParameters();

    private:
      std::unique_ptr<Ui::Mapping> ui_;
    };
  }
}

#endif /* OMNI_UI_MAPPING_H_ */
