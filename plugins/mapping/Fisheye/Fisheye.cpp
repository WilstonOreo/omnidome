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

#include "Fisheye.h"
#include "FisheyeWidget.h"

#include <QDataStream>
#include <QOpenGLShaderProgram>
#include <omni/serialization/PropertyMap.h>

namespace omni
{
  namespace mapping
  {
    Fisheye::Fisheye()
    {
    }

    void Fisheye::bind()
    {
      Interface::bind();
      this->shader_->setUniformValue("stretch",GLfloat(stretch_));
    }

    qreal Fisheye::stretch() const
    {
      return stretch_;
    }

    void Fisheye::setStretch(qreal _stretch)
    {
      if (_stretch <  0.0) _stretch = 0.0;
      if (_stretch >= 1.0) _stretch = 1.0;
      stretch_ = _stretch;
    }

    void Fisheye::fromPropertyMap(PropertyMap const& _map)
    {
      mapping::Interface::fromPropertyMap(_map);
      _map.get("stretch",stretch_);
      setStretch(stretch_);
    }

    void Fisheye::toPropertyMap(PropertyMap& _map) const
    {
      _map("stretch",stretch_);
      mapping::Interface::toPropertyMap(_map);
    }

    QWidget* Fisheye::widget() {
        return ui::makeWidget<ui::mapping::Fisheye>(this);
    }
  }
}
