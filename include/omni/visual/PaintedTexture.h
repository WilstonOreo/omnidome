/* Copyright (c) 2014-2017 "Omnidome" by Michael Winkelmann
undefined
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
#ifndef OMNI_VISUAL_PAINTEDTEXTURE_H_
#define OMNI_VISUAL_PAINTEDTEXTURE_H_

#include <memory>
#include <QOpenGLTexture>
#include <QFont>

class QPainter;

namespace omni {
  namespace visual {
    class PaintedTexture {
    public:
      PaintedTexture();
      virtual ~PaintedTexture();

      QSize size() const;
      virtual void setSize(QSize const&);

      void update();

      GLuint textureId() const;
      GLuint target() const;

    private:
      virtual void paint(QPainter&) = 0;

      QSize size_;
      std::unique_ptr<QOpenGLTexture> texture_;
    };

    class NumberGlyphsTexture : public PaintedTexture {
    public:
      NumberGlyphsTexture();
      ~NumberGlyphsTexture();

      void setSize(QSize const&);

      QColor const& color() const;
      void setColor(QColor const&);

      QFont const& font() const;
      void setFont();

    private:
      void paint(QPainter&);

      QColor color_;
      QFont font_;
    };

    class TextTexture : public PaintedTexture {
    public:
      TextTexture();
      ~TextTexture();

      QFont const& font() const;
      void setFont(QFont const&);

      Qt::Alignment alignment() const;
      void setAlignment(Qt::Alignment);

      QString const& text() const;
      void setText(QString const&);

      QColor const& color() const;
      void setColor(QColor const&);

    private:
      void paint(QPainter&);

      QFont font_;
      QString text_;
      Qt::Alignment alignment_ = Qt::AlignCenter;
      QColor color_;
    };
  }
}

#endif /* OMNI_VISUAL_PAINTEDTEXTURE_H_ */
