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

#ifndef OMNI_PROJ_TUNING_H_
#define OMNI_PROJ_TUNING_H_

#include <memory>
#include <QColor>
#include <omni/Id.h>
#include <omni/WarpGrid.h>
#include <omni/BlendMask.h>
#include <omni/proj/ColorCorrection.h>
#include <omni/proj/Projector.h>
#include <omni/proj/Setup.h>
#include <omni/proj/Screen.h>

namespace omni
{
  namespace proj
  {
    /**@brief A projector tuning holds adjustment and distorsion data for a single projector and screen
       @detail A tuning consists of a projector, warp grid, blend mask and color correction.
               It holds a color as well.
     **/
    class Tuning
    {
    public:
      /// Constructor
      Tuning();

      /// Constructor with a color
      Tuning(QColor const&);

      /// Set screen for this tuning
      void setScreen(QScreen const* _screen, int _subScreenIndex);
      QScreen const* screen() const;
      int subScreenIndex() const;

      Projector& projector();
      Projector const& projector() const;

      /// Returns reference to warp grid
      WarpGrid& warpGrid();

      /// Returns reference to warp grid (const version)
      WarpGrid const& warpGrid() const;

      /// Returns reference to blend mask
      BlendMask& blendMask();

      /// Returns reference to blend mask (const version)
      BlendMask const& blendMask() const;

      /// Color correction for projector
      ColorCorrection& colorCorrection();

      /// Color correction for projector (const)
      ColorCorrection const& colorCorrection() const;

      /// Returns color for mapping
      QColor color() const;

      /// Sets color for mapping
      void setColor(QColor const&);

      /// Return if a screen is associated with mapping
      bool hasScreen() const;

      /// Returns true if screen in argument has a different resolution than given one
      bool resolutionToBeChanged(proj::Screen const* _screen) const;

      /// Returns width of screen
      int width() const;

      /// Returns height of screen
      int height() const;

      /// Return flag if output is disabled, projector output is black
      bool outputDisabled() const;

      /// Return flag if output is enabled
      bool outputEnabled() const;

      /// Disable output if _disabled is true, enable otherwise
      void setOutputDisabled(bool _disabled = true);

      /// Enable output if _enabled is true, disable otherwise
      void setOutputEnabled(bool _enabled = true);

      friend bool operator==(Tuning const&,Tuning const&);

    private:
      bool outputDisabled_ = false;
      QColor color_;
      Projector projector_;
      std::unique_ptr<ProjectorSetup> projectorSetup_;

      WarpGrid warpGrid_;
      BlendMask blendMask_;
      ColorCorrection colorCorrection_;
    };
  }
}

QDataStream& operator>>(QDataStream&, omni::proj::Tuning&);
QDataStream& operator<<(QDataStream&, omni::proj::Tuning const&);

#endif /* OMNI_MAPPING_H_ */
