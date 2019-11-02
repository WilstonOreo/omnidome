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

#ifndef OMNI_VISUAL_TUNING_H_
#define OMNI_VISUAL_TUNING_H_

#include <QRectF>
#include <QOpenGLShaderProgram>
#include <QOpenGLFramebufferObject>
#include <QOpenGLTexture>

#include <omnic/gl/Framebuffer.h>
#include <omni/proj/Tuning.h>
#include <omni/visual/WarpGrid.h>
#include <omni/proj/Calibration.h>

namespace omni
{
  namespace visual
  {
    class Session;

    /// Tuning visualizer
    class OMNI_EXPORT Tuning
    {
    public:
      Tuning(omni::proj::Tuning&);
      ~Tuning();

      omni::proj::Tuning const& tuning() const;

      /// Update shaders, warp grid and blend texture
      void update();

      /// Updates blend texture after it has changed by blend brush
      void updateBlendTexture();

      /// Update warp grid mesh
      void updateWarpGrid();

      /// Set portion of blend mask which is to be updated
      void setBlendTextureUpdateRect(QRect const&);

      /// Draw Test card image for screen setup
      void drawTestCard() const;

      /**@brief Draws warp grid with lines and handles
         @param _rect Screen Rectangle of input
       **/
      void drawWarpGrid() const;

      /// Draw warp grip without lines and handles
      void drawWarpPatch() const;

      /// Draws Blendmask with warp grid and brush stroke layer
      void drawOutput(
          float _inputOpacity = 0.0,
          QColor const& _color = Qt::white,
          float _blendMaskOpacity = 1.0,
          bool _grayscale = true) const;

      void draw(QRectF const& _rect) const;

      /// Draw cursor for blend brush
      void drawCursor() const;

      bool initialized() const;

      /// Update warp buffer which contains image of projector perspective
      void updateWarpBuffer();

      void drawFullScreenOutput() const;

      void generateCalibrationData();

      void drawCalibratedInput(bool _flipped = false) const;
 

        /// Draw screen border (only if widget is not in view only mode)
      void drawScreenBorder() const;

      bool drawFlipped() const;
    
      template<typename F>
      void drawOnSurface(QRectF const& _rect, F f) const;
      
      /// Draw Canvas from Projector's perspective
      void drawCanvas() const;

      QPointF cursorPosition() const;
      void setCursorPosition(QPointF const&);

    private:

      QPointF cursorPosition_;

      /**@brief Calculates rectangle of this tuning
       **/
      QRectF tuningRect() const;

      /// Return QVector4D of channel correction parameters
      QVector4D channelCorrectionAsVec(Channel) const;

      omni::proj::Tuning& tuning_;

      ContextBoundPtr<visual::WarpGrid> warpGrid_;
      ContextBoundPtr<QOpenGLTexture> blendTex_;
      ContextBoundPtr<omnic::gl::FramebufferRGBA16> calibrationFramebuffer_;

      proj::Calibration calibration_;

      /// Frame buffer which holds a texture with current view image
      ContextBoundPtr<QOpenGLFramebufferObject> warpGridBuffer_;

      ContextBoundPtr<QOpenGLFramebufferObject> testCardFrameBuffer_;

      QRect blendTextureUpdateRect_;

      static ContextBoundPtr<QOpenGLShaderProgram> calibrationShader_;
      static ContextBoundPtr<QOpenGLShaderProgram> testCardShader_;
      static ContextBoundPtr<QOpenGLShaderProgram> blendShader_;
      static ContextBoundPtr<QOpenGLShaderProgram> blendBrushShader_;
      static ContextBoundPtr<QOpenGLShaderProgram> blendBrushCursorShader_;
    };
  }
}

#endif /* OMNI_VISUAL_TUNING_H_ */
