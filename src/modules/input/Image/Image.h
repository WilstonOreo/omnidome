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

#ifndef OMNI_INPUT_IMAGE_H_
#define OMNI_INPUT_IMAGE_H_

#include <omni/input/Interface.h>
#include <omni/media/Location.h>

namespace omni {
  namespace input {
    /// Still image input
    class Image : public QObject, public Interface {
      OMNI_INPUT_PLUGIN_DECL
      OMNI_PLUGIN_CR8TR("Input image", "Copyright (C) 2016")
      public:
        OMNI_REGISTER_CLASS(Factory, Image)

        /// Default Constructor
        Image(Interface const * = nullptr);

        /// Destructor
        ~Image();

        /// Update image
        void     update();

        /// Returns texture ID of image
        GLuint   textureId() const;

        /// Load image from file and stores path
        void     load(QString const& _path);

        /// Reload image from stored path name
        void     reload();

        /// Return stored path
        QString  path() const;

        /// InfoText is basename of stored path
        QString  infoText() const;

        /// Return image dimensions in pixels
        QSize    size() const;

        /// Serialize image path to stream
        void     toStream(QDataStream&) const;

        /// Deserialize from stream and load image
        void     fromStream(QDataStream&);

        bool     canAdd();

      private:
        QImage image_;
        media::Location imageLocation_;
        std::unique_ptr<QOpenGLTexture> texture_;
    };
  }
}

#endif /* OMNI_INPUT_IMAGE_H_ */
