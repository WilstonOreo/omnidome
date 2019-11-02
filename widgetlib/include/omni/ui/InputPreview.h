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

#ifndef OMNI_UI_INPUTPREVIEW_H_
#define OMNI_UI_INPUTPREVIEW_H_

#include <QOpenGLShaderProgram>
#include <omni/input/Interface.h>
#include <omni/ui/GLView.h>

namespace omni
{
  namespace ui
  {
    /// GLView for visualizing an input
    class OMNIWIDGET_EXPORT InputPreview : public GLView
    {
      Q_OBJECT
    public:
      InputPreview(QWidget* = nullptr);
      InputPreview(omni::input::Interface* _input, QWidget* = nullptr);
      ~InputPreview();

      /// Return relative border value
      float border() const;

      /// Return pointer to input
      omni::input::Interface* input();

      /// Return pointer to input (const version)
      omni::input::Interface const* input() const;

      /// Set pointer to input
      void setInput(omni::input::Interface*);

      /**@brief Transforms mouse position on widget to normalized position on input
       * @detail Position in range (0.0,0.0) and (1.0,1.0)
       **/
      QPointF screenPos(QPointF const& _pos) const;

    public slots:
      /// Set relative border distance
      void setBorder(float);

    protected:
      void showEvent(QShowEvent*);
    signals:
      void inputChanged();

    private:
      /// Return view rect
      QRectF viewRect() const;
      
      virtual void paint();

      bool initialize();

      /// Relative border
      float border_ = 0.0;

      std::unique_ptr<QOpenGLShaderProgram> shader_;
      omni::input::Interface* input_ = nullptr;
    };
  }
}

#endif /* OMNI_UI_INPUTPREVIEW_H_ */
