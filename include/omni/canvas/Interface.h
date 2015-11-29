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

#ifndef OMNI_CANVAS_INTERFACE_H_
#define OMNI_CANVAS_INTERFACE_H_

#include <set>
#include <QMatrix4x4>
#include <omni/SerializationInterface.h>
#include <omni/mapping/Interface.h>
#include <omni/visual/Interface.h>
#include <omni/Box.h>

namespace omni
{
  namespace canvas
  {
    /**@brief Abstract interface for a canvas
     * @detail A canvas represents the surface on which the projection is performed.
     *         It might be a dome or a planar surface.
     **/
    class Interface :
      public SerializationInterface,
      public visual::Interface
    {
    public:

      /// Virtual destructor
      virtual ~Interface() {}

      /// Draws auxiliary elements which are not used for mapping (e.g. positioning grids)
      inline virtual void drawAux() const
      {
      }

      /// Returns set of supported map modes (an empty set means that all map modes are supported)
      inline virtual MappingModeSet supportedMappingModes() const
      {
        return MappingModeSet();
      }

      inline virtual MappingMode defaultMappingMode() const {
          return MappingMode::NONE;
      }

      /// Abstract method for returning bounding box of canvas
      virtual Box bounds() const = 0;

      /// Spectator's center of canvas (center of bounding box by default)
      inline virtual QVector3D center() const
      {
        return (bounds().max() + bounds().min()) * 0.5;
      }

      /**@brief Flag which tells if this canvas supports UVW mapping modes (true by default)
       **/
      virtual bool supportsUVWMapping() const {
        return true;
      }

      /**@brief Returns overall extent of canvas.
       * @detail Value is needed for defining ranges for projector positioning
       *         Is the length of the diagonal vector of the bounding box by default.
       ***/
      inline virtual qreal extent() const
      {
        return bounds().size().length();
      }

      /// Canvas radius (is half of size by default)
      inline virtual qreal radius() const
      {
        return extent() * 0.5;
      }

      /// Transformation matrix for canvas
      inline virtual QMatrix4x4 matrix() const
      {
        return QMatrix4x4();
      }

    protected:
      bool needsUpdate_ = true;
    };

    /// Our canvas factory
    typedef AbstractFactory<Interface> Factory;

    /// Template alias for our canvas registrar (for auto registration)
    template<typename T>
    using Registrar = typename Factory::registrar_type<T>;
  }

  typedef canvas::Interface Canvas;
  typedef canvas::Factory CanvasFactory;

  template<typename T>
  using CanvasRegistrar = canvas::Registrar<T>;
}

#endif /* OMNI_CANVAS_INTERFACE_H_ */
