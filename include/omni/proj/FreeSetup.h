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

#ifndef OMNI_PROJ_FREESETUP_H_
#define OMNI_PROJ_FREESETUP_H_

#include <QVector3D>
#include "AngleSetup.h"

namespace omni
{
  namespace proj
  {
    class FreeSetup : 
      public AngleSetup,
      SetupRegistrar<FreeSetup>
    {
    public:
      OMNI_TYPEID("FreeSetup")

      FreeSetup();

      void toStream(QDataStream&) const;
      void fromStream(QDataStream&);
      void setup(Projector&);

      QVector3D pos() const;
      void setPos(QVector3D const&);
      void setPos(float _x, float _y, float _z);
      
    private:
      QVector3D pos_;
    };
  }
}

#endif /* OMNI_PROJ_FREESETUP_H_ */
