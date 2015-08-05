#ifndef SLIM_MIXIN_RANGEDVALUE_HPP_
#define SLIM_MIXIN_RANGEDVALUE_HPP_

#include "Range.hpp"

namespace slim
{
  namespace mixin
  {
    /**@brief Template class for a value within a certain min/max range
      @tparam DERIVED name of derived class
      @tparam VALUE value type (e.g. int or double)
     **/
    template<typename DERIVED, typename VALUE>
    struct RangedValue : 
      Range<VALUE>,
      Step<VALUE>
    { 
      typedef Range<VALUE> range_type;
      typedef Step<VALUE> step_type;
      typedef VALUE value_type;

      /// Default constructor
      RangedValue() {}

      /// Constructor from user defined value and range
      RangedValue(
        value_type _value,
        value_type _minimum,
        value_type _maximum,
        value_type _singleStep = 1.0,
        value_type _pageStep = 10.0) :
        range_type(_minimum,_maximum),
        step_type(_singleStep,_pageStep),
        value_(_value),
        defaultValue_(_value),
        pivot_(_minimum)
      {
      }

      /// Returns ratio of current value
      qreal ratio() const
      {
        return range_type::ratio(value());
      }

      value_type value() const
      {
        return step_type::snap() ? step_type::snapped(value_) : value_;
      }

      value_type defaultValue() const
      {
        return step_type::snapped(defaultValue_);
      }

      value_type pivot() const
      {
        return pivot_;
      }

      void increase()
      {
        value_type _oldValue = value_;
        value_ += step_type::singleStep();
        validate();
 
        if (value_ != _oldValue) 
          valueChangedEvent();
      }

      void decrease()
      {
        value_type _oldValue = value_;
        value_ -= step_type::singleStep();
        validate();
        
        if (value_ != _oldValue) 
          valueChangedEvent();
      }

      void setValue(value_type _value)
      {
        value_type _oldValue = value_;
        value_=_value;
        validate();

        if (value_ != _oldValue)
          valueChangedEvent();
      }

      void reset()
      {
        setValue(defaultValue_);
      }

      void setDefaultValue(value_type _value)
      {
        defaultValue_ = _value;
      }

      void setPivot(value_type _pivot)
      {
        pivot_=_pivot;
      }

    protected:
      template<typename WIDGET>
      void apply(WIDGET* _widget) const
      {
        range_type::apply(_widget,value_);
        _widget->setSingleStep(step_type::singleStep());
      }
      
      template<typename F>
      void for_each_step(F f)
      {
        step_type::for_each_step(range_type::minimum(),pivot_,f);
        step_type::for_each_step(pivot_,range_type::maximum(),f);
      }

      void validate()
      {
        range_type::validate();
        if (value_ < range_type::minimum()) value_ = range_type::minimum();
        if (value_ > range_type::maximum()) value_ = range_type::maximum(); 
        if (pivot_ < range_type::minimum()) pivot_ = range_type::minimum();
        if (pivot_ > range_type::maximum()) pivot_ = range_type::maximum();
      }

      virtual void valueChangedEvent()
      {
      }

    private:
      value_type value_ = 0.0;
      value_type defaultValue_ = 0.0;
      value_type pivot_ = 0.0;
    };
  }
}

#endif /* SLIM_MIXIN_RANGEDVALUE_HPP_ */
