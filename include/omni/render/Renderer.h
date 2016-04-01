/* Copyright (c) 2014-2016 "Omnidome" by cr8tr
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

#ifndef OMNI_RENDER_RENDERER_H_
#define OMNI_RENDER_RENDERER_H_

#include <map>
#include <QScreen>
#include <QImage>
#include <omni/RenderBuffer.h>
#include <omni/proj/ColorCorrection.h>

namespace omni {
  class Session;
  namespace proj {
    class Tuning;
  }

  namespace render {
    class Renderer {
      public:
        Renderer(Session& _session);

        void renderToFile(QString const& _filename);

        void render(proj::Tuning const *_tuning,
                    RenderBuffer& _buffer) const;
        void render(proj::Tuning const *_tuning,
                    QImage& _image,
                    int _height = 0) const;

        // RenderOutput render();

        std::map<QScreen const *, QImage>
             stitchScreens(std::vector<const proj::Tuning *>const&) const;

      private:
        void getUpper8bit(RenderBuffer const& _buffer,
                          QImage& _image) const;
        void getLower8bit(RenderBuffer const& _buffer,
                          QImage& _image) const;
        void getAlphaMask(RenderBuffer const & _buffer,
                          QImage & _image,
                          Channel = Channel::ALL) const;
        void encodeColorCorrection(ColorCorrection const &, Channel,
                                   QImage &) const;

        void bufferToRGBImage(RenderBuffer const& _buffer,
                              QImage& _image);

        template<typename PROJECTION, typename MODELVIEW>
        static void renderToBuffer(RenderBuffer& _buffer,
                                   PROJECTION _proj,
                                   MODELVIEW _mv);

        template<typename OPERATION>
        static void bufferToImage(RenderBuffer const& _buffer,
                                  QImage& _image,
                                  OPERATION _f);

        Session& session_;
    };
  }
}

#endif /* OMNI_RENDER_RENDERER_H_ */
