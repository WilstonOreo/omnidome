#ifndef SLIM_MIXIN_RANGE_HPP_
#define SLIM_MIXIN_RANGE_HPP_

#include <limits>
#include "Step.hpp"

namespace slim
{
  namespace mixin
  {
    template<typename VALUE>
    struct Range 
    {
      typedef VALUE value_type;

      Range(
        value_type _minimum = 0.0,
        value_type _maximum = 100.0) :
        minimum_(_minimum),
        maximum_(_maximum)
      {}

      qreal ratio(value_type _v) const
      {
        return  qreal(_v - minimum_) / range();
      }

      value_type range() const
      {
        return maximum_ - minimum_;
      }

      value_type minimum() const
      {
        return minimum_;
      }

      value_type maximum() const
      {
        return maximum_;
      }

      void setMinimum(value_type _minimum)
      {
        minimum_=_minimum;
        validate();
        rangeChangedEvent();
      }
      
      void disableMinimum()
      {
        setMinimum(std::numeric_limits<value_type>::min());
      }

      void setMaximum(value_type _maximum)
      {
        maximum_ = _maximum;
        validate();
        rangeChangedEvent();
      }
      
      void disableMaximum()
      {
        setMaximum(std::numeric_limits<value_type>::max());
      }

      void setRange(value_type _minimum, value_type _maximum)
      {
        minimum_ = _minimum;
        maximum_ = _maximum;
        validate();
        rangeChangedEvent();
      }

      void disableRange()
      {
        setRange(
            std::numeric_limits<value_type>::min(),
            std::numeric_limits<value_type>::max());
      }


      bool minimumUsed() const
      {
        return minimum_ != std::numeric_limits<value_type>::min();
      }
      
      bool maximumUsed() const
      {
        return maximum_ != std::numeric_limits<value_type>::max();
      }

    protected:
      template<typename WIDGET>
      void apply(WIDGET* _widget) const
      {
        _widget->setMinimum(minimum_);
        _widget->setMaximum(maximum_);
      }

      template<typename WIDGET, typename V>
      void apply(WIDGET* _widget, V _value) const
      {
        apply(_widget);
        _widget->setValue(_value);
      }
 
      virtual void validate()
      {
        if (minimum_ > maximum_) std::swap(minimum_,maximum_);
      }

      virtual void rangeChangedEvent()
      {
      }

    private:
      value_type minimum_;
      value_type maximum_;
    };
  }
}
#endif /* SLIM_MIXIN_RANGE_HPP_ */
