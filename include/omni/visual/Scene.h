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

#ifndef OMNI_VISUAL_SCENE_H_
#define OMNI_VISUAL_SCENE_H_

#include <map>
#include <memory>
#include <omni/visual/ProjectorViewMode.h>
#include <omni/serialization/Interface.h>
#include <omni/visual/Light.h>
#include <omni/visual/CameraInterface.h>
#include <omni/LengthUnit.h>

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

    /**@brief A class that holds settings and elements for session 3d
       visualization
       @detail Belongs to a session
     **/
    class Scene {
      public:
        typedef std::map<QString,std::unique_ptr<CameraInterface>> camera_map_type;

        Scene();

        /// Scene scale (min, max dimensions)
        float          scale() const;

        /// Set scene scale
        void           setScale(float);

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
        bool           displayProjectors() const;

        /// Turn displaying of projectors on/off
        void           setDisplayProjectors(bool);

        /// Display projected areas
        bool           displayProjectedAreas() const;

        /// Turn displaying of projector areas on/off
        void           setDisplayProjectedAreas(bool);

        /// Return scene element
        EditMode       editMode() const;

        /// Set mode which scene element is to be manipulated
        void setEditMode(EditMode);

        RotateMode        rotateMode() const;
        void setRotateMode(RotateMode);

        MoveMode          moveMode() const;
        void setMoveMode(MoveMode);

        ProjectorViewMode projectorViewMode() const;
        void setProjectorViewMode(ProjectorViewMode);

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

      private:
        float      scale_                    = 10.0;
        LengthUnit unit_;
        bool       displayInput_             = true;
        bool       displayGrid_              = true;
        bool       displayProjectors_        = true;
        bool       displayProjectedAreas_    = true;
        EditMode   editMode_                 = EditMode::CAMERA;
        RotateMode rotateMode_               = RotateMode::YAW;
        MoveMode   moveMode_                 = MoveMode::MOVE_XY;
        ProjectorViewMode projectorViewMode_ = ProjectorViewMode::INSIDE;

        QString cameraId_;
        std::vector<visual::Light> lights_;
        std::map<QString,std::unique_ptr<visual::CameraInterface>> cameras_;
    };
  }
  using visual::Scene;
}

OMNI_DECL_ENUM_STREAM_OPERATORS(omni::visual::EditMode)
OMNI_DECL_ENUM_STREAM_OPERATORS(omni::visual::MoveMode)
OMNI_DECL_ENUM_STREAM_OPERATORS(omni::visual::RotateMode)

OMNI_DECL_STREAM_OPERATORS(omni::visual::Scene)

#endif /* OMNI_VISUAL_SCENE_H_ */
