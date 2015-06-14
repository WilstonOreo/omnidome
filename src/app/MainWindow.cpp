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

using namespace omni::ui;

MainWindow::MainWindow( QMainWindow *parent) :
  QMainWindow(parent),
  session_(new Session()),
  modified_(false),
  ui_(new Ui::MainWindow)
{
  ui_->setupUi(this);

  ui_->tuningList->setSession(session_.get());

  connect(ui_->btnAddTuning,SIGNAL(clicked()),ui_->tuningList,SLOT(addTuning()));

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

