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
#include <omni/property.h>
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
    class Scene : public QObject, public Serializer<Scene> {
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

        enum ProjectorSelectionMode {
          PROJ_ALL, PROJ_SELECTED, PROJ_NONE
        };
        Q_ENUMS(ProjectorSelectionMode)
      private:
        OMNI_PROPERTY_RW_DEFAULT(qreal,size,setSize,10.0)
        OMNI_PROPERTY_RW(LengthUnit,unit,setUnit)
        OMNI_PROPERTY_RW_DEFAULT(bool,displayInput,setDisplayInput,true)
        OMNI_PROPERTY_RW_DEFAULT(bool,displayGrid,setDisplayGrid,true)
        OMNI_PROPERTY_RW_DEFAULT(ProjectorSelectionMode,displayProjectors,setDisplayProjectors,PROJ_ALL)
        OMNI_PROPERTY_RW_DEFAULT(ProjectorSelectionMode,displayProjectedAreas,setDisplayProjectedAreas,PROJ_ALL)

        OMNI_PROPERTY_CLAMPED(qreal,insideOutside,setInsideOutside,0.5,0.0,1.0)
        OMNI_PROPERTY_CLAMPED(qreal,wireframe,setWireframe,0.0,0.0,1.0)
        OMNI_PROPERTY_RW_DEFAULT(EditMode,editMode,setEditMode,EDIT_CAMERA)
        OMNI_PROPERTY_RW_DEFAULT(RotateMode,rotateMode,setRotateMode,ROTATE_YAW)
        OMNI_PROPERTY_RW_DEFAULT(MoveMode,moveMode,setMoveMode,MOVE_XY)

        OMNI_PROPERTY_RW(QString,currentCameraId,setCurrentCameraId)
      public:
        typedef std::map<QString,std::unique_ptr<CameraInterface>> camera_map_type;

        Scene();

        /// Update light for use in OpenGL
        void        updateLights();

        /// Return reference to camera
        visual::CameraInterface* camera() const;

        camera_map_type& cameras();
        camera_map_type const& cameras() const;

        /// Draw grid with given scene size if it is to be displayed
        void drawGrid() const;

        /// Update grid
        void updateGrid();

    signals:
        void sizeChanged();
        void unitChanged();
        void displayInputChanged();
        void displayGridChanged();
        void displayProjectorsChanged();
        void displayProjectedAreasChanged();
        void insideOutsideChanged();
        void wireframeChanged();
        void editModeChanged();
        void rotateModeChanged();
        void moveModeChanged();
        void currentCameraIdChanged();

      private:
        std::vector<visual::Light> lights_;
        std::map<QString,std::unique_ptr<visual::CameraInterface>> cameras_;
        std::unique_ptr<visual::Grid>   grid_;
    };
  }
  using visual::Scene;
}

#endif /* OMNI_VISUAL_SCENE_H_ */
