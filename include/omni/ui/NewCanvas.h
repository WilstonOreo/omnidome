#ifndef OMNI_UI_NEWCANVAS_H_
#define OMNI_UI_NEWCANVAS_H_

#include <string>
#include <memory>
#include <QDialog>


namespace omni
{
  namespace canvas
  {
    class Interface;
  }
  namespace proj
  {
    class Setup;
  }

  class Session;

  namespace ui
  {
    namespace Ui
    {
      class NewCanvas;
    }

    class NewCanvas : public QDialog
    {
      Q_OBJECT
    public:
      NewCanvas(Session* _session, QWidget* _parent = nullptr);
      ~NewCanvas();

    public slots:
      void canvasChanged(int);
      void projSetupChanged(int);
      void newProjection();

    private:
      std::string canvasType() const;
      std::string projSetupType() const;
      Session* session_;

      std::unique_ptr<proj::Setup> projSetup_;
      std::unique_ptr<canvas::Interface> canvas_;
      std::unique_ptr<Ui::NewCanvas> ui_;
    };
  }
}

#endif /* OMNI_UI_NEWCANVAS_H_ */
