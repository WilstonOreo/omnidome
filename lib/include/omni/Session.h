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

#ifndef OMNI_SESSION_H_
#define OMNI_SESSION_H_

#include <omni/proj/TuningList.h>
#include <omni/input/List.h>
#include <omni/canvas/Interface.h>
#include <omni/proj/ScreenSetup.h>
#include <omni/render/ExportSettings.h>
#include <omni/BlendSettings.h>
#include <omni/visual/Scene.h>

namespace omni {
  class TuningList;

  namespace visual {
    class Session;
  }

  /**@brief A session consists of a canvas, a mapping, a list of tunings and one
     or several inputs
   */
  class OMNI_EXPORT Session {
    public:
      typedef omni::visual::Session visualizer_type;

      /// General mode of the User Interface
      enum class Mode
      {
        SCREENSETUP,     // Mode for setup screens
        ARRANGE,         // Mode for setting up projectors and canvas
        WARP,            // Mode for adjusting warp grid
        BLEND,           // Mode for editing the blend mask
        COLORCORRECTION, // Mode for color correction for each projector
        EXPORT,          // Export mode
        LIVE,            // Live mode. Output is locked for faster display
        NUM_MODES
      };


      /// Default constructor
      Session();
      ~Session();

      /// Returns reference to projector tunings
      proj::TuningList      & tunings();

      /// Returns const reference to projector tunings
      proj::TuningList const& tunings() const;

      /// Makes new mapping with given type Id
      Mapping               * setMapping(Id const& _typeId);

      /// Returns pointer to mapping
      Mapping               * mapping();

      /// Returns pointer to mapping (const version)
      Mapping const         * mapping() const;

      /// Returns reference to list of inputs
      InputList           & inputs();

      /// Returns const reference to list of inputs
      InputList const     & inputs() const;

      /// Makes new canvas with given type id and returns pointer if successful
      Canvas              * setCanvas(Id const& _typeId);

      /// Return pointer to canvas
      Canvas              * canvas();

      /// Return pointer to canvas (const version)
      Canvas const        * canvas() const;

      /// Return reference to current screen setup
      ScreenSetup         & screenSetup();

      /// Return const reference to current screen setup
      ScreenSetup const   & screenSetup() const;

      /// Return reference to export settings
      Scene               & scene();

      /// Return reference to export settings
      Scene const         & scene() const;

      /// Return reference to blend settings
      BlendSettings       & blendSettings();

      /// Return reference to blend settings
      BlendSettings const & blendSettings() const;

      /// Return reference to export settings
      ExportSettings      & exportSettings();

      /// Return reference to export settings
      ExportSettings const& exportSettings() const;


      /// Return current mode
      Mode mode() const;

      /// Set mode of this session
      void setMode(Mode);

      /// A session has an output when there is an input, a canvas and a mapping
      bool hasOutput() const;

      /// Export calibration data of session to a file
      void renderToFile(QString const& _filename);

      /// Save session to file
      void        save(QString const& _filename) const;

      /// Load session to file
      void        load(QString const& _filename);

      /// Deserialize from stream
      void        fromStream(QDataStream&);

      /// Serialize to stream
      void        toStream(QDataStream&) const;

      /// Test for equality. ScreenSetup is ignored
      friend bool operator==(Session const&,
                             Session const&);

      /// Make visualizer if it is not instantiated yet
      visualizer_type      * makeVisualizer();

      visualizer_type* visualizer();
      visualizer_type const* visualizer() const;


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

      visual::Scene  scene_;
      BlendSettings  blendSettings_;
      ExportSettings exportSettings_;

      std::unique_ptr<visualizer_type> viz_;
  };
}

OMNI_DECL_STREAM_OPERATORS(omni::Session)

OMNI_DECL_ENUM_STREAM_OPERATORS(omni::Session::Mode)

#endif /* OMNI_SESSION_H_ */
