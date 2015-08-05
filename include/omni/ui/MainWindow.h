#ifndef OMNI_UI_MAINWINDOW_H_
#define OMNI_UI_MAINWINDOW_H_

#include <set>
#include <QMainWindow>
#include <QComboBox>
#include <QModelIndex>
#include <QMessageBox>
#include <omni/util.h>
#include <omni/Session.h>

namespace omni
{ 
  namespace ui
  {
    class TuningGLView;
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

      MainWindow (QMainWindow *parent = nullptr);
      ~MainWindow();

    public slots:
      /// Initiaties a new session after dialog was confirmed 
      void newProjection();

      /// Save current session 
      void saveProjection();

      /// Set current session under a new filename
      void saveProjectionAs();

      /// Open a new session
      void openProjection();

      /// Open a new session from filename 
      bool openProjection(const QString& _filename);
      
      /// Edit current session with a new filename
      void editAsNew();

      /// Show About dialog
      void showAbout();

      /// Show Screen Setup Page (switches Session mode to Mode::SCREENSETUP)
      void showScreenSetup();

      /// Show Projection Setup Page (switches Session mode to Mode::PROJECTIONSETUP)
      void showProjectionSetup();

      /// Show Warp Grid Page (switches Session mode to Mode::WARP)
      void showWarp();

      /// Show Blend Mask Page (switches Session mode to Mode::BLEND)
      void showBlend();

      /// Show Export Page (switches Session mode to Mode::EXPORT)
      void showExport();

      /// Update all OpenGL views
      void updateAllViews();

    protected:
      void closeEvent(QCloseEvent* _event);
      void showEvent(QShowEvent* _event);

    private slots:
      /// Sets enabled state of toolbar buttons
      void buttonState();

      /// Set current tuning index
      void setTuningIndex(int);

    private:
      /// Sets session mode
      void setMode(Session::Mode _mode);
      
      /// Makes a new session
      void newSession();

      /// Message Box for changing changes
      QMessageBox::StandardButton saveChangesPrompt();

      /// Current filename
      QString filename_;

      /// Current projection session
      std::unique_ptr<Session> session_;
      
      /// Modified flag
      bool modified_ = false;
 
      /// UI containing designed widgets of this window
      std::unique_ptr<Ui::MainWindow> ui_;

      /// Screen Setup page
      QUniquePtr<ScreenSetup> screenSetup_;
      
      /// ProjectionSetup/Canvas preview page
      QUniquePtr<GLView3D> projectionSetup_;

      /// Page for current warp grid
      QUniquePtr<TuningGLView> warp_;
      
      /// Page for current blend mask
      QUniquePtr<TuningGLView> blend_;
      
      /// Page for exporting projection
      QUniquePtr<Export> export_;
    };
  }
}

#endif /* OMNI_UI_MAINWINDOW_H_ */
