#ifndef OMNI_UI_MAINWINDOW_H_
#define OMNI_UI_MAINWINDOW_H_

#include <set>
#include <QMainWindow>
#include <QComboBox>
#include <QModelIndex>
#include <QMessageBox>
#include <omni/util.h>


namespace omni
{
  class Session;
  
  namespace ui
  {
    class ProjectorWidget;
    class GLView;

    namespace Ui
    {
      class MainWindow;
    }

    class MainWindow : public QMainWindow
    {
      Q_OBJECT
    public:
      enum Mode
      {
        CANVAS,
        SCREENS,
        POSITIONING,
        WARP,
        BLEND,
        EXPORT
      };

      typedef std::set<GLView*> view_set_type;

      MainWindow (QMainWindow *parent = nullptr);
      ~MainWindow();

    public slots:
      void newProjection();
      void saveProjection();
      void saveProjectionAs();
      void openProjection();
      bool openProjection(const QString& _filename);
      void editAsNew();

      /// Show About dialog
      void showAbout();

    protected:
      void closeEvent(QCloseEvent* _event);
      void showEvent(QShowEvent* _event);

    private:
      void buttonState();

      void toolButtonsVisible(bool);
      
      QMessageBox::StandardButton saveChangesPrompt();

      QString filename_;
      std::unique_ptr<Session> session_;
      bool modified_ = false;
 
      std::unique_ptr<Ui::MainWindow> ui_;
      
      Mode mode_ = CANVAS;
    };
  }
}

#endif /* OMNI_UI_MAINWINDOW_H_ */
