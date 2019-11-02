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

#ifndef OMNI_UI_MAPPINGPARAMETERS_H_
#define OMNI_UI_MAPPINGPARAMETERS_H_

#include <omni/mapping/Interface.h>
#include <omni/ui/ParameterWidget.h>
#include <omni/ui/mixin/DataModel.h>

namespace omni
{
  namespace ui
  {
    /**@brief Parameter widget for getting and setting mapping parameters
     **/
    class OMNIWIDGET_EXPORT MappingParameters :
        public ParameterWidget,
        public mixin::UnsharedDataModel<MappingParameters,mapping::Interface>
    {
      Q_OBJECT
      OMNI_UI_UNSHARED_DATAMODEL(MappingParameters,mapping::Interface)

    public:
      MappingParameters(QWidget* _parent = nullptr);
      ~MappingParameters() override;

    signals:
        void dataModelChanged();
    protected:
      virtual void dataToFrontend() override;

                /// Return true if data has changed by front end
      virtual bool frontendToData() override;

      /// Add flip and bound to canvas parameters
      void addDefaultParameters();

      omni::ui::AffineTransform* transform_ = nullptr;
    };
  }
}


#endif /* OMNI_UI_MAPPINGPARAMETERS_H_ */
