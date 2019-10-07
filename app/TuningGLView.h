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

#ifndef OMNI_UI_TUNINGGLVIEW_H_
#define OMNI_UI_TUNINGGLVIEW_H_

#include <QOpenGLFramebufferObject>

#include <omni/ui/GLView.h>
#include <omni/ui/mixin/DataModel.h>
#include <omni/visual/Session.h>
#include <omni/visual/Tuning.h>
#include <omni/ui/mixin/TuningFromIndex.h>

namespace omni {
  namespace ui {
    /// A GLView for visualizing all modes of a projector view (tuning)
    class TuningGLView :
      public GLView,
      public mixin::SharedDataModel<TuningGLView,Session>,
      public mixin::TuningFromIndex<TuningGLView>{
        Q_OBJECT
        OMNI_UI_SHARED_DATAMODEL(TuningGLView,Session)
      public:
        TuningGLView(QWidget * = nullptr);
        ~TuningGLView() override;

        /// If disabled, this widget accepts user inputs
        bool                    viewOnly() const;

        /// If disabled, screen rect will be stretched over whole widget
        bool                    keepAspectRatio() const;

        /// Flag for toggling drawing. Only black background is shown when false
        bool                    isDrawingEnabled() const;

        /// Returns true if cursor is visible
        bool                    showCursor() const;

        /// Return relative border value
        float                   border() const;

      public slots:
        /// Set tuning index from session
        void setTuningIndex(int);

        /// If disabled, screen rect will be stretched over whole widget
        void setKeepAspectRatio(bool);

        /// If true, this widget does NOT accept user inputs
        void setViewOnly(bool);

        void setShowCursor(bool);

        /**@brief Set flag which tells if projector view is actually drawn
         * @detail Used for activate/deactivate fullscreen view
         **/
        void setDrawingEnabled(bool);

        /// Set relative border distance
        void setBorder(float);

      private slots:

      signals:
        void    dataModelChanged();

      protected:
        void paint() override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;
        void wheelEvent(QWheelEvent *event) override;
        void keyPressEvent(QKeyEvent *event) override;
        void showEvent(QShowEvent *event) override;

      private:
        virtual void dataToFrontend() override;
        inline virtual bool frontendToData() override {
          return false;
        }

        bool flipped() const;

        void drawOutput(
          float _blendMaskOpacity,
          float _inputOpacity = 1.0,
          QColor _color = Qt::white);


        /// Draw warp grid with handles while keeping aspect ratio
        void    drawWarpGrid();

        /// Draw blend mask with stroke buffer
        void    drawBlendMask();

        /// Draw test card image
        void    drawTestCard();

        /// Draw color corrected view (also used in export mode)
        void    drawColorCorrected();

        /// Update warp buffer which contains image of projector perspective
        void    updateWarpBuffer();

        /// Get rectangle of orthogonal view frustum
        QRectF  viewRect() const;

        /// Transform widget position to view position
        QPointF screenPos(QPointF const& _pos) const;

        // Transform widget position to position in tuning image
        QPointF pixelPos(QPointF const& _pos) const;

        /// Drawing function for drawing on orthogonal 2D surface within view
        // rect
        template<typename F>
        void drawOnSurface(F f);

        /// Initialize OpenGL objects
        bool initialize() override;

        /// Flags which determines if aspect ratio is used when drawing content
        bool keepAspectRatio_ = false;

        /// Flag determines if user input is not accepted
        bool viewOnly_ = false;

        /// Flag for toggling drawing. Only black background is shown when false
        bool drawingEnabled_ = true;

        /// Flag which tells if mouse button is down
        bool mouseDown_ = false;

        /// Last stroke position
        QPointF lastStrokePos_;

        /// Show cursor flag (cursor is also shown when widget is view only)
        bool showCursor_ = true;

        /// Left over distance value for blend blush
        float leftOverDistance_ = 0.0;

        /// Relative border
        float border_ = 0.0;
    };
  }
}

#endif /* OMNI_UI_TUNINGGLVIEW_H_ */
