/* Copyright (c) 2014-2015 "Omnidome" by Michael Winkelmann
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

#include "About.h"

#include "ui_omni_ui_About.h"

using namespace omni::ui;

About::About(QWidget *_parent) :
  QDialog(_parent),
  ui_(new Ui::About())
{
  ui_->setupUi(this);
  ui_->lbVersion->setText(QString("Version %1").arg(OMNI_VERSION_STRING));
  ui_->lbRevision->setText(QString("From revision %1").arg(OMNI_GIT_REVISION));
}

About::~About()
{}
