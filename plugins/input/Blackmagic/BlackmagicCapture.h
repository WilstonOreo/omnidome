/* Copyright (c) 2014-2016 "Omnidome" by Michael Winkelmann
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
#ifndef OMNI_INPUT_BLACKMAGICCAPTURE_H_
#define OMNI_INPUT_BLACKMAGICCAPTURE_H_

#include <atomic>
#include <QSize>
#include <QOpenGLFunctions>
#include <QMutex>
#include <DeckLinkAPI.h>
#include "DeckLinkPointer.h"

class IDeckLink;
class IDeckLinkInput;

namespace omni
{
  namespace input
  {
    class BlackmagicCapture :
      public QObject,
      public IDeckLinkInputCallback
    {
      Q_OBJECT
    public:
      BlackmagicCapture(QObject* = nullptr);
      ~BlackmagicCapture();

      void setup(IDeckLink*,IDeckLinkDisplayMode*);
      void start();
      void stop();

      bool isCapturing() const;
      bool isSetup() const;

      GLuint textureId() const;
      QSize const& size() const;

      virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, LPVOID *ppv)
      {
        return E_NOINTERFACE;
      }
      virtual ULONG STDMETHODCALLTYPE AddRef(void);
      virtual ULONG STDMETHODCALLTYPE  Release(void);
      virtual HRESULT STDMETHODCALLTYPE VideoInputFormatChanged(BMDVideoInputFormatChangedEvents, IDeckLinkDisplayMode*, BMDDetectedVideoInputFormatFlags);
      virtual HRESULT STDMETHODCALLTYPE VideoInputFrameArrived(IDeckLinkVideoInputFrame*, IDeckLinkAudioInputPacket*);
    signals:
      void frameReceived();
      void updated();
      void sizeChanged(QSize const&);

    private:
      void update();
      void setupTexture(QSize const&);

      DeckLinkPointer<IDeckLinkInput> input_;
      DeckLinkPointer<IDeckLinkDisplayMode> displayMode_;
      DeckLinkPointer<IDeckLink> device_;

      BMDVideoInputFlags inputFlags_ = bmdVideoInputFlagDefault;
      BMDPixelFormat pixelFormat_ = bmdFormat8BitYUV;
      QSize size_;
      GLuint textureId_ = 0;
      std::atomic<int32_t>				refCount_;
      QMutex mutex_;
      bool formatDetection_ = false;
      QSize videoFrameSize_;
      void* frameBytes_ = nullptr;
    };
  }
}

#endif /* OMNI_INPUT_BLACKMAGICCAPTURE_H_ */
