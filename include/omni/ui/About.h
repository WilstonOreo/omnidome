#ifndef OMNI_UI_ABOUT_H_
#define OMNI_UI_ABOUT_H_

#include <QDialog>
#include <memory>

namespace omni
{
  namespace ui
  {
    namespace Ui
    {
      class About;
    }

    class About : public QDialog
    {
      Q_OBJECT
    public:
      About(QWidget* _parent = nullptr);
      ~About();

      std::unique_ptr<Ui::About> ui_;
    };
  }
}

#endif /* OMNI_UI_ABOUT_H_ */
