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
    class GLView2D;
    class GLView3D;
    class ScreenSetup;
    class Export;

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
        SCREENSETUP,
        POSITIONING,
        WARP,
        BLEND,
        EXPORT
      };

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

      /// Show Screen Setup Page
      void showScreenSetup();

      /// Show Positioning Page
      void showPositioning();

      /// Show Warping Page
      void showWarp();

      /// Show Blending Page
      void showBlend();

      /// Show Export Page
      void showExport();

    protected:
      void closeEvent(QCloseEvent* _event);
      void showEvent(QShowEvent* _event);

    private:
      void buttonState();
      void setMode(Mode _mode);
      void toolButtonsVisible(bool);
      
      QMessageBox::StandardButton saveChangesPrompt();

      QString filename_;
      std::unique_ptr<Session> session_;
      bool modified_ = false;
 
      std::unique_ptr<Ui::MainWindow> ui_;


      QUniquePtr<ScreenSetup> screenSetup_;
      QUniquePtr<GLView3D> positioning_;
      QUniquePtr<GLView2D> warp_;
      QUniquePtr<GLView2D> blend_;
      QUniquePtr<Export> export_;

      Mode mode_ = SCREENSETUP;
    };
  }
}

#endif /* OMNI_UI_MAINWINDOW_H_ */
