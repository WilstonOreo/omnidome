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

#ifndef OMNI_INPUT_SYPHON_H_
#define OMNI_INPUT_SYPHON_H_

#include <omni/input/Interface.h>
#include <QOpenGLFramebufferObject>
#include "SyphonServerDescription.h"

namespace omni
{
  namespace input
  {
    class Syphon : public QObject, public Interface
    {
      Q_OBJECT
      Q_PLUGIN_METADATA(IID OMNI_INPUT_INTERFACE_IID)
      Q_INTERFACES(omni::input::Interface)
      OMNI_PLUGIN_INFO("Syphon client","Copyright (C) 2016")
    public:
      OMNI_REGISTER_CLASS(Factory,Syphon)

      Syphon();
      ~Syphon() final;

      /// Install update timer on activate
      void  activate() override;

      /// Remove update timer on deactivate
      void     deactivate() override;

      void setDescription(SyphonServerDescription const& _server);
      SyphonServerDescription const& description() const;

      /// Return size of current syphon input
      QSize size() const override;

      /// Return texture id of current syphon texture
      GLuint textureId() const override;

      QWidget* widget() override;

      void update() override;

      void toPropertyMap(PropertyMap&) const override;
      void fromPropertyMap(PropertyMap const&) override;

    private:
	     void* client_ = nullptr;
       void* latestImage_ = nullptr;

       GLuint texId_ = 0;
       bool isSetup_ = false;
       QSize size_;
       SyphonServerDescription description_;
    };
  }
}

#endif /* OMNI_INPUT_SYPHON_H_ */
