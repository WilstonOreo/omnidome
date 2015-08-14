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

#include <omni/ui/ProjectorGroupBox.h>

#include <QVBoxLayout>
#include <QFocusEvent>

using namespace omni::ui;

ProjectorGroupBox::ProjectorGroupBox(QWidget* _parent) :
  QGroupBox(_parent),
  index_(0)
{
  setCheckable(true);
}

ProjectorGroupBox::ProjectorGroupBox(const Color4f& _color, size_t _index) :
  QGroupBox(nullptr)
{
  setColorAndIndex(_color,_index);
}

void ProjectorGroupBox::setColorAndIndex(const Color4f& _color, size_t _index)
{
  setCheckable(true);
  setTitle(QString("Projector #%1").arg(_index+1));
  index_=_index;
  QString _qColorStr = QColor(_color.r()*255,_color.g()*255,_color.b()*255).name();
  QString s = QString(
                "QGroupBox { border: 2px solid %1;"
                "border-radius: 9px; margin-top: 18px; } "
                "QGroupBox::title { color: %2; "
                " background-color: qradialgradient(cx:0.5, cy:0.5, radius: 0.5,"
                " fx:0.5, fy:0.5, stop:0.2 black, stop:1 transparent) ; subcontrol-origin: margin;"
                "subcontrol-position: top left; padding-left: 10px;"
                "padding-right: 10px; }");

  setStyleSheet(s.arg(_qColorStr).arg(_qColorStr));
}

void ProjectorGroupBox::focusInEvent ( QFocusEvent * event )
{
  emit setFocus(index_);
  QGroupBox::focusInEvent(event);
}
