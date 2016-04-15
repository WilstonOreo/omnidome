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

#include <omni/mapping/Interface.h>

#include <omni/input/List.h>
#include <omni/util.h>
#include <omni/serialization/PropertyMap.h>

#include <QOpenGLShaderProgram>

namespace omni {
    namespace mapping {
        Interface::Interface()
        {
            // Disable scaling and translation by default
            transform_.setTranslationEnabled(false);
            transform_.setScaleEnabled(false);
        }

        Interface::~Interface()
        {}

        void Interface::initialize()
        {
            /// Make shader
            shader_.reset(new QOpenGLShaderProgram());
            shader_->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                             vertexShaderSourceCode());
            shader_->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                             fragmentShaderSourceCode());
            shader_->link();
        }

        void Interface::bind() {
            if (!shader_) initialize();

            if (shader_)
            {
                shader_->bind();
                shader_->setUniformValue("flip_horizontal", flipHorizontal_);
                shader_->setUniformValue("flip_vertical",   flipVertical_);

                if (isUVW()) {
                    shader_->setUniformValue("bound_to_canvas", boundToCanvas_);
                    shader_->setUniformValue("matrix",          matrix());
                }
            }
        }

        void Interface::bind(input::Interface const* _input, OutputMode _outputMode, bool _grayscale)
        {
            bind();
            if (shader_)
            {
                shader_->setUniformValue("output_mode",
                                                        util::enumToInt(
                                             _outputMode));

                shader_->setUniformValue("texture",   0);
                shader_->setUniformValue("texture_size",QVector2D(_input->width(),_input->height()));
                shader_->setUniformValue("gray_output", _grayscale);
            }
        }

        void Interface::release()
        {
            if (shader_) shader_->release();
        }

        bool Interface::flipHorizontal() const
        {
            return flipHorizontal_;
        }

        void Interface::setFlipHorizontal(bool _flipHorizontal)
        {
            flipHorizontal_ = _flipHorizontal;
        }

        bool Interface::flipVertical() const
        {
            return flipVertical_;
        }

        void Interface::setFlipVertical(bool _flipVertical)
        {
            flipVertical_ = _flipVertical;
        }

        IdSet Interface::getUVWMappings()
        {
            IdSet _ids;

            for (auto& _mappingId : Factory::classes())
            {
                auto& _id = _mappingId.first;
                std::unique_ptr<Mapping> _mapping(Factory::create(_id));

                if (_mapping->isUVW()) _ids.insert(_id);
            }
            return _ids;
        }

        IdSet Interface::getPlanarMappings()
        {
            IdSet _ids;

            for (auto& _mappingId : Factory::classes())
            {
                auto& _id = _mappingId.first;
                std::unique_ptr<Mapping> _mapping(Factory::create(_id));

                if (!_mapping->isUVW()) _ids.insert(_id);
            }
            return _ids;
        }

        /// Return const ref to affine transform
        AffineTransform const& Interface::transform() const {
            return transform_;
        }

        /// Return ref to affine transform
        AffineTransform& Interface::transform() {
            return transform_;
        }

        /// Set new affine transform
        void Interface::setTransform(AffineTransform const& _transform) {
            transform_ = _transform;
        }

        /**@brief If true, mapping transform is attached to canvas transform
           @detail Is true by default
         **/
        bool Interface::isBoundToCanvas() const {
            return boundToCanvas_;
        }

        /// Set whether mapping transform is attached to canvas transform
        void Interface::setBoundToCanvas(bool _boundToCanvas) {
            boundToCanvas_ = _boundToCanvas;
        }

        QMatrix4x4 Interface::matrix() const {
            return transform_.matrix();
        }

        QString Interface::vertexShaderSourceCode() const
        {
            return util::fileToStr(":/shaders/mapping/common.vert");
        }

        QString Interface::fragmentShaderSourceCode() const
        {
            return
                util::fileToStr(":/shaders/mapping/Template.frag") +
                util::fileToStr(QString(
                                    ":/shaders/mapping/") + getTypeId().str() +
                                ".frag");
        }

        /// Write mapping to stream
        void Interface::toStream(QDataStream& _os) const {
            PropertyMap _map;
            _map("transform",transform_)
                ("boundToCanvas",boundToCanvas_)
                ("flipHorizontal",flipHorizontal_)
                ("flipVertical",flipVertical_);
            _os << _map;
        }

        /// Read mapping from stream
        void Interface::fromStream(QDataStream& _is) {
            PropertyMap _map(_is);
            _map.get("transform",transform_)
                .get("boundToCanvas",boundToCanvas_)
                .get("flipHorizontal",flipHorizontal_)
                .get("flipVertical",flipVertical_);
        }
    }
}
