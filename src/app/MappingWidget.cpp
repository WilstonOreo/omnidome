#include <omni/ui/MappingWidget.h>

#include <slim/GradientSlider.h>

namespace omni
{
  namespace ui
  {
    MappingWidget::MappingWidget(
      Mapping* _mapping,
      QWidget* _parent) :
      QWidget(_parent)
    {
      setup();
      setMapping(_mapping);
    }

    MappingWidget::MappingWidget(
        QWidget* _parent) :
      QWidget(_parent)
    {
      setup();
    }

    MappingWidget::~MappingWidget()
    {
    }
      
    Mapping* MappingWidget::mapping()
    {
      return mapping_;
    }

    Mapping const* MappingWidget::mapping() const
    {
      return mapping_;
    }

    void MappingWidget::setMapping(Mapping* _mapping)
    {
      mapping_=_mapping;
    }
 
    slim::RangedFloat* MappingWidget::addWidget(QString const& _str,float _value,float _min,float _max)
    {
      auto* _widget = new slim::RangedFloat(_str,_value,_min,_max);
      _widget->setUseDefaultValue(true);
      layout()->addWidget(_widget);
      parameters_[_str] = _widget;
      return _widget;
    }

    slim::RangedFloat* MappingWidget::addAngleWidget(QString const& _str,float _value,float _min,float _max)
    {
        auto* _angle = addWidget(_str,_value,_min,_max);
        _angle->setSuffix("°");
        _angle->setSingleStep(5.0);
        _angle->setPageStep(45.0);
        return _angle;
    }

    slim::RangedFloat* MappingWidget::addOffsetWidget(QString const& _str,float _value, float _min, float _max)
    {
        auto* _offset = addWidget(_str,_value,_min,_max);
        _offset->setSuffix("m");
        _offset->setSingleStep(0.1);
        _offset->setPageStep(0.1); 
        return _offset;
    }


    void MappingWidget::setup()
    {
      QVBoxLayout* layout_ = new QVBoxLayout;
      layout_->setContentsMargins(3,3,3,3);
      layout_->setSpacing(3);

      QLinearGradient _gradient(QPointF(0.0,0.0),QPointF(1.0,0.0));
      _gradient.setCoordinateMode(QLinearGradient::ObjectBoundingMode);
      _gradient.setColorAt(0.0/6.0,"#FF0000");
      _gradient.setColorAt(1.0/6.0,"#FFFF00");
      _gradient.setColorAt(2.0/6.0,"#00FF00");
      _gradient.setColorAt(3.0/6.0,"#00FFFF");
      _gradient.setColorAt(4.0/6.0,"#0000FF");
      _gradient.setColorAt(5.0/6.0,"#FF00FF");
      _gradient.setColorAt(6.0/6.0,"#FF0000");

      auto* _gradientSlider = new slim::GradientSlider("Projector");
      _gradientSlider->setGradient(_gradient);
      _gradientSlider->setShowColorOnly(true);
      layout_->addWidget(_gradientSlider);

      auto* _fov =  new slim::RangedFloat("FOV",60.0,10.0,160.0);
      _fov->setSingleStep(4.0);
      _fov->setPageStep(45.0);
      _fov->setSuffix("°");

      auto* _throwRatio = new slim::RangedFloat("Throw Ratio",1.0,0.1,5.0);
      _throwRatio->setSingleStep(0.1);
      _throwRatio->setPageStep(0.3);

      auto* _yaw = addAngleWidget("Yaw",0.0,0.0,360.0);

      auto* _towerHeight = addOffsetWidget("Tower Height",2.0,-5.0,10.0);
      _towerHeight->setSingleStep(0.1);
      _towerHeight->setPageStep(1.0);
      _towerHeight->setPivot(0.0);

      auto* _distance = addOffsetWidget("Distance",5.0,0.0,10.0); 
      _distance->setPageStep(1.0);

      auto* _shift = addOffsetWidget("Shift",0.0,-2.0,2.0);
      _shift->setPageStep(1.0);
      _shift->setPivot(0.0);

      auto* _pitch = addAngleWidget("Pitch",30.0,-90.0,90.0); 
      _pitch->setPivot(0.0);

      auto* _roll = addAngleWidget("Roll",0.0,-45.0,45.0);
      _roll->setSingleStep(1.0);
      _roll->setPageStep(5.0);
      _roll->setPivot(0.0);

      auto* _deltaYaw = addAngleWidget("DeltaYaw",0.0,-45.0,45.0);
      _deltaYaw->setSingleStep(1.0);
      _deltaYaw->setPageStep(5.0);
      _deltaYaw->setPivot(0.0);
    }    
  }
}
