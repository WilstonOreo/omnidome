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

#include <omni/ui/ParameterWidget.h>

#include <QKeyEvent>

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
    }

    void ParameterWidget::updateParameters()
    {
      emit parametersUpdated();
    }

    RangedFloat* ParameterWidget::addWidget(QString const& _str,float _value,float _min,float _max)
    {
      auto* _widget = new RangedFloat(_str,_value,_min,_max,this);
      if (layout())
        layout()->addWidget(_widget);

      this->installEventFilter(_widget);

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
    RangedInt* ParameterWidget::addIntegerWidget(QString const& _str,int _value,int _min, int _max)
    {
      if (getWidget(_str)) {
        return static_cast<RangedInt*>(getWidget(_str));
      }
      auto* _widget = new RangedInt(_str,_value,_min,_max,this);
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

    RangedFloat* ParameterWidget::addAngleWidget(QString const& _str,float _value,float _min,float _max)
    {
      if (getWidget(_str)) {
        return static_cast<RangedFloat*>(getWidget(_str));
      }
      auto* _angle = addWidget(_str,_value,_min,_max);
      _angle->setSuffix("°");
      _angle->setSingleStep(5.0);
      _angle->setPageStep(45.0);
      return _angle;
    }

    RangedFloat* ParameterWidget::addOffsetWidget(QString const& _str,float _value, float _min, float _max)
    {
      if (getWidget(_str)) {
        return static_cast<RangedFloat*>(getWidget(_str));
      }
      auto* _offset = addWidget(_str,_value,_min,_max);
      _offset->setSuffix("m");
      _offset->setSingleStep(0.001);
      _offset->setPageStep(0.01);
      _offset->setPivot(0.0);
      this->registerScaledSlider(_offset);
      return _offset;
    }

    QCheckBox* ParameterWidget::addCheckBox(QString const& _text, bool _checked)
    {
      if (getWidget(_text)) {
        auto* _checkbox = static_cast<QCheckBox*>(getWidget(_text));
        return _checkbox;
      }

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
      auto* _widget = static_cast<RangedFloat*>(this->parameterMap_.at(_str));
      if (!_widget) return 0.0;


      return _widget->value();
    }

    void ParameterWidget::setParamAsFloat(QString const& _str, double _value)
    {
      auto* _widget = static_cast<RangedFloat*>(this->parameterMap_.at(_str));
      if (!_widget) return;

      _widget->setValue(_value);
    }

    int ParameterWidget::getParamAsInt(QString const& _str) const
    {
      auto* _widget = static_cast<RangedInt*>(this->parameterMap_.at(_str));
      if (!_widget) return 0;
      return _widget->value();
    }

    void ParameterWidget::setParamAsInt(QString const& _str, int _value)
    {
      auto* _widget = static_cast<RangedInt*>(this->parameterMap_.at(_str));
      if (!_widget) return;

      _widget->setValue(_value);
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

    int ParameterWidget::focusId() const {
        int _id = 0;
        for (int i = 0; i < layout()->count(); ++i) {
            auto _param = layout()->itemAt(i)->widget();
            if (_param->hasFocus()) {
                return _id;
            }
            ++_id;
        }
        return -1;
    }

    void ParameterWidget::focus(int _index) {
        if (!layout()->itemAt(_index)) return;

        layout()->itemAt(_index)->widget()->setFocus();
    }

    void ParameterWidget::focusFirst() {
        focus(firstFocusId());
    }

    void ParameterWidget::focusLast() {
        focus(layout()->count()-1);
    }

    void ParameterWidget::setScale(float _scale) {
        mixin::Scale<omni::ui::RangedFloat>::setScale(_scale);
    }

    void ParameterWidget::setUnit(QString const& _unitAbbr) {
        mixin::Scale<omni::ui::RangedFloat>::setSuffix(_unitAbbr);
    }

    void ParameterWidget::keyPressEvent(QKeyEvent* _event) {
          if (_event->key() == Qt::Key_Up) {
              focusPrev(false);
          }
          if (_event->key() == Qt::Key_Down) {
              focusNext(false);
          }
      }

    bool ParameterWidget::focusNext(bool _circular) {
        int _focusId = focusId();

        if (_focusId != -1) {
            int _nextFocusId = _focusId + 1;
            if (_nextFocusId >= layout()->count()) {
                _nextFocusId = _circular ? firstFocusId() : layout()->count() - 1;
            }
            auto _param = layout()->itemAt(_nextFocusId)->widget();
            _param->setFocus();
            return _focusId != _nextFocusId;
        }
        return false;
    }

    bool ParameterWidget::focusPrev(bool _circular) {
        int _focusId = focusId();

        if (_focusId != -1) {
            int _prevFocus = _focusId - 1;
            if (_prevFocus < firstFocusId()) {
                _prevFocus = _circular ? layout()->count() - 1 : firstFocusId();
            }
            auto _param = layout()->itemAt(_prevFocus)->widget();
            _param->setFocus();
            return _focusId != _prevFocus;
        }
        return false;
    }

    Rotation* ParameterWidget::addRotationWidget(QString const& _str)
    {
      if (getWidget(_str)) {
        return static_cast<Rotation*>(getWidget(_str));
      }
      auto* _widget = new Rotation(0.0,0.0,0.0,this);

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

    omni::ui::AffineTransform* ParameterWidget::addAffineTransformWidget(
        QString const& _id, omni::AffineTransform* _transform) {
        if (getWidget(_id)) {
            return static_cast<omni::ui::AffineTransform*>(getWidget(_id));
          }

        auto* _widget = ui::makeWidget<omni::ui::AffineTransform>(_transform);

        if (layout())
            layout()->addWidget(_widget);

        /// Install event filter to pipe through focus event to parent widget
        _widget->installEventFilter(this);

        /// Signal-slot connection for updating the data model
        connect(_widget,SIGNAL(dataModelChanged()),this,SLOT(updateParameters()));

        parameterMap_[_id] = _widget;
        parameters_.emplace_back(_widget);
        return _widget;
    }
  }
}
