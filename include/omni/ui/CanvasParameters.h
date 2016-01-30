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

#ifndef OMNI_UI_CANVASPARAMETERS_H_
#define OMNI_UI_CANVASPARAMETERS_H_

#include <omni/canvas/Interface.h>
#include <omni/ui/ParameterWidget.h>

namespace omni
{
  namespace ui
  {
    /**@brief Parameter widget for getting and setting canvas parameters
     **/
    class CanvasParameters : public ParameterWidget
    {
      Q_OBJECT
    public:
      CanvasParameters(QWidget* _parent);
      virtual ~CanvasParameters();

      canvas::Interface* canvas();
      canvas::Interface const* canvas() const;
      void setCanvas(canvas::Interface* _canvas);

      /// Set parameters from sliders to canvas
      void updateParameters();
    private:
      slim::Rotation* rotation_ = nullptr;

      virtual void updateCanvasParameters() = 0;

      /// Show widgets parameter according to given canvas type
      void widgetVisibility();

      /// Clear and setup all parameter widgets
      void setup();

      canvas::Interface* canvas_ = nullptr;
    };
  }
}


#endif /* OMNI_UI_CANVASPARAMETERS_H_ */
