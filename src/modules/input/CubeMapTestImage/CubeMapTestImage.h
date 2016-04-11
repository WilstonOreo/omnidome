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

#ifndef OMNI_INPUT_CUBEMAP_TESTIMAGE_H_
#define OMNI_INPUT_CUBEMAP_TESTIMAGE_H_

#include <omni/input/TestImage.h>

namespace omni
{
  namespace input
  {
    class CubeMapTestImage : public QObject, public TestImage
    {
      OMNI_INPUT_PLUGIN_DECL
      OMNI_PLUGIN_CR8TR("CubeMap test image","Copyright (C) 2016")
    public:
      OMNI_REGISTER_CLASS(Factory,CubeMapTestImage)

      CubeMapTestImage(Interface const* = nullptr);
      ~CubeMapTestImage();

      QSize size() const;

      QWidget* widget();

      void setFlipText(bool);
      bool flipText() const;

      void setDisplayNumbers(bool);
      bool displayNumbers() const;

      void toStream(QDataStream&) const;
      void fromStream(QDataStream&);

    private:
      QString fragmentShaderSource() const;
      QString vertexShaderSource() const;
      void shaderUniformHandler();

      bool flipText_ = false;
      bool displayNumbers_ = true;
    };
  }
}

#endif /* OMNI_INPUT_CUBEMAP_TESTIMAGE_H_ */
