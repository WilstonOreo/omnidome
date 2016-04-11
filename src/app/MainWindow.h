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
#include <omni/ui/mixin/DataModel.h>

namespace omni {
  namespace ui {
    namespace Ui {
      class MainWindow;
    }
    class SceneViewer;
    class ScreenSetup;
    class Export;
    class TuningGLView;
    class ToolBar;
    class RecentSessions;

    class MainWindow :
      public QMainWindow {
      Q_OBJECT

      public:
        MainWindow(QMainWindow *parent = nullptr);
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

        /// Update all OpenGL views
        void updateAllViews();

        /// Sets modified flag to true
        void modified();

      protected:
        void closeEvent(QCloseEvent *_event);
        void showEvent(QShowEvent *_event);

      private slots:
        /// Sets enabled state of toolbar buttons
        void                        buttonState();

        /// Set current tuning index
        void                        setTuningIndex();

        void                        addProjector(QAction *_action);

        /// Sets session mode
        void                        setMode();

      private:
        void                        readSettings();

        /// Makes a new session
        void                        setupSession(std::shared_ptr<Session>& _session);

        /// Message Box for changing changes
        QMessageBox::StandardButton saveChangesPrompt();

        /// Current filename
        QString filename_;

        /// Current projection session
        std::shared_ptr<Session> session_;

        /// Modified flag
        bool modified_ = false;

        /// Locked flag
        bool locked_;

        /// Screen Setup page
        QUniquePtr<ScreenSetup> screenSetup_;

        /// ProjectionSetup/Canvas arrangement and live view page
        QUniquePtr<SceneViewer> sceneViewer_;

        /// Page for warp grid, blend mask and color correction
        QUniquePtr<TuningGLView> tuningView_;

        /// Page for exporting projection
        QUniquePtr<Export> export_;

        /// Store recent sessions
        QUniquePtr<RecentSessions> recentSessions_;

        /// MainWindow toolbar
        QUniquePtr<ToolBar> toolBar_;

        /// UI containing designed widgets of this window
        std::unique_ptr<Ui::MainWindow> ui_;
    };
  }
}

#endif /* OMNI_UI_MAINWINDOW_H_ */
