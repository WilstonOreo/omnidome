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

#include <omni/ui/CanvasParameters.h>

namespace omni
{
  namespace ui
  {
    CanvasParameters::CanvasParameters(QWidget* _parent) :
      ParameterWidget(_parent)
    {
    }

    CanvasParameters::~CanvasParameters()
    {
    }

    canvas::Interface* CanvasParameters::canvas()
    {
      return canvas_;
    }

    canvas::Interface const* CanvasParameters::canvas() const
    {
      return canvas_;
    }

    void CanvasParameters::setCanvas(omni::canvas::Interface* _canvas)
    {
      canvas_ = _canvas;
      setup();
    }

    void CanvasParameters::updateParameters()
    {
      if (!canvas_ || isLocked()) return;
/*
      auto setDomeParams = [this](canvas::Dome* _dome)
      {
        _dome->setDiameter(getParamAsFloat("Diameter"));
        _dome->setCenter(QVector3D(
                           getParamAsFloat("X Offset"),
                           getParamAsFloat("Y Offset"),
                           getParamAsFloat("Z Offset")));
      };

      if (canvas_->getTypeId() == "HalfDome")
      {
        auto* _halfDome = static_cast<canvas::HalfDome*>(canvas_);
        setDomeParams(_halfDome);
        _halfDome->angles().roll().setDegrees(rotation_->x());
        _halfDome->angles().pitch().setDegrees(rotation_->y());
        _halfDome->angles().yaw().setDegrees(rotation_->z());
      }
      else if (canvas_->getTypeId() == "FullSphere")
      {
        setDomeParams(static_cast<canvas::FullSphere*>(canvas_));
      }
      else if (canvas_->getTypeId() == "InflatableDome")
      {
        auto* _inflatableDome = static_cast<canvas::InflatableDome*>(canvas_);
        setDomeParams(_inflatableDome);
        _inflatableDome->setSqueeze(getParamAsFloat("Squeeze"));
      }
      /*else if (canvas_->getTypeId() == "Box")
      {
        auto* _box = static_cast<canvas::Box*>(canvas_);
        _box->setSize(QVector3D(
                getParamAsFloat("Width"),
                getParamAsFloat("Length"),
                getParamAsFloat("Height")
        ));
    }
      else if (canvas_->getTypeId() == "Planar")
      {
        auto* _planar = static_cast<canvas::Planar*>(canvas_);
        _planar->setHeight( getParamAsFloat("Length") );
        _planar->setWidth( getParamAsFloat("Width") );
        _planar->angles().roll().setDegrees(rotation_->x());
        _planar->angles().pitch().setDegrees(rotation_->y());
        _planar->angles().yaw().setDegrees(rotation_->z());
        _planar->setCenter(QVector3D(
                           getParamAsFloat("X Offset"),
                           getParamAsFloat("Y Offset"),
                           getParamAsFloat("Z Offset")));
      }
*/
      this->updateCanvasParameters();

      canvas_->update();
      emit parametersUpdated();
    }

    void CanvasParameters::setup()
    {
      clear();
      if (!canvas_) return;
/*
      this->locked([&]()
      {
        auto* _diameter = addOffsetWidget("Diameter",5.0,0.5,20.0);
        auto* _height = addOffsetWidget("Height",1.0,0.1,10.0);
        auto* _length = addOffsetWidget("Length",1.0,0.1,10.0);
        auto* _width = addOffsetWidget("Width",1.0,0.1,10.0);
        auto* _xOffset = addOffsetWidget("X Offset",0.0,-10.0,10.0);
        auto* _yOffset = addOffsetWidget("Y Offset",0.0,-10.0,10.0);
        auto* _zOffset = addOffsetWidget("Z Offset",0.0,-10.0,10.0);
        auto* _squeeze = addOffsetWidget("Squeeze",1.0,0.1,10.0);
        rotation_ = addRotationWidget("Rotation");

        auto _t = canvas_->getTypeId();
        if (_t == "HalfDome")
        {
          /// Retrieve parameters for HalfDome canvas
          auto* _halfDome = static_cast<canvas::HalfDome*>(canvas_);
          _diameter->setValue(_halfDome->diameter());
          _xOffset->setValue(_halfDome->center().x());
          _yOffset->setValue(_halfDome->center().y());
          _zOffset->setValue(_halfDome->center().z());
          rotation_->setX(_halfDome->angles().roll().degrees());
          rotation_->setY(_halfDome->angles().pitch().degrees());
          rotation_->setZ(_halfDome->angles().yaw().degrees());
        } else
        if (_t == "InflatableDome")
        {
          /// Retrieve parameters for InflatableDome canvas
          auto* _inflatableDome = static_cast<canvas::InflatableDome*>(canvas_);
          _diameter->setValue(_inflatableDome->diameter());
          _squeeze->setValue(_inflatableDome->squeeze());
          _xOffset->setValue(_inflatableDome->center().x());
          _yOffset->setValue(_inflatableDome->center().y());
          _zOffset->setValue(_inflatableDome->center().z());
        } else
        if (_t == "FullSphere")
        {
          /// Retrieve parameters for FullSphere canvas
          auto* _fullsphere = static_cast<canvas::FullSphere*>(canvas_);
          _diameter->setValue(_fullsphere->diameter());
          _xOffset->setValue(_fullsphere->center().x());
          _yOffset->setValue(_fullsphere->center().y());
          _zOffset->setValue(_fullsphere->center().z());
      } else
        if (_t == "Box")
        {
          /// Retrieve parameters for Box canvas
          auto* _box = static_cast<canvas::Box*>(canvas_);
          _width->setValue(_box->size().x());
          _length->setValue(_box->size().y());
          _height->setValue(_box->size().z());
          _xOffset->setValue(_box->center().x());
          _yOffset->setValue(_box->center().y());
          _zOffset->setValue(_box->center().z());
      } else
        if (_t == "Planar")
        {
          /// Retrieve parameters for Planar canvas
          auto* _planar = static_cast<canvas::Planar*>(canvas_);
          _width->setValue(_planar->width());
          _length->setValue(_planar->height());
          _xOffset->setValue(_planar->center().x());
          _yOffset->setValue(_planar->center().y());
          _zOffset->setValue(_planar->center().z());
          rotation_->setX(_planar->angles().roll().degrees());
          rotation_->setY(_planar->angles().pitch().degrees());
          rotation_->setZ(_planar->angles().yaw().degrees());
        }
        }); */
    }
  }
}
