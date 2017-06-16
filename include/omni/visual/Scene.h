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
#include <omni/Serializer.h>
#include <omni/visual/Light.h>
#include <omni/visual/CameraInterface.h>
#include <omni/LengthUnit.h>
#include <omni/visual/Grid.h>

namespace omni {
  namespace visual {

    /**@brief A class that holds settings and elements for session 3d
       visualization
       @detail Belongs to a session
     **/
    class Scene : public QObject {
        Q_OBJECT
    public:
        /// Enum class to determine which scene element is edited
        enum EditMode {
          EDIT_CAMERA,
          EDIT_MOVE,
          EDIT_ROTATE
        };
        Q_ENUMS(EditMode)

        /// Which direction to move with mouse
        enum MoveMode {
          MOVE_X,
          MOVE_Y,
          MOVE_Z,
          MOVE_XY
        };
        Q_ENUMS(MoveMode)

        /// Rotate pitch, yaw, roll
        enum RotateMode {
          ROTATE_PITCH,
          ROTATE_YAW,
          ROTATE_ROLL
        };
        Q_ENUMS(RotateMode)

        enum ProjectorDisplayMode {
          PROJ_ALL, PROJ_SELECTED, PROJ_NONE
        };
        Q_ENUMS(ProjectorSelectionMode)
      private:
        OMNI_PROPERTY_RW_DEFAULT(qreal,size,setSize,10.0)
        OMNI_PROPERTY_RW(LengthUnit,unit,setUnit)
        OMNI_PROPERTY_RW_DEFAULT(bool,displayInput,setDisplayInput,true)
        OMNI_PROPERTY_RW_DEFAULT(bool,displayGrid,setDisplayGrid,true)
        OMNI_PROPERTY_RW_DEFAULT(Scene::ProjectorDisplayMode,displayProjectors,setDisplayProjectors,PROJ_ALL)
        OMNI_PROPERTY_RW_DEFAULT(Scene::ProjectorDisplayMode,displayProjectedAreas,setDisplayProjectedAreas,PROJ_ALL)

      public:
        typedef std::map<QString,std::unique_ptr<CameraInterface>> camera_map_type;

        Scene();

        /// Set unit from string prefix
        void           setUnit(QString const&);

        /// Return scene element
        EditMode       editMode() const;

        /// Set mode which scene element is to be manipulated
        void setEditMode(EditMode);

        RotateMode        rotateMode() const;
        void setRotateMode(RotateMode);

        MoveMode          moveMode() const;
        void setMoveMode(MoveMode);

        /// Inside / outside transparency for 3D canvas view
        qreal insideOutside() const;

        /* @brief Set inside / outside transparency for 3D canvas view
           @param _insideOutside Value 0.0 means inside is only visible
                  1.0 means outside is visible.
         */
        void setInsideOutside(qreal _insideOutside);

        /// Opacity value for wireframe between 0.0 and 1.0
        qreal wireframe() const;

        /* @brief Set wireframe opacity for canvas view
           @param _wireframe Value 0.0 means wireframes are not drawn.
                  1.0 means wireframe is fully visible and thick.
         */
        void setWireframe(qreal _wireframe);

        /// Update light for use in OpenGL
        void        updateLights();

        /// Return reference to camera
        visual::CameraInterface* camera() const;

        /// Return current camera id
        QString currentCameraId() const;

        /// Set id to current camera
        void setCurrentCameraId(QString const&);

        camera_map_type& cameras();
        camera_map_type const& cameras() const;

        /// Draw grid with given scene size if it is to be displayed
        void drawGrid() const;

        /// Update grid
        void updateGrid();
      signals:


      private:
        qreal      insideOutside_            = 0.5;
        qreal      wireframe_                = 0.0;
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

#endif /* OMNI_VISUAL_SCENE_H_ */
