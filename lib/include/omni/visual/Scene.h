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

#ifndef OMNI_VISUAL_SCENE_H_
#define OMNI_VISUAL_SCENE_H_

#include <map>
#include <memory>
#include <omni/serialization/Interface.h>
#include <omni/visual/Light.h>
#include <omni/visual/CameraInterface.h>
#include <omni/LengthUnit.h>
#include <omni/visual/Grid.h>

namespace omni {
  namespace visual {
    /// Enum class to determine which scene element is edited
    enum class EditMode {
      CAMERA,
      MOVE,
      ROTATE
    };

    /// Which direction to move with mouse
    enum class MoveMode {
      MOVE_X,
      MOVE_Y,
      MOVE_Z,
      MOVE_XY
    };

    /// Rotate pitch, yaw, roll
    enum class RotateMode {
      PITCH,
      YAW,
      ROLL
    };

    enum class ProjectorSelectionMode {
      ALL, SELECTED, NONE
    };

    /**@brief A class that holds settings and elements for session 3d
       visualization
       @detail Belongs to a session
     **/
    class OMNI_EXPORT Scene {
      public:
        typedef std::map<QString,std::unique_ptr<CameraInterface>> camera_map_type;

        Scene();

        /// Scene size (min, max dimensions)
        float          size() const;

        /// Set scene size
        void           setSize(float);

        /// Length unit for scene
        LengthUnit const& unit() const;

        /// Set unit from string prefix
        void           setUnit(QString const&);

        /// Set unit from length unit type
        void           setUnit(LengthUnit const&);

        /// Input is shown on canvas
        bool           displayInput() const;

        /// Turn displaying of input on canvas on/off
        void           setDisplayInput(bool);

        /// Display line grid
        bool           displayGrid() const;

        /// Turn displaying of grid on/off
        void           setDisplayGrid(bool);

        /// Display projector frustra (selected projector is always shown)
        ProjectorSelectionMode displayProjectors() const;

        /// Turn displaying of projectors on/off
        void           setDisplayProjectors(ProjectorSelectionMode);

        /// Display projected areas
        ProjectorSelectionMode           displayProjectedAreas() const;

        /// Turn displaying of projector areas on/off
        void           setDisplayProjectedAreas(ProjectorSelectionMode);

        /// Return scene element
        EditMode       editMode() const;

        /// Set mode which scene element is to be manipulated
        void setEditMode(EditMode);

        RotateMode        rotateMode() const;
        void setRotateMode(RotateMode);

        MoveMode          moveMode() const;
        void setMoveMode(MoveMode);

        /// Inside / outside transparency for 3D canvas view
        float insideOutside() const;

        /* @brief Set inside / outside transparency for 3D canvas view
           @param _insideOutside Value 0.0 means inside is only visible
                  1.0 means outside is visible.
         */
        void setInsideOutside(float _insideOutside);

        /// Opacity value for wireframe between 0.0 and 1.0
        float wireframe() const;

        /* @brief Set wireframe opacity for canvas view
           @param _wireframe Value 0.0 means wireframes are not drawn.
                  1.0 means wireframe is fully visible and thick.
         */
        void setWireframe(float _wireframe);

        /// Update light for use in OpenGL
        void        updateLights();

        /// Return reference to camera
        visual::CameraInterface* camera();

        /// Return const reference to camera
        visual::CameraInterface const* camera() const;

        /// Return current camera id
        QString currentCameraId() const;

        /// Set id to current camera
        void setCurrentCameraId(QString const&);

        camera_map_type& cameras();
        camera_map_type const& cameras() const;

        /// Deserialize from stream
        void        fromStream(QDataStream&);

        /// Serialize to stream
        void        toStream(QDataStream&) const;

        /// Test for equality. ScreenSetup is ignored
        friend bool operator==(Scene const&,
                               Scene const&);

        /// Draw grid with given scene size if it is to be displayed
        void drawGrid() const;

        /// Update grid
        void updateGrid();


      private:
        float      insideOutside_            = 0.5;
        float      size_                     = 10.0;
        float      wireframe_                = 0.0;
        LengthUnit unit_;
        bool       displayInput_             = true;
        bool       displayGrid_              = true;
        ProjectorSelectionMode       displayProjectors_        = ProjectorSelectionMode::ALL;
        ProjectorSelectionMode       displayProjectedAreas_    = ProjectorSelectionMode::ALL;
        EditMode   editMode_                 = EditMode::CAMERA;
        RotateMode rotateMode_               = RotateMode::YAW;
        MoveMode   moveMode_                 = MoveMode::MOVE_XY;

        QString cameraId_;
        std::vector<visual::Light> lights_;
        std::map<QString,std::unique_ptr<visual::CameraInterface>> cameras_;
        std::unique_ptr<visual::Grid>   grid_;
    };
  }
  using visual::Scene;
}

OMNI_DECL_ENUM_STREAM_OPERATORS(omni::visual::EditMode)
OMNI_DECL_ENUM_STREAM_OPERATORS(omni::visual::MoveMode)
OMNI_DECL_ENUM_STREAM_OPERATORS(omni::visual::RotateMode)
OMNI_DECL_ENUM_STREAM_OPERATORS(omni::visual::ProjectorSelectionMode)

OMNI_DECL_STREAM_OPERATORS(omni::visual::Scene)

#endif /* OMNI_VISUAL_SCENE_H_ */
