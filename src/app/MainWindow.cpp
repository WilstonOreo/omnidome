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

#include "MainWindow.h"

#include "ui_omni_ui_MainWindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCloseEvent>
#include <QMenu>
#include <QIcon>

#include "Application.h"
#include "RecentSessions.h"
#include <omni/Session.h>
#include <omni/ui/SceneGLView.h>
#include <omni/ui/TuningGLView.h>
#include <omni/ui/ExceptionList.h>
#include <omni/serialization/PropertyMap.h>

#include "proj/Tuning.h"
#include "proj/TuningList.h"

#include "SceneViewer.h"
#include "ScreenSetup.h"
#include "Export.h"
#include "Mapping.h"
#include "Input.h"

#include "ToolBar.h"

using namespace omni::ui;

MainWindow::MainWindow(QMainWindow *parent) :
  QMainWindow(parent),
  session_(new Session()),
  modified_(false),
  ui_(new Ui::MainWindow)
{
  ui_->setupUi(this);

  // Setup toolbar
  {
    toolBar_.reset(new ToolBar(this));
    this->addToolBar(Qt::TopToolBarArea, toolBar_.get());
    connect(toolBar_.get(), SIGNAL(dataModelChanged()),
            this, SLOT(setMode()));
  }

  // Make and setup pages
  {
    QLayout *_layout = new QHBoxLayout();
    screenSetup_.reset(new ScreenSetup(this));
    _layout->addWidget(screenSetup_.get());

    sceneViewer_.reset(new SceneViewer(this));
    _layout->addWidget(sceneViewer_.get());
    sceneViewer_->view()->setUpdateFrequency(60.0);

    tuningView_.reset(new TuningGLView(this));
    _layout->addWidget(tuningView_.get());
    tuningView_->setBorder(0.5);
    tuningView_->setKeepAspectRatio(true);

    export_.reset(new Export(this));
    _layout->addWidget(export_.get());

    _layout->setContentsMargins(0, 0, 0, 0);
    ui_->pages->setLayout(_layout);
  }

  ui_->statusbar->showMessage(QString("Omnidome VR") +
                              QString(OMNIDOME_VERSION_STRING));

  // Setup add projector template menu
  {
    QMenu *_menu = new QMenu();

    _menu->addAction("PeripheralSetup")->setData(QString("PeripheralSetup"));
    _menu->addAction("FreeSetup")->setData(QString("FreeSetup"));
    _menu->addSeparator();

    connect(_menu, SIGNAL(triggered(QAction *)), this,
            SLOT(addProjector(QAction *)));

    ui_->btnAddTuning->setMenu(_menu);
  }

  /// Set dock widget tabs
  {
    tabifyDockWidget(ui_->dockCanvas, ui_->dockScene);
    tabifyDockWidget(ui_->dockInputs, ui_->dockMapping);
    tabifyDockWidget(ui_->dockInputs, ui_->dockColorCorrection);
    tabifyDockWidget(ui_->dockInputs, ui_->dockWarp);
    tabifyDockWidget(ui_->dockInputs, ui_->dockBlend);
  }

  // Connect signals and slots
  {
    // Connect projector position change with view update
    connect(ui_->tuningList, SIGNAL(dataModelChanged()), this,
            SLOT(setTuningIndex()));
    connect(ui_->tuningList, SIGNAL(tuningChanged()), this,
            SLOT(updateAllViews()));
    connect(ui_->tuningList,
            &proj::TuningList::dataModelChanged,
            toolBar_.get(),
            &ToolBar::updateFrontend);
    connect(ui_->tuningList, SIGNAL(tuningRemoved()), screenSetup_.get(),
            SLOT(updateScreens()));
    connect(ui_->tuningList, SIGNAL(tuningRemoved()), this,
            SLOT(updateAllViews()));
    screenSetup_->setTuningList(ui_->tuningList);

    connect(ui_->actionEnableAllTunings, SIGNAL(triggered()),
            ui_->tuningList, SLOT(enableAllTunings()));
    connect(ui_->actionDisableAllTunings, SIGNAL(triggered()),
            ui_->tuningList, SLOT(disableAllTunings()));
    connect(ui_->actionEnableSelectedTuningOnly, SIGNAL(triggered()),
            ui_->tuningList, SLOT(enableSelectedTuningOnly()));

    // Connect canvas parameter change with view update
    connect(ui_->dockCanvasWidget, SIGNAL(
              dataModelChanged()),                     this,
            SLOT(updateAllViews()));
    connect(ui_->dockCanvasWidget, SIGNAL(canvasTypeChanged()),
            this, SLOT(modified()));

    // Connect scene parameter change with 3d view update
    connect(ui_->dockSceneWidget, SIGNAL(dataModelChanged()),
            this, SLOT(updateAllViews()));

    /// Handle scene scale and unit events
    connect(ui_->dockSceneWidget, SIGNAL(sceneScaleChanged(bool)),
            ui_->tuningList, SLOT(updateSceneSize(bool)));
    connect(ui_->dockSceneWidget, SIGNAL(sceneScaleChanged(bool)),
            ui_->dockCanvasWidget, SLOT(updateSceneSize(bool)));
    connect(ui_->dockSceneWidget, SIGNAL(unitChanged()),
            ui_->dockCanvasWidget, SLOT(updateUnits()));
    connect(ui_->dockSceneWidget, SIGNAL(unitChanged()),
            ui_->tuningList, SLOT(updateUnits()));

    // Update all views when input has changed
    connect(ui_->dockInputsWidget, SIGNAL(
              inputIndexChanged()),                 this,
            SLOT(modified()));

    connect(ui_->dockInputsWidget, SIGNAL(
              inputIndexChanged()),                 sceneViewer_.get(),
            SLOT(showInputControlWidget()));
    connect(ui_->dockInputsWidget, SIGNAL(
              inputRemoved()),                 sceneViewer_.get(),
            SLOT(removeInputControlWidget()));
    connect(ui_->dockInputsWidget, SIGNAL(
              inputIndexChanged()),                 this,
            SLOT(setMode()));
    connect(ui_->dockInputsWidget, SIGNAL(
              inputChanged()),                 this,
            SLOT(updateAllViews()));

    // Update all views when mapping mode has changed
    connect(ui_->dockMappingWidget, SIGNAL(
              dataModelChanged()),                    this,
            SLOT(modified()));
    connect(ui_->dockMappingWidget, SIGNAL(
              dataModelChanged()),                    this,
            SLOT(updateAllViews()));
    connect(ui_->dockMappingWidget, SIGNAL(
              dataModelChanged()),                      toolBar_.get(),
            SLOT(buttonStates()));
    connect(ui_->dockMappingWidget, &Mapping::dataModelChanged,
            export_.get(), &Export::updateFrontend);

    // Update all views when warp grid has changed
    connect(ui_->dockWarpWidget, SIGNAL(
              dataModelChanged()),                   this,
            SLOT(modified()));

    // Update all views when blend mask has changed
    connect(ui_->dockBlendWidget,
            &Blend::dataModelChanged,
            this,
            &MainWindow::modified);

    // Update all views when color correction has changed
    connect(ui_->dockColorCorrectionWidget, SIGNAL(
              dataModelChanged()),                  this,
            SLOT(modified()));

    // Connect add tuning button with tuning list
    connect(ui_->btnAddTuning, SIGNAL(
              clicked()),                           ui_->tuningList,
            SLOT(addTuning()));
    connect(ui_->btnAddTuning, SIGNAL(
              clicked()),                           this,
            SLOT(buttonState()));

    // Connect about button with toolbar showAbout slot
    connect(ui_->actionAbout,SIGNAL(triggered()),toolBar_.get(),SLOT(showAbout()));
  }

  {
    // Set splitter sizes
    QList<int> _list;
    int _width = width();
    _list << _width * 1.2 << 100;
    ui_->splitter->setSizes(_list);
  }

  {
    recentSessions_.reset(new RecentSessions);
    ui_->actionRecentFiles->setMenu(recentSessions_->menu());
    connect(recentSessions_.get(), SIGNAL(fileToBeLoaded(QString const &)),
            this, SLOT(openProjection(QString const &)));
  }

  readSettings();
  setupSession(session_);
  setMode();
  raise();
  show();
}

MainWindow::~MainWindow()
{}

void MainWindow::readSettings()
{
  restoreGeometry(Application::settings().value("geometry").toByteArray());

  auto _windowState =
    Application::settings().value("windowState").toByteArray();

  if (_windowState.isEmpty()) {
    setWindowState(Qt::WindowMaximized);
  } else {
    restoreState(_windowState);
  }

  recentSessions_->readFromSettings();
}

void MainWindow::setupSession(std::shared_ptr<Session>& _session)
{
  using util::enumToInt;
  locked_ = true;
  {
    ui_->dockInputsWidget->clear();
    ui_->dockSceneWidget->setDataModel(_session);
    ui_->dockInputsWidget->setDataModel(_session);
    toolBar_->setDataModel(_session);
    screenSetup_->setDataModel(_session);

    // Set session to pages
    sceneViewer_->setDataModel(_session);
    tuningView_->setDataModel(_session);
    export_->setDataModel(_session);

    ui_->tuningList->setDataModel(_session);
    ui_->dockMappingWidget->setDataModel(_session);
    ui_->dockCanvasWidget->setDataModel(_session);
    ui_->dockWarpWidget->setDataModel(_session);
    ui_->dockBlendWidget->setDataModel(_session);
    ui_->dockColorCorrectionWidget->setDataModel(_session);

    screenSetup_->updateScreens();
  }

  locked_ = false;
}

void MainWindow::newProjection()
{
  if (saveChangesPrompt() == QMessageBox::Cancel)
  {
    return;
  }

  session_.reset(new Session());
  setupSession(session_);
  setMode();
  modified_ = false;
  buttonState();
}

void MainWindow::saveProjection()
{
  if (filename_.isEmpty())
  {
    saveProjectionAs();
  }
  else
  {
    session_->save(filename_);
    modified_ = false;
    buttonState();
  }
}

void MainWindow::saveProjectionAs()
{
  QString _filename = QFileDialog::getSaveFileName(this,
                                                   "Save omni Projection...",
                                                   ".",
                                                   "omni Projections (*.omni)");

  if (!_filename.isEmpty())
  {
    std::unique_ptr<ExceptionList> _widget(new ExceptionList);
    try {
      filename_ = _filename;
      session_->save(filename_);
      recentSessions_->addFile(filename_);
      modified_ = false;
      buttonState();
    } catch (Exception& e) {
      _widget->addException(e);
    }
    if (_widget->exceptionCount() > 0) {
      _widget->exec();
    }
  }
}

void MainWindow::openProjection()
{
  if (saveChangesPrompt() == QMessageBox::Cancel)
  {
    return;
  }

  QString _filename = QFileDialog::getOpenFileName(this,
                                                   "Open omni Projection...",
                                                   ".",
                                                   "omni Projections (*.omni)");

  if (!_filename.isEmpty())
  {
    openProjection(_filename);
  }
}

bool MainWindow::openProjection(const QString& _filename)
{
  filename_ = _filename;
  std::shared_ptr<Session> _session(new Session);

  std::unique_ptr<ExceptionList> _widget(new ExceptionList);
  tryWithExceptionList<Exception>([&]() {
    _session->load(filename_);
    setupSession(_session);
    session_ = _session;
    modified_ = false;
    filename_ = _filename;
    recentSessions_->addFile(filename_);
    setMode();
  });

  return true;
}

void MainWindow::editAsNew()
{
  if (!filename_.isEmpty() || modified_)
  {
    if (saveChangesPrompt() == QMessageBox::Cancel)
    {
      return;
    }
  }

  filename_ = "";
  modified_ = false;
  buttonState();
}

void MainWindow::updateAllViews()
{
  switch (session_->mode()) {
  case Session::Mode::ARRANGE:
  case Session::Mode::LIVE:
    sceneViewer_->triggerUpdate();
    break;
  default:
  case Session::Mode::WARP:
  case Session::Mode::BLEND:
  case Session::Mode::COLORCORRECTION:
    tuningView_->makeCurrent();
    tuningView_->triggerUpdate();
    break;
  }
  ui_->tuningList->updateViews();
}

/// Sets modified flag to true
void MainWindow::modified()
{
  if (locked_) return;

  modified_ = true;
  updateAllViews();
  buttonState();
}

void MainWindow::showEvent(QShowEvent *event)
{}

void MainWindow::closeEvent(QCloseEvent *_event)
{
  if (saveChangesPrompt() == QMessageBox::Cancel)
  {
    _event->ignore();
    return;
  }

  // Delete screen setup manually, so all fullscreen widgets are free'd too
  if (screenSetup_) {
    screenSetup_->setParent(nullptr);
    screenSetup_.reset();
  }

  Application::settings().setValue("geometry", saveGeometry());
  Application::settings().setValue("windowState", saveState());
  recentSessions_->writeToSettings();
  QMainWindow::closeEvent(_event);
}

void MainWindow::buttonState()
{
  QString _str("Omnidome");

  if (!filename_.isEmpty())
  {
    _str += " - " + filename_;
  }

  if (modified_) _str += "*";
  setWindowTitle(_str);

  ui_->actionEditAsNew->setEnabled(!filename_.isEmpty());
  ui_->actionSave->setEnabled(modified_);
  toolBar_->buttonStates();
}

/// Set current tuning index
void MainWindow::setTuningIndex()
{
  int _index = session_->tunings().currentIndex();

  tuningView_->setTuningIndex(_index);
  tuningView_->setChildViews(ui_->tuningList->getViews(_index));

  ui_->dockColorCorrectionWidget->updateFrontend();
  ui_->dockBlendWidget->updateFrontend();
  ui_->dockWarpWidget->updateFrontend();
}

void MainWindow::addProjector(QAction *_action)
{
  auto _id = _action->data().toString();

  /// Check for single projector setups
  for (auto& _idSetup : omni::proj::SetupFactory::classes())
  {
    if (_idSetup.first.str() == _id)
    {
      ui_->tuningList->addTuning(_id);
      return;
    }
  }
}

void MainWindow::setMode()
{
  auto _mode       = session_->mode();
  bool _hasTunings = session_->hasOutput();

  screenSetup_->setVisible(_mode == Session::Mode::SCREENSETUP);
  sceneViewer_->setVisible(_mode == Session::Mode::ARRANGE ||
                           (_mode == Session::Mode::LIVE && _hasTunings));
  tuningView_->setVisible((
                            _mode == Session::Mode::WARP ||
                            _mode == Session::Mode::BLEND ||
                            _mode == Session::Mode::COLORCORRECTION) &&
                          _hasTunings);
  export_->setVisible(_mode == Session::Mode::EXPORT && _hasTunings);

  using omni::ui::proj::Tuning;

  ui_->btnAddTuning->setVisible(
    _mode == Session::Mode::SCREENSETUP ||
    _mode == Session::Mode::ARRANGE);
  ui_->dockCanvas->setVisible(_mode == Session::Mode::ARRANGE);
  ui_->dockWarp->setVisible(_mode == Session::Mode::WARP);
  ui_->dockBlend->setVisible(_mode == Session::Mode::BLEND);
  ui_->dockColorCorrection->setVisible(_mode == Session::Mode::COLORCORRECTION);
  ui_->dockScene->hide();

  switch (_mode)
  {
  case Session::Mode::SCREENSETUP:
    ui_->dockMapping->hide();
    ui_->dockInputs->show();
    break;

  case Session::Mode::ARRANGE:
    ui_->dockInputs->show();
    ui_->dockMapping->setVisible(session_->inputs().current());
    ui_->dockCanvas->raise();
    ui_->dockScene->show();
    break;

  case Session::Mode::WARP:
    ui_->dockInputs->show();
    ui_->dockMapping->setVisible(session_->inputs().current());
    ui_->dockWarp->raise();
    break;

  case Session::Mode::BLEND:
    ui_->dockInputs->setVisible(true);
    ui_->dockMapping->setVisible(false);
    ui_->dockBlend->raise();
    break;

  case Session::Mode::COLORCORRECTION:
    ui_->dockInputs->setVisible(true);
    ui_->dockMapping->setVisible(session_->inputs().current());
    ui_->dockColorCorrection->raise();
    break;

  case Session::Mode::EXPORT:
    ui_->dockInputs->show();
    ui_->dockMapping->setVisible(session_->inputs().current());
    ui_->dockInputs->raise();
    break;

  case Session::Mode::LIVE:
    ui_->dockInputs->show();
    ui_->dockMapping->hide();
    ui_->dockScene->show();
  default:
    break;
  }

  ui_->tuningList->sessionModeChange();
  updateAllViews();
  buttonState();
}

QMessageBox::StandardButton MainWindow::saveChangesPrompt()
{
  if (modified_)
  {
    QMessageBox::StandardButton _reply = QMessageBox::question(this,
                                                               "Screen update",
                                                               "There are unsaved modifications. Do you want to save them?",
                                                               QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                                                               QMessageBox::Yes);

    if (_reply == QMessageBox::Yes) saveProjection();
    return _reply;
  }
  return QMessageBox::No;
}
