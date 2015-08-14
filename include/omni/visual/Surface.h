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

#ifndef OMNI_VISUAL_SURFACE_H_
#define OMNI_VISUAL_SURFACE_H_

#include <QRectF>
#include <omni/visual/Cursor.h>

namespace omni
{
  namespace visual
  {
    /**@brief visual::Surface handles painting on a orthogonal 2D canvas
      *@detail 
     **/
    class Surface 
    {
    public:
      bool ignoreAspect() const;
      void setIgnoreAspect(bool); 

      bool upsideDown() const;
      void setUpsideDown(bool);

      qreal border() const;
      void setBorder(qreal);
      
      QSize sceneSize() const;
      void setSceneSize(QSize);
      QRectF sceneRect(bool _upsideDown = false) const;

      QSize windowSize() const;
      void setWindowSize(QSize);
      QRectF windowRect(bool _upsideDown) const;

    private:
      bool ignoreAspect_ = false;
      bool upsideDown_ = false;
      qreal border_ = 0.0;
      QSize windowSize_;
      QSize sceneSize_;
      Cursor cursor_;
    };
  }
}


#endif /* OMNI_VISUAL_SURFACE_H_ */
