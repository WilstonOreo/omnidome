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

#ifndef OMNI_UI_SCENEGLVIEW_H_
#define OMNI_UI_SCENEGLVIEW_H_

#include <omni/ui/GLView.h>
#include <omni/visual/Grid.h>
#include <omni/ui/mixin/DataModel.h>
#include <omni/visual/Session.h>

namespace omni {
  namespace ui {
    /**@brief An OpenGL view for visualizing the scene of a session
     **/
    class SceneGLView :
      public GLView,
      public mixin::SharedDataModel<SceneGLView,Session>
     {
      Q_OBJECT
      OMNI_UI_SHARED_DATAMODEL(SceneGLView,Session)

      public:
        SceneGLView(QWidget *_parent = nullptr);
        ~SceneGLView() override;

      public slots:
        void         changeZoom(int _value);
      signals:
        void dataModelChanged();

      protected:
        void paint() override;

        /// Change zoom on mouse wheel event
        virtual void wheelEvent(QWheelEvent *event) override;
        virtual void keyPressEvent(QKeyEvent *event) override;
        virtual void mouseMoveEvent(QMouseEvent *event) override;
        virtual void showEvent(QShowEvent* event) override;

      private:
        virtual void dataToFrontend() override;
        inline virtual bool frontendToData() override {
          return false;
        }

        bool         initialize() override;
    };
  }
}

#endif /* OMNI_UI_SCENEGLVIEW_H_ */
