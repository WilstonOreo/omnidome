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
#include <omni/visual/Grid.h>

namespace omni {
  namespace ui {
    /**@brief An OpenGL view for visualizing the scene of a session
     **/
    class GLView3D : public GLView {
      Q_OBJECT

      public:
        GLView3D(QWidget *_parent = nullptr);
        ~GLView3D();

      public slots:
        void         changeZoom(int _value);

      protected:
        virtual void paintGL();

        /// Change zoom on mouse wheel event
        virtual void wheelEvent(QWheelEvent *event);
        virtual void keyPressEvent(QKeyEvent *event);
        virtual void mouseMoveEvent(QMouseEvent *event);
        virtual void showEvent(QShowEvent* event);

      private:
        bool         initialize();

        std::unique_ptr<visual::Grid>   grid_;
    };
  }
}

#endif /* OMNI_UI_GLVIEW3D_H_ */
