#include <omni/ui/ParameterWidget.h>

namespace omni
{
  namespace ui
  {
    ParameterWidget::ParameterWidget(QWidget* _parent) :
      QWidget(_parent)
    { 
    }

    ParameterWidget::~ParameterWidget() 
    {
    }


    void ParameterWidget::clear()
    {   
      this->locked([&]() 
      {
        parameters_.clear();
        parameterMap_.clear();

        if (layout()) 
        {
          QLayoutItem *item = nullptr;
          while ((item = layout()->takeAt(0)))
          {
            delete item->widget();
            delete item;
          }
        }
      });
    }
      
    void ParameterWidget::updateParameters()
    {
      if (isLocked()) return;

      emit parametersUpdated();
    }

    slim::RangedFloat* ParameterWidget::addWidget(QString const& _str,float _value,float _min,float _max)
    {
      auto* _widget = new slim::RangedFloat(_str,_value,_min,_max,this);
      if (layout())
        layout()->addWidget(_widget);

      _widget->setUseDefaultValue(true);

      /// Install event filter to pipe through focus event to parent widget
      _widget->installEventFilter(this);

      /// Signal-slot connection for updating the data model 
      connect(_widget,SIGNAL(valueChanged()),this,SLOT(updateParameters()));
      parameterMap_[_str] = _widget;
      parameters_.emplace_back(_widget);
      return _widget;
    }
      
    /// Adds a integer widget with a name, a value and min-max range
    slim::RangedInt* ParameterWidget::addIntegerWidget(QString const& _str,int _value,int _min, int _max)
    {
      auto* _widget = new slim::RangedInt(_str,_value,_min,_max,this);
      if (layout())
        layout()->addWidget(_widget);

      _widget->setRange(_min,_max);
      _widget->setSingleStep(1);
      _widget->setPageStep(2);
      _widget->setDefaultValue(_value);
      _widget->setUseDefaultValue(true);

      /// Install event filter to pipe through focus event to parent widget
      _widget->installEventFilter(this);

      /// Signal-slot connection for updating the data model 
      connect(_widget,SIGNAL(valueChanged()),this,SLOT(updateParameters()));
      parameterMap_[_str] = _widget;
      parameters_.emplace_back(_widget);
      return _widget;
    }

    slim::RangedFloat* ParameterWidget::addAngleWidget(QString const& _str,float _value,float _min,float _max)
    {
      auto* _angle = addWidget(_str,_value,_min,_max);
      _angle->setSuffix("°");
      _angle->setSingleStep(5.0);
      _angle->setPageStep(45.0);
      return _angle;
    }

    slim::RangedFloat* ParameterWidget::addOffsetWidget(QString const& _str,float _value, float _min, float _max)
    {
      auto* _offset = addWidget(_str,_value,_min,_max);
      _offset->setSuffix("m");
      _offset->setSingleStep(0.1);
      _offset->setPageStep(0.1);
      return _offset;
    }
    
    QCheckBox* ParameterWidget::addCheckBox(QString const& _text, bool _checked)
    {
      auto* _checkbox = new QCheckBox(_text, this);
      if (layout())
        layout()->addWidget(_checkbox);
      
      _checkbox->setChecked(_checked);

      /// Signal-slot connection for updating the data model 
      connect(_checkbox,SIGNAL(clicked()),this,SLOT(updateParameters()));
      parameterMap_[_text] = _checkbox;
      parameters_.emplace_back(_checkbox);
      return _checkbox;

    }

    double ParameterWidget::getParamAsFloat(QString const& _str) const
    {
      auto* _widget = static_cast<slim::RangedFloat*>(this->parameterMap_.at(_str));
      if (!_widget) return 0.0;
      return _widget->value();
    }
   
    void ParameterWidget::setParamAsFloat(QString const& _str, double _value) 
    {
      auto* _widget = static_cast<slim::RangedFloat*>(this->parameterMap_.at(_str));
      if (!_widget) return;

      locked([&]() 
      {
        _widget->setValue(_value);
      });      
    }

    int ParameterWidget::getParamAsInt(QString const& _str) const
    {
      auto* _widget = static_cast<slim::RangedInt*>(this->parameterMap_.at(_str));
      if (!_widget) return 0;
      return _widget->value();
    }
    
    void ParameterWidget::setParamAsInt(QString const& _str, int _value)
    {
      auto* _widget = static_cast<slim::RangedInt*>(this->parameterMap_.at(_str));
      if (!_widget) return;

      locked([&]() 
      {
        _widget->setValue(_value);
      });      
    }

    bool ParameterWidget::getParamAsBool(QString const& _str) const
    {
      auto* _widget = static_cast<QCheckBox*>(this->parameterMap_.at(_str));
      if (!_widget) return false;
      return _widget->isChecked();
    }
      
      
    QWidget* ParameterWidget::getWidget(QString const& _id)
    {
      if (!parameterMap_.count(_id)) return nullptr;
      return parameterMap_[_id];
    }
      
    void ParameterWidget::setParametersVisible(bool _visible)
    {
      for (auto& _parameter : parameters_) 
        _parameter->setVisible(_visible);
    }

    slim::Rotation* ParameterWidget::addRotationWidget(QString const& _str)
    {
      auto* _widget = new slim::Rotation(0.0,0.0,0.0,this);

      if (layout())
        layout()->addWidget(_widget);

      /// Install event filter to pipe through focus event to parent widget
      _widget->installEventFilter(this);

      /// Signal-slot connection for updating the data model 
      connect(_widget,SIGNAL(xChanged()),this,SLOT(updateParameters()));
      connect(_widget,SIGNAL(yChanged()),this,SLOT(updateParameters()));
      connect(_widget,SIGNAL(zChanged()),this,SLOT(updateParameters()));

      parameterMap_[_str] = _widget;
      parameters_.emplace_back(_widget);
      return _widget;
    }
  }
}
