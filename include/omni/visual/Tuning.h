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

#ifndef OMNI_VISUAL_TUNING_H_
#define OMNI_VISUAL_TUNING_H_

#include <QRectF>
#include <QOpenGLShaderProgram>
#include <QOpenGLFramebufferObject>
#include <QOpenGLTexture>

#include <omni/proj/Tuning.h>
#include <omni/visual/WarpGrid.h>
#include <omni/visual/Texture32F.h>
#include <omni/proj/Calibration.h>

namespace omni
{
  namespace visual
  {
    class Session;

    /// Tuning visualizer
    class Tuning
    {
    public:
      Tuning(omni::proj::Tuning&);
      ~Tuning();

      omni::proj::Tuning const& tuning() const;

      /// Update shaders, warp grid and blend texture
      void update();

      /// Updates blend texture after it has changed by blend brush
      void updateBlendTexture();

      void drawBlendMask() const;

      /// Update warp grid mesh
      void updateWarpGrid();

      /// Set portion of blend mask which is to be updated
      void setBlendTextureUpdateRect(QRect const&);

      /// Draw Test card image for screen setup
      void drawTestCard(int _id, bool _grayscale = false) const;

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

      /// Draw cursor for blend brush
      void drawCursor(QPointF const& _pos);

      bool initialized() const;

      void free();

      /// Update warp buffer which contains image of projector perspective
      void updateWarpBuffer(visual::Session const* _vizSession);

      void generateCalibrationData();

      void drawCalibratedInput();

    private:

      /**@brief Calculates rectangle of this tuning
       **/
      QRectF tuningRect() const;

      /// Return QVector4D of channel correction parameters
      QVector4D channelCorrectionAsVec(Channel) const;

      omni::proj::Tuning& tuning_;

      std::unique_ptr<visual::WarpGrid> warpGrid_;
      ContextBoundPtr<QOpenGLTexture> blendTex_;
      ContextBoundPtr<Texture32F> calibrationTex_;

      proj::Calibration calibration_;

      /// Frame buffer which holds a texture with current view image
      ContextBoundPtr<QOpenGLFramebufferObject> warpGridBuffer_;

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
