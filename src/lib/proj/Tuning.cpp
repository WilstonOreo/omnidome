/* Copyright (c) 2014-2016 "Omnidome" by Michael Winkelmann
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

#include <omni/proj/Tuning.h>

#include <QOpenGLContext>
#include <omni/Session.h>
#include <omni/util.h>
#include <omni/serialization/PropertyMap.h>
#include <omni/proj/ScreenSetup.h>
#include <omni/proj/Calibration.h>

#include <omni/visual/Tuning.h>

#include <QColor>


namespace omni {
    namespace proj {
        Tuning::Tuning() :
            color_("#FFFFFF"),
            projector_(new Projector(this)),
            warpGrid_(new WarpGrid(this)),
            blendMask_(new BlendMask(this)),
            colorCorrection_(new ColorCorrection(this))
        {
        }

        bool Tuning::virtualScreen() const {
          return virtualScreen_;
        }

        void Tuning::setVirtualScreen(bool) {

        }

        /// Content rectangle (position) inside the screen
        QRect Tuning::contentGeometry() const {
          return contentGeometry_;
        }

        void Tuning::setContentGeometry(QRect const& _contentGeometry) {
          if (contentGeometry_ != _contentGeometry) {
            contentGeometry_ = _contentGeometry;
            //projector_.setAspectRatio(session_.screenSetup().subScreenAspectRatio(screen()));
            //blendMask_.setBrush(100.0,blendMask_.brush().feather(),blendMask_.brush().opacity(),blendMask_.brush().invert());
            contentGeometryChanged();
          }
        }

        int Tuning::width() const
        {
          return contentGeometry().width();
        }

        int Tuning::height() const
        {
          return contentGeometry().height();
        }

        Projector* Tuning::projector() const
        {
            return projector_;
        }

        WarpGrid* Tuning::warpGrid() const
        {
            return warpGrid_;
        }

        BlendMask* Tuning::blendMask() const
        {
            return blendMask_;
        }

        ColorCorrection* Tuning::colorCorrection() const {
            return colorCorrection_;
        }

        /// Return pointer to visualizer
        Tuning::visualizer_type* Tuning::visualizer() {
          return viz_.get();
        }

        Tuning::visualizer_type* Tuning::makeVisualizer() {
          if (!QOpenGLContext::currentContext()) return nullptr;

          if (!viz_) {
            viz_.reset(new visualizer_type(*this));
          }
          if (!viz_->initialized()) {
            viz_->update();
          }
          return visualizer();
        }

        /// Render calibration
        void Tuning::renderCalibration(Calibration& _calib)
        const {
          _calib.render(*this);
        }

        /// Render and return calibration
        Calibration Tuning::renderCalibration() const {
            return Calibration(*this);
        }

        OMNI_DEFINE_PROPERTY_RW(Tuning,bool,output,setOutput)
        OMNI_DEFINE_PROPERTY_RW(Tuning,bool,virtualScreen,setVirtualScreen)
        OMNI_DEFINE_PROPERTY_RW(Tuning,qreal,overlapOpacity,setOverlapOpacity)
        OMNI_DEFINE_PROPERTY_RW(Tuning,QColor,color,setColor)
        OMNI_DEFINE_PROPERTY_RW(Tuning,QRect,contentGeometry,setContentGeometry)
        OMNI_DEFINE_PROPERTY_RW(Tuning,QRect,screenGeometry,setScreenGeometry)
    }
}
