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

#ifndef OMNI_UI_GLVIEW3D_H_
#define OMNI_UI_GLVIEW3D_H_

#include <omni/ui/GLView.h>
#include <omni/visual/Camera.h>
#include <omni/visual/Light.h>
#include <omni/visual/Grid.h>

namespace omni
{
  namespace ui
  {
    enum class EditMode {
        CAMERA,
        MOVE,
        ROTATE,
    };

    enum class MoveMode {
        MOVE_X,
        MOVE_Y,
        MOVE_Z,
        MOVE_XY
    };

    enum class RotateMode {
        PITCH,
        YAW,
        ROLL
    };

    /**@brief An OpenGL view with a camera and perspective view
     * @detail Also holds three lights
     **/
    class GLView3D : public GLView
    {
      Q_OBJECT
    public:
      GLView3D(QWidget* _parent = nullptr);
      ~GLView3D();

      /// Input is shown on canvas
      bool displayInput() const;

      /// Display measurements on elements
      bool displayMeasures() const;

      /// Display line grid
      bool displayGrid() const;

      /// Display projector frustra (selected projector is always shown)
      bool displayProjectors() const;

      /// Display projected areas
      bool displayProjectedAreas() const;

      /// Set edit mode (CAMERA, ROTATE or MOVE)
      EditMode editMode() const;

      /// Set rotation axis when edit mode is ROTATE
      RotateMode rotateMode() const;

      /// Set movement axis when edit mode is MOVE
      MoveMode moveMode() const;

      /// Set projector view mode (INSIDE, OUTSIDE or BOTH)
      ProjectorViewMode projectorViewMode() const;
      void setProjectorViewMode(ProjectorViewMode);

    public slots:
      void setDisplayInput(bool);
      void setDisplayMeasures(bool);
      void setDisplayGrid(bool);
      void setDisplayProjectors(bool);
      void setDisplayProjectedAreas(bool);
      void setEditMode(EditMode);
      void setRotateMode(RotateMode);
      void setMoveMode(MoveMode);

      void setProjectorViewMode(int);
      void changeZoom(int _value);

    protected:
      virtual void paintGL();

      virtual void wheelEvent(QWheelEvent* event);
      virtual void keyPressEvent(QKeyEvent* event);
      virtual void mouseMoveEvent(QMouseEvent *event);

      void setupCamera();
      void updateLight();

    private:
      bool initialize();

      visual::Camera camera_;
      visual::Grid grid_;
      std::array<visual::Light,3> lights_;
      bool displayInput_ = true;
      bool displayMeasures_ = true;
      bool displayGrid_ = true;
      bool displayProjectors_ = true;
      bool displayProjectedAreas_ = true;

      EditMode editMode_ = EditMode::CAMERA;
      RotateMode rotateMode_ = RotateMode::YAW;
      MoveMode moveMode_ = MoveMode::MOVE_XY;

      ProjectorViewMode projectorViewMode_ = ProjectorViewMode::INSIDE;
    };
  }
}

#endif /* OMNI_UI_GLVIEW3D_H_ */
