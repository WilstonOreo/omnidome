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


#ifndef OMNI_PATCH_INTERFACE_H_
#define OMNI_PATCH_INTERFACE_H_

#include <vector>
#include <omni/visual/Interface.h>
#include <omni/serialization/Interface.h>
#include <omni/input/Interface.h>
#include <omni/patch/PatchHandle.h>

namespace omni {
  namespace patch {
    class Interface :
      public visual::Interface,
      public serialization::Interface,
      public TypeIdInterface {
    public:
      virtual ~Interface() {}

      /**@brief Get handles under cursor (const version).
         @detail Position is normalized [0,0] - [1,1].
       **/
      inline std::vector<PatchHandle const*> getHandles(QPointF const& _p) const {
        std::vector<PatchHandle*> _handles = const_cast<Interface*>(this)->getHandles(_p);
        std::vector<PatchHandle const*> _result;
        for (auto& _handle : _handles) {
          _result.push_back(_handle);
        }
        return _result;
      }

      /**@brief Get handles under cursor.
         @detail Position is normalized [0,0] - [1,1].
       **/
      virtual std::vector<PatchHandle*> getHandles(QPointF const&) = 0;

      /// Return pointer to widget
      virtual inline QWidget* widget() { return nullptr; }

      virtual input::Interface* input() = 0;

      virtual void assignInput(input::Interface const*) = 0;
      virtual void removeInput() = 0;

      inline virtual bool canHaveInput() const { return false; }
    };

    /// Patch Factory typedef
    typedef AbstractFactory<Interface> Factory;
  }

  typedef patch::Interface Patch;
  typedef patch::Factory   PatchFactory;
}

#define OMNI_PATCH_INTERFACE_IID "org.omnidome.patch.Interface"

Q_DECLARE_INTERFACE(omni::patch::Interface, OMNI_PATCH_INTERFACE_IID)

#define OMNI_PATCH_PLUGIN_DECL                    \
  Q_OBJECT                                        \
  Q_PLUGIN_METADATA(IID OMNI_PATCH_INTERFACE_IID) \
  Q_INTERFACES(omni::patch::Interface)            \
  OMNI_PLUGIN_TYPE("Patch")


#endif /* OMNI_PATCH_INTERFACE_H_ */
