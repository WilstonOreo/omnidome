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

#ifndef OMNI_INPUT_INTERFACE_H_
#define OMNI_INPUT_INTERFACE_H_

#include <set>
#include <QObject>
#include <QOpenGLTexture>
#include <omni/SerializationInterface.h>
#include <omni/mapping/Interface.h>

namespace omni
{
  namespace input
  {
    /// Generic input interface
    class Interface : public SerializationInterface
    {
    public:
      typedef Interface interface_type;

      /// Virtual destructor
      virtual ~Interface();

      /// Return supported map modes (if set is empty, all map modes are supported)
      inline virtual MappingModeSet mapModes() const
      {
        return  MappingModeSet({  });
      }

      /// An input must return an OpenGL texture ID
      virtual GLuint textureId() const = 0;

      /// Update interface
      inline virtual void update() {}

      /**@brief Free stored OpenGL Content (like textures, shaders etc)
       * @detail Is called before destructor, when there is still an active OpenGL context
       **/
      inline virtual void free() {}

      /// Returns an optional string with some basic information about the object
      inline virtual QString infoText() const
      {
        return QString();
      };

      /// An input must return width and height information
      inline virtual QSize size() const = 0;

      /// Return width from size
      inline int width() const
      {
        return size().width();
      }

      /// Return height from size
      inline int height() const
      {
        return size().height();
      }

      virtual QWidget* widget() = 0;

      /**@brief An Input may have an optional ruler position
       * @detail Position is between (0.0,0.0) and (1.0,1.0)
       **/
      virtual inline QPointF rulerPos() const
      {
        return QPointF(-1.0,-1.0);
      }

      /// Set ruler position.
      virtual inline void setRulerPos(QPointF const&) {}
    };

    /// Input Factory typedef
    typedef AbstractFactory<Interface> Factory;
  }

  typedef input::Interface Input;
  typedef input::Factory InputFactory;
}

#define OMNI_INPUT_INTERFACE_IID "org.omnidome.input.Interface"

Q_DECLARE_INTERFACE(omni::input::Interface, OMNI_INPUT_INTERFACE_IID)

#define OMNI_INPUT_PLUGIN_DECL \
    Q_OBJECT \
    Q_PLUGIN_METADATA(IID OMNI_INPUT_INTERFACE_IID) \
    Q_INTERFACES(omni::input::Interface)

#endif /* OMNI_INPUT_INTERFACE_H_ */
