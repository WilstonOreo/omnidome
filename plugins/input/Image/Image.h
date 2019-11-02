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

#ifndef OMNI_INPUT_IMAGE_H_
#define OMNI_INPUT_IMAGE_H_

#include <omni/visual/ContextBoundPtr.h>
#include <omni/input/Interface.h>
#include <omni/media/Location.h>

namespace omni {
  namespace input {
    /// Still image input
    class OMNIPLUGIN_EXPORT Image : public QObject, public Interface {
        Q_OBJECT
        Q_PLUGIN_METADATA(IID OMNI_INPUT_INTERFACE_IID)
        Q_INTERFACES(omni::input::Interface)
        OMNI_PLUGIN_INFO("Input image", "Copyright (C) 2016")
      public:
        OMNI_REGISTER_CLASS(Factory, Image)

        /// Default Constructor
        Image() {}

        /// Destructor
        ~Image() final = default;

        /// Update image
        void     update() override;

        /// Returns texture ID of image
        GLuint   textureId() const override;

        /// Load image from file and stores path
        void     load(QString const& _path);

        /// Reload image from stored path name
        void     reload();

        /// Return stored path
        QString  path() const;

        /// InfoText is basename of stored path
        QString  infoText() const override;

        /// Return image dimensions in pixels
        QSize    size() const override;

        /// Serialize image path to stream
        void     toPropertyMap(PropertyMap&) const override;

        /// Deserialize from stream and load image
        void     fromPropertyMap(PropertyMap const&) override;

        bool     canAdd() override;

        /// Input widget
        QWidget* widget() override;

      private:
        QImage image_;
        media::Location imageLocation_;
        ContextBoundPtr<QOpenGLTexture> texture_;
    };
  }
}

#endif /* OMNI_INPUT_IMAGE_H_ */
