#include <omni/ui/MainWindow.h>

#include "ui_omni_ui_MainWindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCloseEvent>

#include <omni/Session.h>
#include <omni/ui/proj/Tuning.h>
#include <omni/ui/proj/TuningList.h>
#include <omni/ui/About.h>

#include <omni/ui/ScreenSetup.h>
#include <omni/ui/GLView3D.h>
#include <omni/ui/TuningGLView.h>
#include <omni/ui/Export.h>

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
  
  // Connect signals and slots
  {
    // Connect projector position change with view update
    connect(ui_->tuningList,SIGNAL(projectorSetupChanged()),this,SLOT(updateAllViews()));
    connect(ui_->tuningList,SIGNAL(currentIndexChanged(int)),this,SLOT(updateAllViews()));

    // Connect canvas parameter change with view update
    connect(ui_->grpCanvas,SIGNAL(canvasChanged()),this,SLOT(updateAllViews()));
    connect(ui_->grpCanvas,SIGNAL(canvasTypeChanged()),this,SLOT(updateAllViews()));

    // Update all views when input has changed
    connect(ui_->grpInputs,SIGNAL(inputChanged()),this,SLOT(updateAllViews())); 

    // Update all views when mapping mode has changed
    connect(ui_->grpMapping,SIGNAL(mappingChanged()),this,SLOT(updateAllViews()));
    connect(ui_->grpMapping,SIGNAL(mappingTypeChanged()),this,SLOT(updateAllViews()));

    // Connect tuning index of tuning list for warp and blend widget
    connect(ui_->tuningList,SIGNAL(currentIndexChanged(int)),warp_.get(),SLOT(setTuningIndex(int)));
    connect(ui_->tuningList,SIGNAL(currentIndexChanged(int)),blend_.get(),SLOT(setTuningIndex(int)));

    // Connect add tuning button with tuning list 
    connect(ui_->btnAddTuning,SIGNAL(clicked()),ui_->tuningList,SLOT(addTuning())); 
  }

  newSession();
  raise();
  show();
}

MainWindow::~MainWindow()
{
}
      
void MainWindow::newSession()
{
  session_.reset(new Session());

  // Attach session pointer
  ui_->tuningList->setSession(session_.get());
  ui_->grpCanvas->setSession(session_.get());
  ui_->grpMapping->setSession(session_.get());
  ui_->grpInputs->setSession(session_.get());
  
  // Set session to pages
  projectionSetup_->setSession(session_.get());

  blend_->setSession(session_.get());
  warp_->setSession(session_.get());

  setMode(session_->mode());
}

void MainWindow::newProjection()
{
  if (saveChangesPrompt() == QMessageBox::Cancel)
  {
    return;
  }

/*  std::unique_ptr<NewCanvas> _dialog(new NewCanvas(session_.get()));
  _dialog->raise();
  if (!_dialog->exec()) return;

  reset();
  initialPositions();*/
}

void MainWindow::saveProjection()
{
  if (filename_.isEmpty())
  {
    saveProjectionAs();
  }
  else
  {
    //session_->save(filename_);
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
    //session_->save(filename_);
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
//  session_->load(filename_);
  modified_ = false;
  filename_ = _filename;
  buttonState();
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
//  ui_->listScreens->close();
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
}
     
void MainWindow::setMode(Session::Mode _mode)
{
  session_->setMode(_mode);

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

  switch (_mode)
  {
    case Session::Mode::SCREENSETUP:
    ui_->grpCanvas->hide();
    ui_->grpInputs->show();
    ui_->grpMapping->hide();
    break;

    case Session::Mode::PROJECTIONSETUP:
    ui_->grpCanvas->show();
    ui_->grpInputs->show();
    ui_->grpMapping->show();
    break;

    case Session::Mode::WARP:
    ui_->grpCanvas->hide();
    ui_->grpInputs->show();
    ui_->grpMapping->show();
    break;

    case Session::Mode::BLEND:
    ui_->grpCanvas->hide();
    ui_->grpWarp->hide();
    ui_->grpBlend->show();
    ui_->grpInputs->hide();
    ui_->grpMapping->hide();
    break;

    case Session::Mode::EXPORT:
    ui_->grpCanvas->hide();
    ui_->grpWarp->hide();
    ui_->grpBlend->hide();
    ui_->grpInputs->show();
    ui_->grpMapping->show();
    break;

  default: break;
  }

  ui_->tuningList->sessionModeChange();
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

