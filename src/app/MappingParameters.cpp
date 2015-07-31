#include <omni/ui/MappingParameters.h>

#include <omni/mapping/Equirectangular.h>
#include <omni/mapping/Fisheye.h>
#include <omni/mapping/CubeMap.h>
#include <omni/mapping/Planar.h>

namespace omni
{
  namespace ui
  {
    MappingParameters::MappingParameters(QWidget* _parent) : 
      ParameterWidget(_parent) 
    {
    }

    MappingParameters::~MappingParameters()
    {
    }

    mapping::Interface* MappingParameters::mapping()
    {
      return mapping_;
    }

    mapping::Interface const* MappingParameters::mapping() const
    {
      return mapping_;
    }

    void MappingParameters::setMapping(mapping::Interface* _mapping)
    {
      mapping_ = _mapping;
      setup();
    }

    void MappingParameters::updateParameters()
    {
      if (!mapping_ || isLocked()) return;

      auto applyRotation = [&](mapping::Rotatable* _rotatable) 
      {
        _rotatable->setRoll(rotation_->x());
        _rotatable->setPitch(rotation_->y());
        _rotatable->setYaw(rotation_->z());
      };

      if (mapping_->getTypeId() == "Equirectangular")
      {
        auto* _equirectangular = static_cast<mapping::Equirectangular*>(mapping_);
        _equirectangular->setStripTop(getParamAsFloat("Strip Top"));
        _equirectangular->setStripBottom(getParamAsFloat("Strip Bottom"));
        applyRotation(_equirectangular);
      } else
      if (mapping_->getTypeId() == "Fisheye")
      {
        auto* _fisheye = static_cast<mapping::Fisheye*>(mapping_);
        _fisheye->setStretch(getParamAsFloat("Stretch"));
        applyRotation(_fisheye);
      } else
      if (mapping_->getTypeId() == "CubeMap")
      {
        auto* _cubemap = static_cast<mapping::CubeMap*>(mapping_);
        applyRotation(_cubemap);
      } else
      if (mapping_->getTypeId() == "Planar")
      {
        auto* _planar = static_cast<mapping::Planar*>(mapping_);
        _planar->setOffset(QVector2D(getParamAsFloat("Offset X"),getParamAsFloat("Offset Y")));
        _planar->setStretch(QVector2D(getParamAsFloat("Stretch X"),getParamAsFloat("Stretch Y")));
      }

      emit parametersUpdated();
    }
    
    void MappingParameters::setup()
    {
      clear();
      if (!mapping_) return;

      this->locked([&]() 
      {
        auto applyRotation = [&](mapping::Rotatable* _rotatable)
        {
          rotation_ = addRotationWidget("Rotation");
          rotation_->setX(_rotatable->roll().degrees());
          rotation_->setY(_rotatable->pitch().degrees());
          rotation_->setZ(_rotatable->yaw().degrees());
        };

        if (mapping_->getTypeId() == "Equirectangular")
        {
          // Set slider values for Equirectangular mapping
          auto* _equirectangular = static_cast<mapping::Equirectangular*>(mapping_);
          applyRotation(_equirectangular);
          auto* _stripTop = addOffsetWidget("Strip Top",0.0,0.0,1.0);
          auto* _stripBottom = addOffsetWidget("Strip Bottom",0.0,0.0,1.0);
          _stripTop->setValue(_equirectangular->stripTop());
          _stripBottom->setValue(_equirectangular->stripBottom());

        } else
        if (mapping_->getTypeId() == "Fisheye")
        {
          // Set slider values for Fisheye mapping
          auto* _fisheye = static_cast<mapping::Fisheye*>(mapping_);
          applyRotation(_fisheye);
          auto* _stretch = addOffsetWidget("Stretch",0.0,0.0,1.0);
          _stretch->setValue(_fisheye->stretch());
        } else
        if (mapping_->getTypeId() == "CubeMap")
        {
          // Set slider values for Cube mapping
          auto* _cubemap = static_cast<mapping::CubeMap*>(mapping_);
          applyRotation(_cubemap);
        } else
        if (mapping_->getTypeId() == "Planar")
        {
          // Set slider values for Planar mapping
          auto* _planar = static_cast<mapping::Planar*>(mapping_);
          auto* _offsetX = addOffsetWidget("Offset X",0.0,-1.0,1.0);
          auto* _offsetY = addOffsetWidget("Offset Y",0.0,-1.0,1.0);
          auto* _stretchX = addOffsetWidget("Stretch X",1.0,0.0,1.0);
          auto* _stretchY = addOffsetWidget("Stretch Y",1.0,0.0,1.0);
          _offsetX->setValue(_planar->offset().x());
          _offsetY->setValue(_planar->offset().y());
          _stretchX->setValue(_planar->stretch().x());
          _stretchY->setValue(_planar->stretch().y());
        }
      });
    }

  }
}
