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

#ifndef SLIM_MIXIN_SPAN_HPP_
#define SLIM_MIXIN_SPAN_HPP_

#include "Range.hpp"
#include "Step.hpp"

namespace slim
{
  namespace mixin
  {
    template<typename DERIVED, typename VALUE>
    struct Span : 
      Range<VALUE>,
      Step<VALUE>
    {
      typedef Range<VALUE> range_type;
      typedef Step<VALUE> step_type;
      typedef VALUE value_type;

      Span() {}

      Span(
        value_type _lower,
        value_type _upper,
        value_type _minimum,
        value_type _maximum,
        value_type _singleStep = 1.0,
        value_type _pageStep = 10.0) :
        range_type(_minimum,_maximum),
        step_type(_singleStep,_pageStep),
        lower_(_lower),
        upper_(_upper)
      {}
 
      qreal ratio() const
      {
        return range_type::ratio(upper_ - lower_);
      }

      void setLowerValue(value_type _lower)
      {
        lower_=_lower;
        validate();
        lowerChangedEvent();
      }
      
      void setUpperValue(value_type _upper)
      {
        upper_=_upper;
        validate();
        upperChangedEvent();
      }

      value_type lowerValue() const
      {
        return step_type::snap() ? step_type::snapped(lower_) : lower_;
      }

      value_type upperValue() const
      {
        return step_type::snap() ? step_type::snapped(upper_) : upper_;
      }
 
    protected:
      void validate()
      {
        range_type::validate();
        if (lower_ > upper_) std::swap(lower_,upper_);
        if (lower_ < range_type::minimum()) lower_ = range_type::minimum();
        if (lower_ > range_type::maximum()) lower_ = range_type::maximum();
        if (upper_ < range_type::minimum()) upper_ = range_type::minimum();
        if (upper_ > range_type::maximum()) upper_ = range_type::maximum();
      }

      virtual void lowerChangedEvent() {}
      virtual void upperChangedEvent() {}

    private:
      value_type lower_ = 0.0, upper_ = 0.0;
    };
  }
}

#endif /* SLIM_MIXIN_SPAN_HPP_ */
