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

#ifndef OMNI_PROJ_TUNING_H_
#define OMNI_PROJ_TUNING_H_

#include <memory>
#include <QColor>
#include <omni/Id.h>
#include <omni/WarpGrid.h>
#include <omni/BlendMask.h>
#include <omni/proj/CalibrationMode.h>
#include <omni/proj/ColorCorrection.h>
#include <omni/proj/Projector.h>

namespace omni {
  class Session;

  namespace visual {
    /// Forward declaration for tuning visualizer
    class Tuning;
  }

  namespace proj {
    class Calibration;

    /**@brief A projector tuning holds adjustment and distorsion data for a
       single projector and screen
       @detail A tuning consists of a projector, warp grid, blend mask and color
          correction.
               It holds a color as well. It can render a projector calibration
     **/
    class Tuning : public QObject, public Serializer<Tuning> {
        Q_OBJECT
        OMNI_PROPERTY_RW_DEFAULT(bool,virtualScreen,setVirtualScreen,false)
        OMNI_PROPERTY_RW(QRect,screenGeometry,setScreenGeometry)
        OMNI_PROPERTY_RW(QRect,contentGeometry,setContentGeometry)
        Q_PROPERTY(int width READ width NOTIFY contentGeometryChanged STORED false)
        Q_PROPERTY(int height READ width NOTIFY contentGeometryChanged STORED false)

        OMNI_PROPERTY_OBJ(Projector,projector)
        OMNI_PROPERTY_OBJ(WarpGrid,warpGrid)
        OMNI_PROPERTY_OBJ(BlendMask,blendMask)
        OMNI_PROPERTY_OBJ(ColorCorrection,colorCorrection)

        OMNI_PROPERTY_RW(QColor,color,setColor)
        OMNI_PROPERTY_RW(bool,showOutput,setShowOutput)
        OMNI_PROPERTY_RW_DEFAULT(qreal,overlapOpacity,setOverlapOpacity,0.0)
      public:
        typedef omni::visual::Tuning visualizer_type;

        /// Constructor
        Tuning(QObject* = nullptr);
        ~Tuning();

        /// Return pointer to visualizer
        visualizer_type      * visualizer() const;

        /// Make visualizer if it is not instantiated yet
        visualizer_type      * makeVisualizer();

        /// Return if a screen is associated with mapping
        bool                   hasScreen() const;

        /// Render calibration
        void                   renderCalibration(Calibration& _calib) const;

        /// Render and return calibration
        Calibration renderCalibration() const;

        /// Returns width of screen
        int         width() const;

        /// Returns height of screen
        int         height() const;

      signals:
        void virtualScreenChanged();
        void screenGeometryChanged();
        void contentGeometryChanged();
        void colorChanged();
        void showOutputChanged();
        void overlapOpacityChanged();

      private:
        std::unique_ptr<visualizer_type> viz_;
    };
  }
}

#endif /* OMNI_MAPPING_H_ */
