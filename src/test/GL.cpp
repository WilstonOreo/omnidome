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


#include <omni/ui/GLView3D.h>

#include <omni/Session.h>
#include <omni/canvas/HalfDome.h>
#include <omni/input/Image.h>
#include <omni/input/TestImage.h>
#include <omni/mapping/Equirectangular.h>
#include <omni/proj/FreeSetup.h>
#include <omni/proj/PeripheralSetup.h>

#include <QDataStream>
#include <QFile>
#include <QApplication>

int main(int ac, char* av[])
{
  using namespace omni;
  /// Start gui
  QApplication _a(ac, av);

  Session _session;

  _session.inputs().add<input::Image>(":/spherical.jpg");

  _session.inputs().setCurrentIndex(0);

  for (int i = 0; i < 18; ++i)
  {
    auto* _tuning = _session.tunings().add();
  
    auto* _projSetup = static_cast<proj::PeripheralSetup*>(_tuning->projector().setup("PeripheralSetup"));

    _projSetup->setYaw(i*20);
    _projSetup->setPitch(i*5);
    _projSetup->setDistanceCenter(9.0);
    _projSetup->setup(const_cast<proj::Projector&>(_tuning->projector()));
  }

  Q_ASSERT(_session.setCanvas("HalfDome"));

  
  ui::GLView3D _view;

  _view.show();
  _view.setSession(&_session);

  return _a.exec();
}

