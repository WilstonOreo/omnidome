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

#include "SceneViewer.h"
#include "ui_omni_ui_SceneViewer.h"

namespace omni {
  namespace ui {
    SceneViewer::SceneViewer(QWidget *_parent) :
      QWidget(_parent),
      ui_(new Ui::SceneViewer)
    {
      ui_->setupUi(this);
      ui_->toolBar->hide();
      ui_->inputWidget->setLayout(new QVBoxLayout);
      removeInputControlWidget();
    }

    SceneViewer::~SceneViewer() {}

    void SceneViewer::dataToFrontend() {
      ui_->view->setDataModel(dataModel());
    }

    SceneGLView * SceneViewer::view() {
      return ui_->view;
    }

    SceneGLView const * SceneViewer::view() const {
      return ui_->view;
    }

    bool SceneViewer::frontendToData() {
      return false;
    }

    /// Show large widget from current input
    void SceneViewer::showInputControlWidget() {
      if (!dataModel()) return;
      showInputControlWidget(dataModel()->inputs().current());
    }

    /// Show large widget from given input
    void SceneViewer::showInputControlWidget(omni::input::Interface* _input) {
      removeInputControlWidget();
      if (!_input) return;

      inputControlWidget_ = _input->controlWidget();
      if (!inputControlWidget_) return;

      ui_->inputWidget->layout()->addWidget(_input->controlWidget());

      QList<int> _sizes;
      _sizes << width() / 2;
      _sizes << width() / 2;
      ui_->splitter->setSizes(_sizes);
      ui_->inputWidget->show();
    }

    /// Remove large input widget
    void SceneViewer::removeInputControlWidget() {
      ui_->inputWidget->hide();
      QList<int> _sizes;
      _sizes << width();
      _sizes << 0;
      ui_->splitter->setSizes(_sizes);

      if (!inputControlWidget_) return;

      ui_->inputWidget->layout()->removeWidget(inputControlWidget_);
      inputControlWidget_->deleteLater();
      inputControlWidget_ = nullptr;
    }
  }
}
