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

#ifndef OMNI_SESSION_H_
#define OMNI_SESSION_H_

#include <omni/proj/TuningList.h>
#include <omni/input/List.h>
#include <omni/canvas/Interface.h>
#include <omni/proj/ScreenSetup.h>
#include <omni/RenderOptions.h>

namespace omni
{
  class TuningList;

  /**@brief A session consists of a canvas, a mapping, a list of tunings and one or several inputs
   */
  class Session
  {
  public:
    /// General mode of the User Interface
    enum class Mode
    {
      SCREENSETUP, // Mode for setup screens
      ARRANGE, // Mode for setting up projectors and canvas
      WARP, // Mode for adjusting warp grid
      BLEND, // Mode for editing the blend mask
      COLORCORRECTION, // Mode for color correction for each projector
      EXPORT, // Export mode. Output is locked for faster display
      LIVE, // Live mode. Output is locked for faster display
      NUM_MODES
    };

    /// Blend Mask display mode
    enum class BlendMode
    {
      COLOR, // Displays blend mask with color of tuning
      WHITE, // Displays white blend mask
      INPUT // Displays blend mask with current input
    };

    /// Default constructor
    Session();
    ~Session();

    /// Returns reference to projector tunings
    proj::TuningList& tunings();

    /// Returns const reference to projector tunings
    proj::TuningList const& tunings() const;

    /// Makes new mapping with given type Id
    Mapping* setMapping(Id const& _typeId);

    /// Returns pointer to mapping
    Mapping* mapping();

    /// Returns pointer to mapping (const version)
    Mapping const* mapping() const;

    /// Returns reference to list of inputs
    InputList& inputs();

    /// Returns const reference to list of inputs
    InputList const& inputs() const;

    /// Makes new canvas with given type id and returns pointer if successful
    Canvas* setCanvas(Id const& _typeId);

    /// Return pointer to canvas
    Canvas* canvas();

    /// Return pointer to canvas (const version)
    Canvas const* canvas() const;

    /// Return reference to current screen setup
    ScreenSetup& screenSetup();

    /// Return const reference to current screen setup
    ScreenSetup const& screenSetup() const;

    /// Return current mode
    Mode mode() const;

    /// Set mode of this session
    void setMode(Mode);

    /// Return mode of blend mask mode
    BlendMode blendMode() const;

    /// Set blend mask mode
    void setBlendMode(BlendMode);

    /// A session has an output when there is an input, a canvas and a mapping
    bool hasOutput() const;

    /// Export calibration data of session to a file
    void renderToFile(QString const& _filename, RenderOptions const& _opt);

    /// Save session to file
    void save(QString const& _filename) const;

    /// Load session to file
    void load(QString const& _filename);

    /// Test for equality. ScreenSetup is ignored
    friend bool operator==(Session const&,Session const&);

  private:
    /// List with all projector tunings
    proj::TuningList tunings_;

    /// Owning pointer for mapping
    std::unique_ptr<Mapping> mapping_;

    /// List with all inputs (e.g. images, videos etc)
    InputList inputs_;

    /// Owning pointer for canvas
    std::unique_ptr<Canvas> canvas_;

    /// Current screen setup
    ScreenSetup screenSetup_;

    /// Current session mode
    Mode mode_ = Mode::SCREENSETUP;

    BlendMode blendMode_ = BlendMode::COLOR;
  };
}

/// Serialize session to stream
QDataStream& operator<<(QDataStream&, omni::Session const&);

/// Deserialize session from stream
QDataStream& operator>>(QDataStream&, omni::Session&);

#endif /* OMNI_SESSION_H_ */
