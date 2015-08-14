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

#include <omni/visual/Surface.h>

#include <omni/visual/util.h>

namespace omni
{
  namespace visual
  {
    bool Surface::ignoreAspect() const
    {
      return ignoreAspect_;
    }
    void Surface::setIgnoreAspect(bool _ignoreAspect)
    {
      ignoreAspect_ = _ignoreAspect;
    }
      
    bool Surface::upsideDown() const
    {
      return upsideDown_;
    }

    void Surface::setUpsideDown(bool _upsideDown)
    {
      upsideDown_=_upsideDown;
    }
  
    qreal Surface::border() const
    {
      return border_;
    }

    void Surface::setBorder(qreal _border)
    {
      border_=_border;
    }
 
    QSize Surface::sceneSize() const
    {
      return sceneSize_;
    }

    void Surface::setSceneSize(QSize _sceneSize) 
    {
      sceneSize_=_sceneSize; 
    }

    QRectF Surface::sceneRect(bool _upsideDown) const
    {
      qreal _sceneAspect = util::aspect(sceneSize_);
      qreal _windowAspect = util::aspect(windowSize_);
      float b = border_ * 0.5;
      float _left = -0.5 - b,_right = 0.5 + b,_bottom = -0.5 - b,_top = 0.5 + b;

      if (!upsideDown_)
      {
        std::swap(_bottom,_top);
      }

      if (!ignoreAspect())
      {
        if (_windowAspect > _sceneAspect)
        {
          _top *= _windowAspect / _sceneAspect;
          _bottom *=  _windowAspect / _sceneAspect;
        }
        else
        {
          _left *= _sceneAspect / _windowAspect;
          _right *= _sceneAspect / _windowAspect;
        }
      }
      return QRectF(QPointF(_left,_top),QPointF(_right,_bottom));
    }

    QSize Surface::windowSize() const
    {
      return windowSize_;
    }

    void Surface::setWindowSize(QSize _windowSize)
    {
      windowSize_ = _windowSize;
    }
  }
}
