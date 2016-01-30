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

#include "Image.h"

#include <QOpenGLContext>
#include <QFileInfo>
#include <omni/visual/util.h>
#include <QDebug>

namespace omni
{
  namespace input
  {
    Image::Image()
    {
    }

    Image::Image(QString const& _path)
    {
      load(_path);
    }

    Image::~Image()
    {
    }

    void Image::free()
    {
        needsUpdate_ = true;
        texture_.reset();
    }

    void Image::update()
    {
      if (!needsUpdate_ || image_.width() == 0) return;

      visual::with_current_context([&](QOpenGLFunctions& _) {
          texture_.reset(new QOpenGLTexture(image_));
          texture_->setMinMagFilters(
              QOpenGLTexture::Linear,
              QOpenGLTexture::Linear);
              needsUpdate_ = false;
      });
    }

    GLuint Image::textureId() const
    {
      return !texture_ ? 0 : texture_->textureId();
    }

    void Image::load(QString const& _path)
    {
      image_ = QImage(_path).mirrored();
      path_=_path;
      needsUpdate_ = true;
    }

    void Image::reload()
    {
      if (path_.isEmpty()) return;

      load(path_);
    }

    QString Image::path() const
    {
      return path_;
    }

    QString Image::infoText() const
    {
      QFileInfo fi(path_);
      return fi.baseName();
    }

    QSize Image::size() const
    {
      return image_.size();
    }

    void Image::toStream(QDataStream& _stream) const
    {
      _stream << path_;
    }

    void Image::fromStream(QDataStream& _stream)
    {
      _stream >> path_;
      reload();
    }

    QWidget* Image::widget() {
        return nullptr;
    }
  }
}
