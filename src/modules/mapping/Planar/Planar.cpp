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

#include "Planar.h"
#include "PlanarWidget.h"

#include <QOpenGLShaderProgram>
#include <QDataStream>

namespace omni
{
  namespace mapping
  {
    Planar::Planar() :
      offset_(0.0,0.0),
      stretch_(1.0,1.0),
      tileHorizontal_(false),
      tileVertical_(false)
    {
    }

    Planar::~Planar()
    {
    }

    Mode Planar::mode() const
    {
      return Mode::PLANAR;
    }

    void Planar::bind(OutputMode _outputMode)
    {
      Interface::bind(_outputMode);
      this->shader_->setUniformValue("offset",offset_);
      this->shader_->setUniformValue("stretch",stretch_);
      this->shader_->setUniformValue("tile_horizontal",GLint(tileHorizontal_));
      this->shader_->setUniformValue("tile_vertical",GLint(tileVertical_));
    }

    QVector2D& Planar::offset()
    {
      return offset_;
    }

    QVector2D const& Planar::offset() const
    {
      return offset_;
    }

    void Planar::setOffset(QVector2D const& _offset)
    {
      offset_=_offset;
    }

    QVector2D& Planar::stretch()
    {
      return stretch_;
    }

    QVector2D const& Planar::stretch() const
    {
      return stretch_;
    }

    void Planar::setStretch(QVector2D const& _stretch)
    {
      stretch_ = _stretch;
    }

    bool Planar::tileHorizontal() const
    {
      return tileHorizontal_;
    }

    void Planar::setTileHorizontal(bool _tileHorizontal)
    {
      tileHorizontal_ = _tileHorizontal;
    }

    bool Planar::tileVertical() const
    {
      return tileVertical_;
    }

    void Planar::setTileVertical(bool _tileVertical)
    {
      tileVertical_ = _tileVertical;
    }

    void Planar::fromStream(QDataStream& _stream)
    {
      Interface::fromStream(_stream);
      _stream >> offset_ >> stretch_ >> tileHorizontal_ >> tileVertical_;
    }

    void Planar::toStream(QDataStream& _stream) const
    {
      Interface::toStream(_stream);
      _stream << offset_ << stretch_ << tileHorizontal_ << tileVertical_;
    }

    QWidget* Planar::widget() {
        return new ui::mapping::Planar(this);
    }
  }
}
