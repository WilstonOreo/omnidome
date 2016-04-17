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
#include <QDebug>
#include <QErrorMessage>
#include <QFileDialog>
#include <omni/visual/util.h>

namespace omni {
  namespace input {
    Image::Image(Interface const *_parent) :
      Interface(_parent)
    {}

    Image::~Image()
    {}

    void Image::free()
    {
      visual::with_current_context([&](QOpenGLFunctions& _) {
        texture_.reset();
      });
    }

    void Image::update()
    {
      if (image_.width() == 0) return;

      visual::with_current_context([&](QOpenGLFunctions& _) {
        if (texture_) return;

        texture_.reset(new QOpenGLTexture(QOpenGLTexture::TargetRectangle));
        texture_->setData(image_);
        texture_->setMinMagFilters(
          QOpenGLTexture::Linear,
          QOpenGLTexture::Linear);
        texture_->allocateStorage();
      });
    }

    GLuint Image::textureId() const
    {
      return !texture_ ? 0 : texture_->textureId();
    }

    void Image::load(QString const& _path)
    {
      imageLocation_.load(_path,image_);
      image_       = image_.mirrored();
      texture_.reset();
      update();
    }

    void Image::reload()
    {
      load(path());
    }

    QString Image::path() const
    {
      return imageLocation_.path();
    }

    QString Image::infoText() const
    {
      QFileInfo fi(path());
      return fi.baseName();
    }

    QSize Image::size() const
    {
      return image_.size();
    }

    void Image::toStream(QDataStream& _stream) const
    {
      input::Interface::toStream(_stream);

      _stream << imageLocation_;
    }

    void Image::fromStream(QDataStream& _stream)
    {
      input::Interface::fromStream(_stream);

      _stream >> imageLocation_;
      reload();
    }

    bool Image::canAdd() {
      QString _filename = QFileDialog::getOpenFileName(
        widget(), "Open image file...", ".", "Image files (*.png *.jpg *.jpeg)");

      if (_filename.isEmpty()) return false;

      try {
        this->load(_filename);
      } catch (...) {
        QErrorMessage _errorMessageBox(widget());
        _errorMessageBox.showMessage("Error loading image.");
        return false;
      }
      return true;
    }
  }
}
