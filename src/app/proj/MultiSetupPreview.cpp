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

#include "proj/MultiSetupPreview.h"

#include <omni/visual/util.h>
#include <omni/visual/Projector.h>
#include <omni/proj/PeripheralSetup.h>

namespace omni
{
  namespace ui
  {
    namespace proj
    {
      MultiSetupPreview::MultiSetupPreview(QWidget* _parent) :
        GLView3D(_parent)
      {
      }

      MultiSetupPreview::~MultiSetupPreview()
      {
      }

      omni::proj::MultiSetup* MultiSetupPreview::multiSetup()
      {
        return multiSetup_;
      }

      omni::proj::MultiSetup const* MultiSetupPreview::multiSetup() const
      {
        return multiSetup_;
      }
      void MultiSetupPreview::setMultiSetup(omni::proj::MultiSetup* _multiSetup)
      {
        multiSetup_ = _multiSetup;
        updateProjectors();
      }

      void MultiSetupPreview::updateProjectors()
      {
        if (!multiSetup_) return;

        projectorViz_.clear();

        projectors_ = multiSetup_->projectors();
        for (auto& _projector : projectors_)
        {
          visual::Projector _projViz(_projector);
          _projViz.setColor("#00ccff");
          /// Make new projector visualizer
          projectorViz_.emplace_back(_projViz);
        }
        update();
      }

      void MultiSetupPreview::paintGL()
      {
        if (!session()) return;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        visual::viewport(this);

        this->setupCamera();

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        updateLight();

        visual::with_current_context([&](QOpenGLFunctions& _)
        {
          auto* _canvas = session()->canvas();

          _.glEnable(GL_LIGHTING);
          _.glEnable(GL_COLOR_MATERIAL);
          _.glEnable(GL_DEPTH_TEST);
          if (_canvas)
          {
            this->session_->drawCanvas(mapping::OutputMode::LIGHTING_ONLY);
          }

          _.glDisable(GL_DEPTH_TEST);
          _.glDisable(GL_LIGHTING);
          for (auto& _projector : projectorViz_)
          {
            _projector.draw();
            _projector.drawPositioning(_canvas ? _canvas->center() : QVector3D(0,0,0));
          }

          for (auto& _projector : projectors_)
            session_->drawFrustumIntersection(_projector,"#0088cc",ProjectorViewMode::BOTH);

          _.glEnable(GL_DEPTH_TEST);

          for (auto& _projector : projectorViz_)
            _projector.drawHalo();
        });
      }
    }
  }
}
