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

#ifndef OMNI_UI_GLVIEW_H_
#define OMNI_UI_GLVIEW_H_

#include <set>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QPointF>
#include <omni/ui/global.h>

namespace omni {

  namespace ui {
    /**@brief GLView for visualizing tunings or session
       @detail Holds shared_ptr to session as data model
     **/
    class OMNIWIDGET_EXPORT GLView :
      public QOpenGLWidget,
      protected QOpenGLFunctions {
        Q_OBJECT

      public:
        explicit GLView(QWidget *_parent = nullptr);
        virtual ~GLView();

        /// Return aspect ratio of widget
        float   aspect() const;

        /// Return mouse position
        QPointF mousePosition() const;

        /// Return boolean value if widget has been initialized
        bool    initialized() const;

        /// Return update frequency
        float   updateFrequency() const;

        /// Return update frequency
        void    setUpdateFrequency(float _updateFrequency);

        float   currentFps() const;

        void    drawFps(float _fps) const;

      public slots:
        void    triggerUpdate();

      protected:
        /// Initialize OpenGL contents
        void         initializeGL();

        /// Resize GL and viewport
        virtual void resizeGL(int _w,
                              int _h);

        /// Paint routine
        virtual void paint() = 0;

        /// Mouse press event sets mouse position
        virtual void mousePressEvent(QMouseEvent *);

        void timerEvent(QTimerEvent*);

        /// Mouse position stored
        QPointF mousePosition_;

      private:
        void paintGL();

        bool   updateTriggered_ = false;
        /// Pure virtual function to initialize GL contents
        virtual bool initialize() = 0;

        /// Is initialized flag is set to true after successful initialization
        bool initialized_ = false;

        float  updateFreq_     = 0.0;
        float  currentFps_ = 0.0;
        
        int timerId_ = 0;
    };
  }
}

#endif /* OMNI_UI_GLVIEW_H_ */
