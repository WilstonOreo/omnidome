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

#ifndef OMNI_UI_PROJ_MULTISETUP_H_
#define OMNI_UI_PROJ_MULTISETUP_H_

#include <omni/TypeIdInterface.h>
#include <omni/Session.h>

namespace omni
{
  class Session;

  namespace proj
  {
    /**@brief Projector setup template interface  
     * @detail Adds one or several projectors/tunings in a certain placement to a session
     **/
    class MultiSetup : public TypeIdInterface
    {
    public: 
      virtual ~MultiSetup() {}

      /// Add tunings to session
      inline void add(Session* _session)
      {
        auto&& _projs = this->projectors();

        for (auto& _proj : _projs)
        {
          auto* _tuning = _session->tunings().add();
          _tuning->projector() = std::move(_proj);
        }
      }

      virtual std::vector<Projector> projectors() const = 0;


    private:
    };

    /// Typedef for our factory
    typedef AbstractFactory<MultiSetup> MultiSetupFactory;

    /// MultiSetup alias for our registrar (for auto registration)
    template<typename T>
    using MultiSetupRegistrar = typename MultiSetupFactory::registrar_type<T>; 
  }
}

#endif /* OMNI_UI_PROJ_MULTISETUP_H_ */

