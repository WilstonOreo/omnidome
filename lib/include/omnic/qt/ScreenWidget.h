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
#ifndef OMNIC_QT_SCREENWIDGET_H_
#define OMNIC_QT_SCREENWIDGET_H_

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <omnic/gl/TextureRef.h>
#include <omnic/gl/Renderer.h>
#include <omnic/util.h>
#include <omnic/CalibratedProjector.h>

namespace omnic {
  namespace qt {
    /// A widget for a complete screen which can hold several calibrated outputs
    class ScreenWidget : 
      public QOpenGLWidget,
      protected QOpenGLFunctions {
      Q_OBJECT
    public:
      explicit ScreenWidget(QWidget *_parent = nullptr);
      virtual ~ScreenWidget();

      /// Return screen geometry of this widget
      QRect const& screenGeometry() const;

      /// Return if the assigned screen is virtual
      bool isVirtual() const;

      /// Add another calibrated projector and setup GL textures
      void addCalibratedProjector(omnic::CalibratedProjector const&);
      
      /// Remove all projectors
      void removeProjectors();

      /// Set input texture
      void setTexture(gl::TextureRef const&);

      /// Return reference to input texture
      gl::TextureRef const& texture() const;

    protected:
      void paintGL();
      void resizeGL(int w, int h);
      void initializeGL();

    private:
      void resetOpenGLState();
      QRect screenGeometry_;
      bool isVirtual_ = false;

      gl::TextureRef tex_;

      int initializations_ = 0;
      std::vector<CalibratedProjector> projectors_;
      std::vector<gl::Renderer> renderers_;
    };
  }
}

#endif /* OMNIC_QT_SCREENWIDGET_H_ */


