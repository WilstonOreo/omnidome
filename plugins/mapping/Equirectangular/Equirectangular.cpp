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

#include "Equirectangular.h"


#include <QDataStream>
#include <QOpenGLShaderProgram>
#include <omni/util.h>
#include <omni/serialization/PropertyMap.h>
#include "EquirectangularWidget.h"

namespace omni
{
  namespace mapping
  {
    Equirectangular::Equirectangular()
    {
    }

    void Equirectangular::bind()
    {
      Interface::bind();
      this->shader_->setUniformValue("strip_top",GLfloat(stripTop_));
      this->shader_->setUniformValue("strip_bottom",GLfloat(stripBottom_));
    }

    qreal Equirectangular::stripTop() const
    {
      return stripTop_;
    }

    void Equirectangular::setStripTop(qreal _stripTop)
    {
      stripTop_ = _stripTop;
      validate();
    }

    qreal Equirectangular::stripBottom() const
    {
      return stripBottom_;
    }

    void Equirectangular::setStripBottom(qreal _stripBottom)
    {
      stripBottom_ = _stripBottom;
      validate();
    }

    void Equirectangular::validate()
    {
      if (stripBottom_ < 0.0) stripBottom_ = 0.0;
      if (stripTop_ > 1.0) stripTop_ = 1.0;
      if (stripBottom_ > stripTop_) std::swap(stripBottom_,stripTop_);
    }

    void Equirectangular::fromPropertyMap(PropertyMap const& _map)
    {
      mapping::Interface::fromPropertyMap(_map);
      _map.get("stripBottom",stripBottom_)
          .get("stripTop",stripTop_);
      validate();
    }

    void Equirectangular::toPropertyMap(PropertyMap& _map) const
    {
      _map("stripBottom",stripBottom_)
          ("stripTop",stripTop_);
      mapping::Interface::toPropertyMap(_map);
    }

    bool operator==(Equirectangular const& _lhs, Equirectangular const& _rhs)
    {
      return
        OMNI_TEST_MEMBER_EQUAL(stripTop_) &&
        OMNI_TEST_MEMBER_EQUAL(stripBottom_);
    }

    QWidget* Equirectangular::widget() {
        return ui::makeWidget<ui::mapping::Equirectangular>(this);
    }
  }
}
