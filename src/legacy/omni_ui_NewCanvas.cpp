#include <omni/ui/NewCanvas.h>
#include "ui_NewCanvas.h"

#include <omni/Session.h>

#include <tbd/qt/ParameterForm.h>
#include <omni/canvas/Interface.hpp>
#include <omni/proj/Setup.hpp>

using namespace omni::ui;

NewCanvas::NewCanvas(omni::Session* _session, QWidget* _parent) :
  QDialog(_parent),
  session_(_session),
  projSetup_(nullptr),
  canvas_(nullptr),
  ui_(new Ui::NewCanvas)
{
  ui_->setupUi(this);
  ui_->cbCanvasType->setEditable(false);
  ui_->cbProjSetup->setEditable(false);
  for (auto& _class : omni::canvas::Registry::classes())
  {
    ui_->cbCanvasType->addItem(QString().fromStdString(_class.first));
  }

  ui_->cbCanvasType->setCurrentIndex(0);
  raise();
}

NewCanvas::~NewCanvas() {}

void NewCanvas::canvasChanged(int)
{
  canvas_ = canvas::Registry::create(canvasType());
  ui_->previewGL->setCanvas(canvas_.get());

  if (ui_->canvasFrame->layout())
    delete ui_->canvasFrame->layout();
  ui_->canvasFrame->setLayout(new tbd::qt::ParameterForm(canvas_.get()));
  connect(
    ui_->canvasFrame->layout(),
    SIGNAL(valueChanged()),
    ui_->previewGL,
    SLOT(update()));

  ui_->cbProjSetup->clear();
  for (auto& _setup : canvas_->supportedSetups())
  {
    ui_->cbProjSetup->addItem(QString().fromStdString(_setup));
  }
  ui_->cbProjSetup->setCurrentIndex(0);
}

void NewCanvas::projSetupChanged(int _value)
{
  projSetup_ = proj::SetupRegistry::create(projSetupType());

  if (ui_->projSetupFrame->layout())
    delete ui_->projSetupFrame->layout();
  if (projSetup_)
  {
    ui_->projSetupFrame->setLayout(new tbd::qt::ParameterForm(projSetup_.get()));
    connect(
      ui_->projSetupFrame->layout(),
      SIGNAL(valueChanged()),
      ui_->previewGL,
      SLOT(update()));
  }

  ui_->previewGL->setProjSetup(projSetup_.get());
}

void NewCanvas::newProjection()
{
  session_->initialize(canvas_,projSetup_);
  std::cout << session_->canvas().get() << std::endl;
  accept();
}

std::string NewCanvas::canvasType() const
{
  return ui_->cbCanvasType->itemText(ui_->cbCanvasType->currentIndex()).toStdString();
}

std::string NewCanvas::projSetupType() const
{
  return ui_->cbProjSetup->itemText(ui_->cbProjSetup->currentIndex()).toStdString();
}
