/* Copyright (c) 2014-2015 "Omnidome" by cr8tr
 * Dome Mapping Projection Software (http://omnido.me).
 * Omnidome was created by Michael Winkelmann aka Wilston Oreo (@WilstonOreo)
 * 
 * This file is part of Omnidome.
 * 
 * Omnidome is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef OMNI_UI_MAINWINDOW_H_
#define OMNI_UI_MAINWINDOW_H_

#include <set>
#include <QMainWindow>
#include <QOpenGLContext>
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

      /// Sets modified flag to true
      void modified();

    protected:
      void closeEvent(QCloseEvent* _event);
      void showEvent(QShowEvent* _event);

    private slots:
      /// Sets enabled state of toolbar buttons
      void buttonState();

      /// Set current tuning index
      void setTuningIndex(int);


      void addProjector(QAction* _action);


    private:
      /// Sets session mode
      void setMode(Session::Mode _mode);
      
      /// Makes a new session
      void setupSession();

      /// Message Box for changing changes
      QMessageBox::StandardButton saveChangesPrompt();

      /// Current filename
      QString filename_;

      /// Current projection session
      std::unique_ptr<Session> session_;
      
      /// Modified flag
      bool modified_ = false;
 
      /// Locked flag
      bool locked_;

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
    
      QUniquePtr<QOpenGLContext> glContext_;
    };
  }
}

#endif /* OMNI_UI_MAINWINDOW_H_ */
