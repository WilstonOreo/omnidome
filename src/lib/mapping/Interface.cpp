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

#include <QOpenGLShaderProgram>


namespace omni
{
  namespace mapping
  {
    Interface::Interface()
    {
    }

    Interface::~Interface()
    {
    }

    void Interface::initialize()
    {
      /// Make shader
      shader_.reset(new QOpenGLShaderProgram());
      shader_->addShaderFromSourceCode(QOpenGLShader::Vertex,vertexShaderSourceCode());
      shader_->addShaderFromSourceCode(QOpenGLShader::Fragment,fragmentShaderSourceCode());
      shader_->link();
    }

    void Interface::bind(OutputMode _outputMode)
    {
      if (!shader_) initialize();

      if (shader_)
      {
        shader_->bind();
        shader_->setUniformValue("flip_horizontal",flipHorizontal_);
        shader_->setUniformValue("flip_vertical",flipVertical_);

        switch (_outputMode)
        {
          case OutputMode::MAPPED_INPUT:
          shader_->setUniformValue("output_mode",0);
          break;
          case OutputMode::TEXCOORDS:
          shader_->setUniformValue("output_mode",1);
          break;
          case OutputMode::UVW:
          shader_->setUniformValue("output_mode",2);
          break;
        };
      }
    }

    void Interface::release()
    {
      if (shader_)
        shader_->release();
    }

    void Interface::fromStream(QDataStream&)
    {
    }

    void Interface::toStream(QDataStream&) const
    {
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
        if (_mapping->isUVW())
          _ids.insert(_id);
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
        if (!_mapping->isUVW())
          _ids.insert(_id);
      }
      return _ids;
    }

    QString Interface::vertexShaderSourceCode() const
    {
      return util::fileToStr(":/shaders/mapping/common.vert");
    }

    QString Interface::fragmentShaderSourceCode() const
    {
      return
        util::fileToStr(":/shaders/mapping/Template.frag") +
        util::fileToStr(QString(":/shaders/mapping/") + getTypeId().str() + ".frag");
    }
  }
}
