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
#ifndef OMNI_INPUT_TESTIMAGE_H_
#define OMNI_INPUT_TESTIMAGE_H_

#include <QOpenGLTexture>
#include <omni/visual/ContextBoundPtr.h>
#include <omni/input/Framebuffer.h>

namespace omni
{
  namespace input
  {
    /// Test image for equirectangular mappings
    class TestImage : public QObject, public Framebuffer
    {
      Q_OBJECT
      Q_PLUGIN_METADATA(IID OMNI_INPUT_INTERFACE_IID)
      Q_INTERFACES(omni::input::Interface)
      OMNI_PLUGIN_INFO("Test image","Copyright (C) 2016")
    public:
      OMNI_REGISTER_CLASS(Factory,TestImage)

      enum class Mode {
        FISHEYE,
        EQUIRECTANGULAR,
        CUBEMAP
      };

      TestImage();
      ~TestImage() final = default;

      QSize size() const override;

      QWidget* widget() override;

      void update() override;

      void setMode(Mode);
      Mode mode() const;

      /// Return ruler position
      QPointF      rulerPos() const;

      /// Set ruler position
      void         setRulerPos(QPointF const&);

      void setFlipText(bool);
      bool flipText() const;

      void setDisplayNumbers(bool);
      bool displayNumbers() const;

      void toPropertyMap(PropertyMap&) const override;
      void fromPropertyMap(PropertyMap const&) override;

    private:
      static ContextBoundPtr<QOpenGLTexture> polarTex_;
      static ContextBoundPtr<QOpenGLShaderProgram> sphericalShader_;
      static ContextBoundPtr<QOpenGLShaderProgram> cubeMapShader_;

      QPointF rulerPos_;

      Mode mode_ = Mode::FISHEYE;
      bool flipText_ = true;
      bool displayNumbers_ = true;
    };
  }
}

OMNI_DECL_ENUM_STREAM_OPERATORS(omni::input::TestImage::Mode);


#endif /* OMNI_INPUT_EQUIRECTANGULAR_TESTIMAGE_H_ */
