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
      OMNI_INPUT_PLUGIN_DECL
      OMNI_PLUGIN_INFO("Syphon client","Copyright (C) 2016")
    public:
      OMNI_REGISTER_CLASS(Factory,Syphon)

      Syphon(Interface const* = nullptr);
      ~Syphon();

      /// Install update timer on activate
      void  activate();

      /// Remove update timer on deactivate
      void     deactivate();

      void setDescription(SyphonServerDescription const& _server);
      SyphonServerDescription const& description() const;

      /// Return size of current syphon input
      QSize size() const;

      /// Return texture id of current syphon texture
      GLuint textureId() const;

      QWidget* widget();

      void update();

      void toPropertyMap(PropertyMap&) const;
      void fromPropertyMap(PropertyMap const&);

    protected:
      void timerEvent(QTimerEvent *);

    private:
	     void* client_ = nullptr;
       void* latestImage_ = nullptr;

       int timerId_ = 0;
       GLuint texId_ = 0;
       bool isSetup_ = false;
       QSize size_;
       SyphonServerDescription description_;
    };
  }
}

#endif /* OMNI_INPUT_SYPHON_H_ */
