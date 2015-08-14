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

#ifndef OMNI_CANVAS_INFLATABLEDOME_H_
#define OMNI_CANVAS_INFLATABLEDOME_H_

#include <omni/canvas/Dome.h>
#include <omni/canvas/FullSphere.h>

namespace omni
{
  namespace canvas
  {
    /**@brief A fullsphere dome with z offset and squeeze factor
     **/
    class InflatableDome : 
      public FullSphere, 
      Registrar<InflatableDome>
    {
    public: 
      OMNI_TYPEID("InflatableDome")

      InflatableDome();
      ~InflatableDome();
      
      float squeeze() const;
      void setSqueeze(float _squeeze);

      void draw() const;
      void update();

    private:
      float squeeze_ = 1.0;
    };
  }

  using canvas::InflatableDome;
}

#endif /* OMNI_DOME_INFLATABLE_H_ */
