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

#include "TexCoords.h"
#include "TexCoordsWidget.h"

#include <QOpenGLShaderProgram>
#include <QDataStream>

namespace omni
{
  namespace mapping
  {
    TexCoords::TexCoords() :
      offset_(0.0,0.0),
      stretch_(1.0,1.0),
      tileHorizontal_(false),
      tileVertical_(false)
    {
    }

    void TexCoords::bind()
    {
      Interface::bind();
      this->shader_->setUniformValue("offset",offset_);
      this->shader_->setUniformValue("stretch",stretch_);
      this->shader_->setUniformValue("tile_horizontal",GLint(tileHorizontal_));
      this->shader_->setUniformValue("tile_vertical",GLint(tileVertical_));
    }

    QVector2D& TexCoords::offset()
    {
      return offset_;
    }

    QVector2D const& TexCoords::offset() const
    {
      return offset_;
    }

    void TexCoords::setOffset(QVector2D const& _offset)
    {
      offset_=_offset;
    }

    QVector2D& TexCoords::stretch()
    {
      return stretch_;
    }

    QVector2D const& TexCoords::stretch() const
    {
      return stretch_;
    }

    void TexCoords::setStretch(QVector2D const& _stretch)
    {
      stretch_ = _stretch;
    }

    bool TexCoords::tileHorizontal() const
    {
      return tileHorizontal_;
    }

    void TexCoords::setTileHorizontal(bool _tileHorizontal)
    {
      tileHorizontal_ = _tileHorizontal;
    }

    bool TexCoords::tileVertical() const
    {
      return tileVertical_;
    }

    void TexCoords::setTileVertical(bool _tileVertical)
    {
      tileVertical_ = _tileVertical;
    }

    void TexCoords::fromPropertyMap(PropertyMap const& _map)
    {
      Interface::fromPropertyMap(_map);
      _map.get("offset",offset_).
           get("stretch",stretch_).
           get("tileHorizontal",tileHorizontal_).
           get("tileVertical",tileVertical_);
    }

    void TexCoords::toPropertyMap(PropertyMap& _map) const
    {
      Interface::toPropertyMap(_map);
      _map("offset",offset_)
          ("stretch",stretch_)
          ("tileHorizontal",tileHorizontal_)
          ("tileVertical",tileVertical_);
    }

    QWidget* TexCoords::widget() {
        return ui::makeWidget<ui::mapping::TexCoords>(this);
    }
  }
}
