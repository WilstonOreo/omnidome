/* Copyright (c) 2014-2016 "OmniCalibration" by Michael Winkelmann
 * Calibration Format for Omnidome (http://omnido.me).
 * Created by Michael Winkelmann aka Wilston Oreo (@WilstonOreo)
 *
 * This file is part of Omnidome.
 *
 * Simplified BSD license
 * Copyright (c) 2016
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef OMNIC_QT_OUTPUT_H_
#define OMNIC_QT_OUTPUT_H_ 

#include <vector>
#include <QObject>
#include <omnic/gl/TextureRef.h>
#include <omnic/qt/util.h>
#include <omnic/Calibration.h>
#include <omnic/qt/ScreenWidget.h>

namespace omnic {
  namespace qt {
    /// This class manages all screen output widgets for a calibration
    class Output : public QObject {
      Q_OBJECT
    public:
      Output();
      ~Output();

      /// Show all output screens
      void showScreens();

      /// Hide all output screen
      void hideScreens();

      /// Show virtual screens (non fullscreen windows)
      void showVirtualScreens();

      /// Hide virtual screens (non fullscreen windows)
      void hideVirtualScreens();
    
      /// Set calibration and setup output screens
      void setCalibration(omnic::Calibration const&);
      
      /// Return current calibration
      omnic::Calibration const& calibration() const;

      /// Remove calibration and remove screen widgets
      void removeCalibration();

      /**@brief Set input texture with id, size and target
         @param _textureId OpenGL id of texture
         @param _textureWidth Width (in pixels) of texture
         @param _textureHeight Height (in pixels) of texture
         @param _target Texture target (GL_TEXTURE_RECTANGLE by default)
       **/
      void setTexture(GLuint _textureId,
          uint32_t _textureWidth,
          uint32_t _textureHeight,
          GLuint _target = GL_TEXTURE_RECTANGLE 
      );

      /// Set texture by given reference to texture
      void setTexture(gl::TextureRef const& _texture);

      /**@brief Return reference to texture.
       * @detail Texture id, size and target is accessible from this struct
       **/
      gl::TextureRef const& texture() const;

      /// Update all widgets
      void update();

    private:
      ScreenWidget* getWidgetFromScreen(omnic::Rect const&);

      gl::TextureRef texture_;
      omnic::Calibration calibration_;
      std::vector<QUniquePtr<ScreenWidget>> widgets_;
    };
  }
}

#endif /* OMNIC_QT_OUTPUT_H_ */
