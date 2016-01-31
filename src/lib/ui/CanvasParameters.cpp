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

#include <omni/ui/CanvasParameters.h>

namespace omni
{
  namespace ui
  {
    CanvasParameters::CanvasParameters(QWidget* _parent) :
      ParameterWidget(_parent)
    {
    }

    CanvasParameters::CanvasParameters(
        canvas::Interface* _canvas,
        QWidget* _parent) :
      ParameterWidget(_parent)
    {
        QLayout *_layout = new QVBoxLayout;
        _layout->setSpacing(2);
        _layout->setContentsMargins(0, 0, 0, 0);
        setLayout(_layout);
        setCanvas(_canvas);
    }

    CanvasParameters::~CanvasParameters()
    {
    }

    canvas::Interface* CanvasParameters::canvas()
    {
      return canvas_;
    }

    canvas::Interface const* CanvasParameters::canvas() const
    {
      return canvas_;
    }

    void CanvasParameters::setCanvas(omni::canvas::Interface* _canvas)
    {
      canvas_ = _canvas;
    }

    void CanvasParameters::updateParameters()
    {
      if (!canvas_ || isLocked()) return;

      this->updateCanvasParameters();

      canvas_->update();
      emit parametersUpdated();
    }

  }
}
