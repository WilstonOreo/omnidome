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
#include <omni/mapping/Interface.h>

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
    class Tuning : public QObject {
        Q_OBJECT
        Q_PROPERTY(bool virtualScreen READ virtualScreen WRITE setVirtualScreen NOTIFY virtualScreenChanged)
        Q_PROPERTY(QRect screenGeometry READ screenGeometry WRITE screenGeometry NOTIFY screenGeometryChanged)
        Q_PROPERTY(QRect contentGeometry READ contentGeometry WRITE contentGeometry NOTIFY contentGeometryChanged)
        Q_PROPERTY(int width READ width NOTIFY contentGeometryChanged STORED false)
        Q_PROPERTY(int height READ width NOTIFY contentGeometryChanged STORED false)
        Q_PROPERTY(Projector* projector READ projector CONSTANT)
        Q_PROPERTY(WarpGrid* warpGrid READ warpGrid CONSTANT)
        Q_PROPERTY(BlendMask* blendMask READ blendMask CONSTANT)
        Q_PROPERTY(ColorCorrection* colorCorrection READ colorCorrection CONSTANT)

        Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
        Q_PROPERTY(bool output READ output WRITE setOutput NOTIFY outputChanged)
        Q_PROPERTY(qreal overlapOpacity READ setOverlapOpacity NOTIFY overlapOpacityChanged)
      public:
        typedef omni::visual::Tuning visualizer_type;

        /// Constructor
        Tuning(QObject* = nullptr);
        ~Tuning();

        bool virtualScreen() const;
        void setVirtualScreen(bool);

        QRect const& screenGeometry() const;
        void setScreenGeometry(QRect const&);

        QRect const& contentGeometry() const;
        void setContentGeometry(QRect const&);

        Projector            * projector() const;

        /// Returns reference to warp grid
        WarpGrid             * warpGrid() const;

        /// Returns reference to blend mask
        BlendMask            * blendMask() const;

        /// Color correction for projector
        ColorCorrection      * colorCorrection() const;

        /// Returns color for tuning
        QColor                 color() const;

        /// Sets color for tuning
        void                   setColor(QColor const&);

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

        /// Return flag if output is disabled, projector output is black
        bool        outputDisabled() const;

        /// Return flag if output is enabled
        bool        outputEnabled() const;

        /// Disable output if _disabled is true, enable otherwise
        void        setOutputDisabled(bool _disabled = true);

        /// Enable output if _enabled is true, disable otherwise
        void        setOutputEnabled(bool _enabled = true);

        /// Return opacity of overlapped blend mask
        float       overlapOpacity() const;

        /// Set opacity of overlap mask in blend mode
        void        setOverlapOpacity(float);

      signals:
        void virtualScreenChanged();
        void outputChanged();
        void overlapOpacityChanged();
        void colorChanged();

      private:
        bool virtualScreen_ = false;
        QRect screenGeometry_;
        QRect contentGeometry_;
        bool   output_ = true;
        bool   overlapOpacity_ = 0.0;
        QColor color_;
        Projector* projector_;
        WarpGrid* warpGrid_;
        BlendMask* blendMask_;
        ColorCorrection* colorCorrection_;
        std::unique_ptr<visualizer_type> viz_;
    };
  }
}

#endif /* OMNI_MAPPING_H_ */
