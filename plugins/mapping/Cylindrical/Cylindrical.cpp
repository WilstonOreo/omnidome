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

#include "Cylindrical.h"
#include "CylindricalWidget.h"

#include <QDataStream>
#include <QOpenGLShaderProgram>
#include <omni/util.h>
#include <omni/ui/MappingParameters.h>
#include <omni/serialization/PropertyMap.h>

namespace omni
{
  namespace mapping
  {
    Cylindrical::Cylindrical()
    {
    }

    void Cylindrical::bind()
    {
      Interface::bind();
      this->shader_->setUniformValue("angle_begin",GLfloat(beginAngle_));
      this->shader_->setUniformValue("angle_end",GLfloat(endAngle_));
    }

    /// Value between 0.0 and 1.0 for top strip position
    qreal Cylindrical::beginAngle() const {
      return beginAngle_;
    }

    /// Top border position of strip. Value is clamped between 0.0 and 1.0
    void Cylindrical::setBeginAngle(qreal _beginAngle) {
      beginAngle_ = _beginAngle;
      validate();
    }

    /// Value between 0.0 and 1.0 to for bottom strip position
    qreal Cylindrical::endAngle() const {
      return endAngle_;
    }

    /// Bottom border position of strip. Value is clamped between 0.0 and 1.0
    void Cylindrical::setEndAngle(qreal _endAngle) {
      endAngle_ = _endAngle;
      validate();
    }

    void Cylindrical::validate()
    {
      if (beginAngle_ < 0.0) beginAngle_ = 0.0;
      if (endAngle_ > 1.0) endAngle_ = 1.0;
      if (beginAngle_ > endAngle_) std::swap(beginAngle_,endAngle_);
    }

    QWidget* Cylindrical::widget() {
        return ui::makeWidget<omni::ui::mapping::Cylindrical>(this);
    }

    void Cylindrical::toPropertyMap(PropertyMap& _map) const {
      mapping::Interface::toPropertyMap(_map);
      _map("beginAngle",beginAngle_)
          ("endAngle",endAngle_);
    }

    void Cylindrical::fromPropertyMap(PropertyMap const& _map) {
      mapping::Interface::fromPropertyMap(_map);
      _map.get("beginAngle",beginAngle_)
          .get("endAngle",endAngle_);
      validate();
    }
  }
}
