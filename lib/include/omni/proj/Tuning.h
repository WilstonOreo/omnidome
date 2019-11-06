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
#include <omni/Factory.h>
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
    class OMNI_EXPORT Tuning {
      public:
        typedef omni::visual::Tuning visualizer_type;

        /// Constructor
        Tuning(Session const&);

        /// Assign virtual screen to tuning
        void assignVirtualScreen();

        /// Set screen for this tuning
        void                   setScreen(QScreen const *_screen,
                                         int _subScreenIndex);

        QScreen const        * screen() const;
        int                    subScreenIndex() const;

        Projector            & projector();
        Projector const      & projector() const;

        /// Returns reference to warp grid
        WarpGrid             & warpGrid();

        /// Returns reference to warp grid (const version)
        WarpGrid const       & warpGrid() const;

        /// Returns reference to blend mask
        BlendMask            & blendMask();

        /// Returns reference to blend mask (const version)
        BlendMask const      & blendMask() const;

        /// Color correction for projector
        ColorCorrection      & colorCorrection();

        /// Color correction for projector (const)
        ColorCorrection const& colorCorrection() const;

        /// Returns color for mapping
        QColor                 color() const;

        /// Sets color for mapping
        void                   setColor(QColor const&);

        /// Return pointer to visualizer
        visualizer_type      * visualizer();

        /// Return pointer to visualizer (const version)
        visualizer_type const* visualizer() const;

        /// Make visualizer if it is not instantiated yet
        visualizer_type      * makeVisualizer();

        /// Return if a screen is associated with mapping
        bool                   hasScreen() const;

        /// Render calibration
        void                   renderCalibration(Calibration& _calib)
        const;

        /// Rectangle of the output screen on desktop
        QRect                  screenGeometry() const;

        /// Content rectangle (position) inside the screen
        QRect                  contentGeometry() const;

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

        /// Write tuning to stream
        void        toStream(QDataStream&) const;

        /// Read tuning from stream
        void        fromStream(QDataStream&);

        /// Test for equality
        friend bool operator==(Tuning const&,
                               Tuning const&);

        /// Return const reference to owning session
        Session const& session() const;

        /// Return id of tuning
        int id() const;

      private:
        bool   outputDisabled_ = false;
        bool   overlapOpacity_ = 0.0;
        QColor color_;
        Projector projector_;

        /**@brief Screen this tuning is assigned to.
           @detail If screen is nullptr, tuning is assigned to virtual screen
         **/
        QScreen const* screen_ = nullptr;

        /// SubScreen index of this tuning
        int subScreenIndex_ = 0;

        WarpGrid  warpGrid_;
        BlendMask blendMask_;
        ColorCorrection colorCorrection_;
        std::unique_ptr<visualizer_type> viz_;
        Session const& session_;
    };
  }
}

OMNI_DECL_STREAM_OPERATORS(omni::proj::Tuning)

#endif /* OMNI_MAPPING_H_ */
