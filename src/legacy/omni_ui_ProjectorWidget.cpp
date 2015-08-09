#include <omni/ui/ProjectorWidget.h>

#include "ui_ProjectorWidget.h"

#include <omni/Mapping.h>

using namespace omni::ui;

ProjectorWidget::ProjectorWidget(
  Mapping* _mapping,
  const view_set_type& _views) :
  QWidget(nullptr),
  updateMode_(true),
  mapping_(_mapping),
  views_(_views),
  ui_(new Ui::ProjectorWidget)
{
  ui_->setupUi(this);
  getSettings();
};

ProjectorWidget::~ProjectorWidget()
{
}

omni::proj::Projector const* ProjectorWidget::proj() const
{
  if (!mapping()) return nullptr;
  return &mapping()->projector();
}

omni::proj::Projector* ProjectorWidget::proj()
{
  if (!mapping()) return nullptr;
  return &mapping()->projector();
}

void ProjectorWidget::toggleSettings()
{
  ui_->settingsFrame->setVisible(ui_->groupBox->isChecked());

  for (auto& _view : views_)
  {
    if (!ui_->groupBox->isChecked())
    {
      _view->excludedProjectors().insert(&mapping_->projector());
    }
    else
    {
      _view->excludedProjectors().erase(&mapping_->projector());
    }
    _view->update();
  }
}

void ProjectorWidget::projectorVisible(bool _v)
{
  for (auto& _view : views_)
    _view->update();
}

void ProjectorWidget::updateSettings(double)
{
  if (updateMode_) return;

  mapping_->projector().adjust(
    ui_->boxYawAngle->value(),
    ui_->boxDistanceCenter->value(),
    ui_->boxTowerHeight->value(),
    ui_->boxShift->value(),
    ui_->boxPitchAngle->value(),
    ui_->boxRollAngle->value(),
    ui_->boxDeltaYawAngle->value());

  for (auto& _view : views_)
    _view->update();
}

void ProjectorWidget::getSettings()
{
  if (!mapping_) return;
  updateMode_ = true;
  auto& _proj = mapping_->projector();

  ui_->boxYawAngle->setValue(_proj.yaw());
  ui_->boxDistanceCenter->setValue(_proj.distance_center());
  ui_->boxTowerHeight->setValue(_proj.tower_height());
  ui_->boxShift->setValue(_proj.shift());
  ui_->boxPitchAngle->setValue(_proj.pitch());
  ui_->boxRollAngle->setValue(_proj.roll());
  ui_->boxDeltaYawAngle->setValue(_proj.delta_yaw());
  updateMode_ = false;
}
