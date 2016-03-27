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

#include "proj/MultiSetupParameters.h"

#include <QVBoxLayout>
#include <omni/proj/RingArray.h>

namespace omni {
  namespace ui {
    namespace proj {
      MultiSetupParameters::MultiSetupParameters(QWidget *_parent) :
        ParameterWidget(_parent)
      {
        setLayout(new QVBoxLayout);
      }

      MultiSetupParameters::~MultiSetupParameters()
      {}

      omni::proj::MultiSetup * MultiSetupParameters::multiSetup()
      {
        return multiSetup_;
      }

      omni::proj::MultiSetup const * MultiSetupParameters::multiSetup() const
      {
        return multiSetup_;
      }

      void MultiSetupParameters::setMultiSetup(omni::proj::MultiSetup *_template)
      {
        multiSetup_ = _template;
        setup();
      }

      void MultiSetupParameters::updateParameters()
      {
        if (!multiSetup_ || isLocked()) return;


        if (multiSetup_->getTypeId() == "RingArray")
        {
          auto *_ringArray = static_cast<omni::proj::RingArray *>(multiSetup_);

          _ringArray->setNumberOfProjectors(getParamAsInt("Projectors"));
          _ringArray->setDistanceCenter(getParamAsFloat("Distance To Center"));
          _ringArray->setTowerHeight(getParamAsFloat("Tower Height"));
          _ringArray->setPitch(getParamAsFloat("Pitch"));
          _ringArray->setYaw(getParamAsFloat("Yaw"));
          _ringArray->setFov(getParamAsFloat("FOV"));
        }

        emit parametersUpdated();
      }

      void MultiSetupParameters::setup()
      {
        clear();

        if (!multiSetup_) return;

        this->locked([&]()
        {
          if (multiSetup_->getTypeId() == "RingArray")
          {
            auto *_ringArray = static_cast<omni::proj::RingArray *>(multiSetup_);

            addIntegerWidget("Projectors",
                             _ringArray->numberOfProjectors(), 2, 10);
            addOffsetWidget("Distance To Center", _ringArray->distanceCenter(),
                            0.1, 10.0);
            addOffsetWidget("Tower Height", _ringArray->towerHeight(), -2.0, 2.0);
            addAngleWidget("Pitch", _ringArray->pitch().degrees(), -90.0, 90.0);
            addAngleWidget("Yaw", _ringArray->yaw().degrees(), -180.0, 180.0);
            addAngleWidget("FOV", _ringArray->fov().degrees(), 30.0, 100.0);
          }
        });
      }
    }
  }
}
