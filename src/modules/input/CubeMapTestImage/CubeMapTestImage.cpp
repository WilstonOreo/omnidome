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

#include "CubeMapTestImage.h"

#include <QDebug>
#include <omni/util.h>
#include "CubeMapTestImageWidget.h"

namespace omni
{
  namespace input
  {
    CubeMapTestImage::CubeMapTestImage(Interface const* _parent)  :
      TestImage(_parent)
    {
    }

    CubeMapTestImage::~CubeMapTestImage()
    {
    }

    QString CubeMapTestImage::fragmentShaderSource() const
    {
      static QString _fragmentSrc;
      if (_fragmentSrc.isEmpty())
        _fragmentSrc = util::fileToStr(":/shaders/CubeMapTestImage.frag");
      return _fragmentSrc;
    }

    QString CubeMapTestImage::vertexShaderSource() const
    {
      static QString _vertSrc;
      if (_vertSrc.isEmpty())
        _vertSrc = util::fileToStr(":/shaders/test_image.vert");
      return _vertSrc;
    }

    void CubeMapTestImage::setFlipText(bool _flipText) {
        flipText_ = _flipText;
        update();
    }

    bool CubeMapTestImage::flipText() const {
        return flipText_;
    }

    void CubeMapTestImage::setDisplayNumbers(bool _displayNumbers) {
        displayNumbers_ = _displayNumbers;
        update();
    }

    bool CubeMapTestImage::displayNumbers() const {
        return displayNumbers_;
    }

    void CubeMapTestImage::shaderUniformHandler() {
        TestImage::shaderUniformHandler();
        this->shader_->setUniformValue("flip_text",flipText_);
        this->shader_->setUniformValue("display_numbers",displayNumbers_);
    }

    QSize CubeMapTestImage::size() const
    {
      return QSize(6144,1024);
    }

    QWidget* CubeMapTestImage::widget() {
        return new omni::ui::input::CubeMapTestImage(this);
    }

    void CubeMapTestImage::toStream(QDataStream& _stream) const
    {
      TestImage::toStream(_stream);
      _stream << flipText_;
    }

    void CubeMapTestImage::fromStream(QDataStream& _stream)
    {
      TestImage::fromStream(_stream);
      _stream >> flipText_;
      update();
    }
  }
}
