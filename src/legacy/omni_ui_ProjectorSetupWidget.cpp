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

#include <omni/ui/ProjectorSetupWidget.h>

#include "ui_ProjectorSetupWidget.h"
#include <QMessageBox>
#include <omni/Session.h>

using namespace omni::ui;

ProjectorSetupWidget::ProjectorSetupWidget(omni::Session* _session,
    int _mapId,
    const view_set_type& _views) :
  QWidget(nullptr),
  mapId_(_mapId),
  session_(_session),
  views_(_views),
  lastScreenIdx_(0),
  ui_(new Ui::ProjectorSetupWidget)
{
  ui_->setupUi(this);
  getSettings();
  updateScreens();
}

ProjectorSetupWidget::~ProjectorSetupWidget()
{
}

void ProjectorSetupWidget::updateScreens()
{
  if (!mapping()) return;
  ui_->cbScreen->clear();
  ui_->cbScreen->setEditable(false);
  ui_->cbScreen->addItem("(None)");
  for (auto& _screen : session_->screens())
  {
    std::stringstream _ss;
    _ss << (_screen->index()+1) << " (" << _screen->width() << "x" << _screen->height() << ")";
    ui_->cbScreen->addItem(QString().fromStdString(_ss.str()));
  }

  ui_->cbScreen->setCurrentIndex(0);
}

void ProjectorSetupWidget::toggleSettings(bool _toggle)
{
  ui_->frameSettings->setVisible(_toggle);

  for (auto& _view : views_)
  {
    if (!_toggle)
    {
      _view->excludedProjectors().insert(&mapping()->projector());
    }
    else
    {
      _view->excludedProjectors().erase(&mapping()->projector());
    }
    _view->update();
  }
}

void ProjectorSetupWidget::fovChanged(double _fov)
{
  if (updateMode_) return;
  updateMode_ = true;
  auto& _proj = mapping()->projector();
  _proj.fov(_fov);
  ui_->boxThrowRatio->setValue(_proj.throwRatio());
  for (auto& _view : views_)
    _view->update();
  updateMode_ = false;
  emit command(nullptr);
}

void ProjectorSetupWidget::throwRatioChanged(double _throwRatio)
{
  if (updateMode_) return;
  updateMode_ = true;
  auto& _proj = mapping()->projector();
  _proj.throwRatio(_throwRatio);
  ui_->boxFOV->setValue(_proj.fov());
  for (auto& _view : views_)
    _view->update();
  updateMode_ = false;
  emit command(nullptr);
}

void ProjectorSetupWidget::screenChanged(int _screenId)
{
  if (!session_) return;
  if (_screenId == lastScreenIdx_) return;

  proj::Screen const* _screen = _screenId == 0 ? nullptr : session_->getScreen(_screenId-1);

  if (mapping()->resolutionToBeChanged(_screen))
  {
    QMessageBox::StandardButton _reply =
      QMessageBox::question(this,"Reset warp grid",
                            "Changing the screen resolution causes the current blend masks to be discarded. Do you want to continue?",
                            QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
    if (_reply == QMessageBox::No)
    {
      ui_->cbScreen->setCurrentIndex(lastScreenIdx_);
      return;
    }
  }

  lastScreenIdx_ = _screenId;

  mapping()->screen(_screen);

  ui_->lbWidth->setVisible(!_screen);
  ui_->lbHeight->setVisible(!_screen);
  ui_->boxWidth->setVisible(!_screen);
  ui_->boxHeight->setVisible(!_screen);

  if (_screen)
  {
    ui_->lbSubScreen->setVisible(_screen->hasSubScreens());
    ui_->boxSubScreen->setVisible(_screen->hasSubScreens());
    ui_->boxSubScreen->setValue(mapping()->subScreenId()+1);
    if (ui_->boxSubScreen->isVisible())
    {
      ui_->boxSubScreen->setMinimum(1);
      ui_->boxSubScreen->setMaximum(_screen->subScreenCount());
    }

    for (auto& _view : views_)
    {
      _view->useTexture();
      _view->update();
    }
  }
  else
  {
    ui_->lbSubScreen->hide();
    ui_->boxSubScreen->hide();
    resolutionChanged();
  }
  emit screenChanged();
}

void ProjectorSetupWidget::subScreenChanged(int _screenId)
{
  if (!mapping()) return;
  mapping()->subScreenId(_screenId-1);
  emit screenChanged();
}

void ProjectorSetupWidget::resolutionChanged()
{
  mapping()->resize(ui_->boxWidth->value(),ui_->boxHeight->value());
  emit command(nullptr);

  for (auto& _view : views_)
  {
    _view->useTexture();
    _view->update();
  }
}

void ProjectorSetupWidget::getSettings()
{
  if (!mapping()) return;
  updateMode_ = true;
  auto& _proj = mapping()->projector();

  ui_->cbScreen->clear();
  ui_->boxFOV->setValue(_proj.fov());
  ui_->boxThrowRatio->setValue(_proj.throwRatio());
  ui_->boxWidth->setValue(mapping()->width());
  ui_->boxHeight->setValue(mapping()->height());
  if (mapping()->screen())
    screenChanged(mapping()->screen()->index());
  else
    screenChanged(0);

  updateMode_ = false;
}

omni::Mapping* ProjectorSetupWidget::mapping()
{
  if (!session_) return nullptr;
  return session_->getMapping(mapId_);
}

