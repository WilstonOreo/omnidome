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

#include <omni/ui/MainWindow.h>

#include "ui_omni_ui_MainWindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCloseEvent>
#include <QMenu>

#include <omni/Session.h>
#include <omni/proj/MultiSetup.h>
#include <omni/ui/proj/Tuning.h>
#include <omni/ui/proj/TuningList.h>
#include <omni/ui/About.h>

#include <omni/ui/ScreenSetup.h>
#include <omni/ui/GLView3D.h>
#include <omni/ui/TuningGLView.h>
#include <omni/ui/Export.h>
#include <omni/ui/proj/MultiSetupDialog.h>


using namespace omni::ui;

MainWindow::MainWindow( QMainWindow *parent) :
  QMainWindow(parent),
  modified_(false),
  ui_(new Ui::MainWindow)
{
  ui_->setupUi(this);

  // Make and setup pages
  {
    QLayout* _layout = new QHBoxLayout();
    screenSetup_.reset(new ScreenSetup(this));
    _layout->addWidget(screenSetup_.get());

    projectionSetup_.reset(new GLView3D(this));
    _layout->addWidget(projectionSetup_.get());

    warp_.reset(new TuningGLView(this));
    _layout->addWidget(warp_.get());
    warp_->setBorder(0.5);
    warp_->setKeepAspectRatio(true);

    blend_.reset(new TuningGLView(this));
    _layout->addWidget(blend_.get());
    blend_->setBorder(0.5);
    blend_->setKeepAspectRatio(true);

    export_.reset(new Export(session_.get()));
    _layout->addWidget(export_.get());
    ui_->pages->setLayout(_layout);
  }

  // Setup add projector template menu
  {
    QMenu* _menu = new QMenu();

    _menu->addAction("PeripheralSetup")->setData(QString("PeripheralSetup"));
    _menu->addAction("FreeSetup")->setData(QString("FreeSetup"));
    _menu->addSeparator();

    for (auto& _idMultiSetup : omni::proj::MultiSetupFactory::classes())
    {
      auto& _id = _idMultiSetup.first;
      auto* _action = _menu->addAction(_id);
      _action->setData(QString(_id));
    }

    connect(_menu,SIGNAL(triggered(QAction*)),this,SLOT(addProjector(QAction*)));

    ui_->btnAddTuning->setMenu(_menu);
  }

  // Connect signals and slots
  {
    // Connect projector position change with view update
    connect(ui_->tuningList,SIGNAL(projectorSetupChanged()),this,SLOT(modified()));
    connect(ui_->tuningList,SIGNAL(currentIndexChanged(int)),this,SLOT(modified()));
    
    // Detach tuning from screen setup when it was removed
    connect(ui_->tuningList,SIGNAL(tuningToBeRemoved(omni::ui::proj::Tuning*)),
        screenSetup_.get(),SLOT(detachTuning(omni::ui::proj::Tuning*)));

    // Connect canvas parameter change with view update
    connect(ui_->grpCanvas,SIGNAL(canvasChanged()),this,SLOT(modified()));
    connect(ui_->grpCanvas,SIGNAL(canvasTypeChanged()),this,SLOT(modified()));
    connect(ui_->grpCanvas,SIGNAL(displayInputToggled(bool)),projectionSetup_.get(),SLOT(setDisplayInput(bool)));

    // Update all views when input has changed
    connect(ui_->grpInputs,SIGNAL(inputChanged()),this,SLOT(modified()));

    // Update all views when mapping mode has changed
    connect(ui_->grpMapping,SIGNAL(mappingChanged()),this,SLOT(modified()));
    connect(ui_->grpMapping,SIGNAL(mappingTypeChanged()),this,SLOT(modified()));

    
    // Update all views when warp grid has changed
    connect(ui_->grpWarp,SIGNAL(warpGridChanged()),this,SLOT(modified()));

    // Update all views when warp grid has changed
    connect(ui_->grpBlend,SIGNAL(blendMaskChanged()),this,SLOT(modified()));

    // Connect tuning index of tuning list for warp and blend widget
    connect(ui_->tuningList,SIGNAL(currentIndexChanged(int)),this,SLOT(setTuningIndex(int)));

    // Connect add tuning button with tuning list
    connect(ui_->btnAddTuning,SIGNAL(clicked()),ui_->tuningList,SLOT(addTuning()));
    connect(ui_->btnAddTuning,SIGNAL(clicked()),this,SLOT(buttonState()));
  }

  session_.reset(new Session());
  setupSession();
  raise();
  show();
}

MainWindow::~MainWindow()
{
}



void MainWindow::setupSession()
{
  setMode(Session::Mode::SCREENSETUP);

  locked_ = true;
  {
    ui_->tuningList->setSession(session_.get());
    ui_->grpCanvas->setSession(session_.get());
    ui_->grpMapping->setSession(session_.get());
    ui_->grpInputs->setSession(session_.get());
    ui_->grpWarp->setSession(session_.get());
    ui_->grpBlend->setSession(session_.get());

    // Set session to pages
    projectionSetup_->setSession(session_.get());

    blend_->setSession(session_.get());
    warp_->setSession(session_.get());
    export_->setSession(session_.get());
  }
  locked_ = false;

  setMode(session_->mode());

}

void MainWindow::newProjection()
{
  if (saveChangesPrompt() == QMessageBox::Cancel)
  {
    return;
  }

  session_.reset(new Session());
  setupSession();
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
  QString _filename = QFileDialog::getSaveFileName(this, "Save omni Projection...", ".", "omni Projections (*.omni)");
  if( !_filename.isEmpty() )
  {
    filename_ = _filename;
    session_->save(filename_);
    modified_ = false;
    buttonState();
  }
}

void MainWindow::openProjection()
{
  if (saveChangesPrompt() == QMessageBox::Cancel)
  {
    return;
  }

  QString _filename = QFileDialog::getOpenFileName(this, "Open omni Projection...", ".", "omni Projections (*.omni)");
  if( !_filename.isEmpty() )
  {
    openProjection(_filename);
  }
}

bool MainWindow::openProjection(const QString& _filename)
{
  filename_ = _filename;
  session_.reset(new Session());
  session_->load(filename_);
  setupSession();
  modified_ = false;
  filename_ = _filename;
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

void MainWindow::showAbout()
{
  std::unique_ptr<About> _about(new About());
  _about->exec();
}

/// Show Screen Setup Page
void MainWindow::showScreenSetup()
{
  setMode(Session::Mode::SCREENSETUP);
}

/// Show ProjectionSetup Page
void MainWindow::showProjectionSetup()
{
  setMode(Session::Mode::PROJECTIONSETUP);
}

/// Show Warping Page
void MainWindow::showWarp()
{
  setMode(Session::Mode::WARP);
}

/// Show Blending Page
void MainWindow::showBlend()
{
  setMode(Session::Mode::BLEND);
}

/// Show Export Page
void MainWindow::showExport()
{
  setMode(Session::Mode::EXPORT);
}

void MainWindow::updateAllViews()
{
  projectionSetup_->update();
  warp_->update();
  blend_->update();
  ui_->tuningList->updateViews();
}

/// Sets modified flag to true
void MainWindow::modified()
{
  if (locked_) return;
  modified_ = true;
  updateAllViews();

  if (session_->mode() != Session::Mode::WARP &&
      session_->mode() != Session::Mode::BLEND)
    ui_->grpMapping->setVisible(session_->inputs().current() != nullptr);
  buttonState();
}


void MainWindow::showEvent(QShowEvent *event)
{
}

void MainWindow::closeEvent(QCloseEvent * _event)
{
  if (saveChangesPrompt() == QMessageBox::Cancel)
  {
    _event->ignore();
    return;
  }

  // Delete screen setup manually, so all fullscreen widgets are free'd too
  screenSetup_->hide();
  screenSetup_->setParent(nullptr);
  screenSetup_.reset();
}

void MainWindow::buttonState()
{
  QString _str("Omnidome");

  if (!filename_.isEmpty())
  {
    _str += " - " + filename_;
  }
  if (modified_)
    _str += "*";
  setWindowTitle(_str);

  ui_->btnEditAsNew->setEnabled(!filename_.isEmpty());
  ui_->btnSave->setEnabled(modified_);

  /// Show buttons only when there is a session
  bool _hasSession = !!session_;
  ui_->btnScreenSetup->setVisible(_hasSession);
  ui_->btnProjectionSetup->setVisible(_hasSession);
  ui_->btnWarp->setVisible(_hasSession);
  ui_->btnBlend->setVisible(_hasSession);
  ui_->btnExport->setVisible(_hasSession);

  if (session_->mode() == Session::Mode::BLEND) 
  {
    ui_->grpInputs->setVisible(session_->blendMode() == Session::BlendMode::INPUT);
    ui_->grpMapping->setVisible((session_->blendMode() == Session::BlendMode::INPUT)
        && session_->inputs().current());
  }

  if (_hasSession)
  {
    bool _hasTunings = session_->tunings().size() > 0;
    ui_->btnWarp->setEnabled(_hasTunings);
    ui_->btnBlend->setEnabled(_hasTunings);
    ui_->btnExport->setEnabled(_hasTunings);
  }
}

/// Set current tuning index
void MainWindow::setTuningIndex(int _index)
{
  ui_->tuningList->setTuningIndex(_index);
  warp_->setTuningIndex(_index);
  blend_->setTuningIndex(_index);

  warp_->setChildViews( ui_->tuningList->getViews(_index) );
  blend_->setChildViews( ui_->tuningList->getViews(_index) );


  ui_->grpWarp->updateWarpGrid();
  ui_->grpBlend->updateBlendMask();
}
      
void MainWindow::addProjector(QAction* _action)
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

  /// Check for multi projector setups 
  for (auto& _idMultiSetup : omni::proj::MultiSetupFactory::classes())
  {
    if (_idMultiSetup.first.str() == _id)
    {
      ui_->tuningList->addMultiSetup(_id);  
      return;
    }
  }
}

void MainWindow::setMode(Session::Mode _mode)
{
  session_->setMode(_mode);

  bool _hasTunings = session_->tunings().size() > 0;

  // Select first tuning if there is no tuning selected yet
  if (_hasTunings && session_->tunings().currentIndex() < 0)
  {
    setTuningIndex(0);
  }

  screenSetup_->setVisible(_mode == Session::Mode::SCREENSETUP);
  projectionSetup_->setVisible(_mode == Session::Mode::PROJECTIONSETUP);
  warp_->setVisible(_mode == Session::Mode::WARP);
  blend_->setVisible(_mode == Session::Mode::BLEND);
  export_->setVisible(_mode == Session::Mode::EXPORT);

  using omni::ui::proj::Tuning;

  ui_->btnAddTuning->setVisible(
    _mode == Session::Mode::SCREENSETUP ||
    _mode == Session::Mode::PROJECTIONSETUP);
  ui_->grpWarp->setVisible(_mode == Session::Mode::WARP);
  ui_->grpBlend->setVisible(_mode == Session::Mode::BLEND);
  ui_->grpCanvas->setVisible(_mode == Session::Mode::PROJECTIONSETUP);

  ui_->btnScreenSetup->setDown(_mode == Session::Mode::SCREENSETUP);
  ui_->btnProjectionSetup->setDown(_mode == Session::Mode::PROJECTIONSETUP);
  ui_->btnWarp->setDown(_mode == Session::Mode::WARP);
  ui_->btnBlend->setDown(_mode == Session::Mode::BLEND);
  ui_->btnExport->setDown(_mode == Session::Mode::EXPORT);

  switch (_mode)
  {
  case Session::Mode::SCREENSETUP:
    ui_->grpCanvas->hide();
    ui_->grpMapping->hide();
    ui_->grpInputs->show();
    break;

  case Session::Mode::PROJECTIONSETUP:
    ui_->grpCanvas->show();
    ui_->grpInputs->show();
    ui_->grpMapping->setVisible(session_->inputs().current());
    break;

  case Session::Mode::WARP:
    ui_->grpCanvas->hide();
    ui_->grpInputs->show();
    ui_->grpMapping->setVisible(session_->inputs().current());
    break;

  case Session::Mode::BLEND:
    ui_->grpCanvas->hide();
    ui_->grpInputs->setVisible(session_->blendMode() == Session::BlendMode::INPUT);
    ui_->grpMapping->setVisible((session_->blendMode() == Session::BlendMode::INPUT)
        && session_->inputs().current());
    break;

  case Session::Mode::EXPORT:
    ui_->grpCanvas->hide();
    ui_->grpInputs->show();
    ui_->grpMapping->setVisible(session_->inputs().current());
    break;

  default:
    break;
  }

  ui_->tuningList->sessionModeChange();
  buttonState();
}

QMessageBox::StandardButton MainWindow::saveChangesPrompt()
{
  if (modified_)
  {
    QMessageBox::StandardButton _reply = QMessageBox::question(this,"Screen update",
                                         "There are unsaved modifications. Do you want to save them?",
                                         QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel,QMessageBox::Yes);
    if (_reply == QMessageBox::Yes)
      saveProjection();
    return _reply;
  }
  return QMessageBox::No;
}

