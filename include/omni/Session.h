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

#include <omni/Serializer.h>
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
  class Session : public QObject, public Serializer<Session> {
      Q_OBJECT
      Q_PROPERTY(Mode mode READ mode WRITE setMode NOTIFY modeChanged)
      Q_PROPERTY(Mapping* READ mapping WRITE setMapping NOTIFY mappingChanged)
      Q_PROPERTY(Canvas* READ canvas WRITE setCanvas NOTIFY canvasChanged)
      Q_PROPERTY(bool hasOutput READ output NOTIFY hasOutputChanged)

      Q_PROPERTY(TuningList* READ tunings CONSTANT)
      Q_PROPERTY(InputList* READ inputs CONSTANT)
      Q_PROPERTY(ScreenSetup* READ screenSetup CONSTANT)
      Q_PROPERTY(BlendSettings* READ blendSettings CONSTANT)
      Q_PROPERTY(ExportSettings* READ exportSettings CONSTANT)
      Q_PROPERTY(Scene* READ scene CONSTANT)
    public:
      typedef omni::visual::Session visualizer_type;

      /// General mode of the User Interface
      enum Mode
      {
        MODE_SCREENSETUP,     // Mode for setup screens
        MODE_ARRANGE,         // Mode for setting up projectors and canvas
        MODE_WARP,            // Mode for adjusting warp grid
        MODE_BLEND,           // Mode for editing the blend mask
        MODE_COLORCORRECTION, // Mode for color correction for each projector
        MODE_EXPORT,          // Export mode
        MODE_LIVE,            // Live mode. Output is locked for faster display
        MODE_NUM_MODES
      };
      Q_ENUM(Mode)

      /// Default constructor
      Session();
      ~Session();

      /// Return current mode
      Mode mode() const;

      /// Set mode of this session
      void setMode(Mode);

      /// Returns reference to projector tunings
      proj::TuningList      * tunings() const;

      /// Returns pointer to mapping
      Mapping               * mapping() const;

      /// Set a new mapping. Session will be parent of mapping.
      void                  setMapping(Mapping*);

      /// Returns reference to list of inputs
      InputList           * inputs() const;

      /// Return pointer to canvas
      Canvas              * canvas() const;

      /// Makes new canvas with given type id and returns pointer if successful
      void                  setCanvas(Canvas*);

      /// Return reference to current screen setup
      ScreenSetup         * screenSetup() const;

      /// Return reference to scene settings
      Scene               * scene() const;

      /// Return reference to blend settings
      BlendSettings       * blendSettings() const;

      /// Return reference to export settings
      ExportSettings      * exportSettings() const;


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

      visualizer_type      * visualizer() const;
    signals:
      void modeChanged();
      void mappingChanged();
      void canvasChanged();
      void hasOutputChanged();

    private:
      /// List with all projector tunings
      proj::TuningList* tunings_;

      /// Owning pointer for mapping
      Mapping* mapping_;

      /// List with all inputs (e.g. images, videos etc)
      InputList* inputs_;

      /// Owning pointer for canvas
      Canvas* canvas_;

      /// Current screen setup
      ScreenSetup* screenSetup_;

      /// Current session mode
      Mode mode_ = MODE_SCREENSETUP;

      visual::Scene*  scene_;
      BlendSettings*  blendSettings_;
      ExportSettings* exportSettings_;

      std::unique_ptr<visualizer_type> viz_;
  };
}

#endif /* OMNI_SESSION_H_ */
