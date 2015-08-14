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

#ifndef OMNI_UI_BLEND_H_
#define OMNI_UI_BLEND_H_

#include <memory>
#include <QGroupBox>

namespace omni
{
  class Session;
  class BlendMask;
  
  namespace ui
  {
    namespace Ui
    {
      class Blend;
    }

    class Blend : public QGroupBox
    {
      Q_OBJECT
    public:
      Blend(QWidget* = nullptr);
      ~Blend();

      Session const* session() const;
      void setSession(Session* _session);
    
    public slots:
      /// Update sliders from given blend mask
      void updateBlendMask();

      /// Set slider values to blend mask
      void changeBlendMask();

      

    signals:
      void blendMaskChanged();

    private:
      omni::BlendMask const* blendMask() const;
      omni::BlendMask* blendMask();

      bool locked_ = false;
      Session* session_ = nullptr;
      std::unique_ptr<Ui::Blend> ui_;
    };
  }
}

#endif /* OMNI_UI_BLEND_H_ */
