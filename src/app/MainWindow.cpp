#include <omni/ui/MainWindow.h>

#include "ui_omni_ui_MainWindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCloseEvent>

#include <omni/Session.h>
#include <omni/ui/proj/TuningList.h>
#include <omni/ui/About.h>

#include <omni/ui/ScreenSetup.h>
#include <omni/ui/GLView3D.h>
#include <omni/ui/GLView2D.h>
#include <omni/ui/Export.h>

using namespace omni::ui;

MainWindow::MainWindow( QMainWindow *parent) :
  QMainWindow(parent),
  session_(new Session()),
  modified_(false),
  ui_(new Ui::MainWindow)
{
  ui_->setupUi(this);

  // Attach session pointer
  ui_->tuningList->setSession(session_.get());
  ui_->grpCanvas->setSession(session_.get());
  ui_->grpMapping->setSession(session_.get());

  connect(ui_->btnAddTuning,SIGNAL(clicked()),ui_->tuningList,SLOT(addTuning()));

  // Make and setup pages
  {
    QLayout* _layout = new QHBoxLayout();
    screenSetup_.reset(new ScreenSetup());
    _layout->addWidget(screenSetup_.get());

    positioning_.reset(new GLView3D());
    positioning_->setSession(session_.get());
    _layout->addWidget(positioning_.get());

    warp_.reset(new GLView2D());
    warp_->setSession(session_.get());
    _layout->addWidget(warp_.get());

    blend_.reset(new GLView2D());
    blend_->setSession(session_.get());
    _layout->addWidget(blend_.get());

    export_.reset(new Export(session_.get()));
    _layout->addWidget(export_.get());
    ui_->pages->setLayout(_layout);

    setMode(SCREENSETUP);
  }


  raise();
  show();
}

MainWindow::~MainWindow()
{
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
  setMode(SCREENSETUP);
}

/// Show Positioning Page
void MainWindow::showPositioning()
{
  setMode(POSITIONING);
}

/// Show Warping Page
void MainWindow::showWarp()
{
  setMode(WARP);
}

/// Show Blending Page
void MainWindow::showBlend()
{
  setMode(BLEND);
}

/// Show Export Page
void MainWindow::showExport()
{
  setMode(EXPORT);
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
     
void MainWindow::setMode(Mode _mode)
{
  screenSetup_->setVisible(_mode == SCREENSETUP);
  positioning_->setVisible(_mode == POSITIONING);
  warp_->setVisible(_mode == WARP);
  blend_->setVisible(_mode == BLEND);
  export_->setVisible(_mode == EXPORT);

  switch (_mode)
  {
  case SCREENSETUP:
    ui_->grpCanvas->show();
    ui_->grpWarp->hide();
    ui_->grpBlend->hide();
    ui_->grpInputs->show();
    ui_->grpMapping->show();
    break;

  case POSITIONING:
    ui_->grpCanvas->show();
    ui_->grpWarp->hide();
    ui_->grpBlend->hide();
    ui_->grpInputs->show();
    ui_->grpMapping->show();
    break;

  case WARP:
    ui_->grpCanvas->hide();
    ui_->grpWarp->show();
    ui_->grpBlend->hide();
    ui_->grpInputs->show();
    ui_->grpMapping->show();
    break;

  case BLEND:
    ui_->grpCanvas->hide();
    ui_->grpWarp->hide();
    ui_->grpBlend->show();
    ui_->grpInputs->show();
    ui_->grpMapping->show();
    break;

  case EXPORT:
    ui_->grpCanvas->hide();
    ui_->grpWarp->hide();
    ui_->grpBlend->hide();
    ui_->grpInputs->show();
    ui_->grpMapping->show();
    break;

  default: break;
  }

  mode_=_mode;
}

void MainWindow::toolButtonsVisible(bool _visible)
{
  ui_->btnSave->setVisible(_visible);
  ui_->btnEditAsNew->setVisible(_visible);
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

