/* Copyright (c) 2014-2015 "Omnidome" by Michael Winkelmann
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

#ifndef OMNI_UI_MIXIN_RANGEDVALUE_H_
#define OMNI_UI_MIXIN_RANGEDVALUE_H_

#include "Range.h"

namespace omni {
  namespace ui {
    namespace mixin {
      /**@brief Template class for a value within a certain min/max range
         @tparam DERIVED name of derived class
         @tparam VALUE value type (e.g. int or double)
       **/
      template<typename DERIVED, typename VALUE>
      struct RangedValue :
        Range<VALUE>,
          Step<VALUE>
      {
        typedef Range<VALUE>range_type;
        typedef Step<VALUE> step_type;
        typedef VALUE       value_type;

        /// Default constructor
        RangedValue() {}

        /// Constructor from user defined value and range
        RangedValue(
          value_type _value,
          value_type _minimum,
          value_type _maximum,
          value_type _singleStep = 1.0,
          value_type _pageStep = 10.0) :
          range_type(_minimum, _maximum),
          step_type(_singleStep, _pageStep),
          value_(_value),
          defaultValue_(_value),
          pivot_(_minimum)
        {}

        /// Returns ratio of current value
        qreal ratio() const
        {
          return range_type::ratio(value());
        }

        /// Return value
        value_type value() const
        {
          return step_type::snap() ? step_type::snapped(value_) : value_;
        }

        /// Return default value
        value_type defaultValue() const
        {
          return step_type::snapped(defaultValue_);
        }

        /// Return pivot value
        value_type pivot() const
        {
          return pivot_;
        }

        /// Increate value by single step value
        void increase()
        {
          value_type _oldValue = value_;

          value_ += step_type::singleStep();
          validate();

          if (value_ != _oldValue) valueChangedEvent();
        }

        /// Decrease value by single step value
        void decrease()
        {
          value_type _oldValue = value_;

          value_ -= step_type::singleStep();
          validate();

          if (value_ != _oldValue) valueChangedEvent();
        }

        /// Set new value and emit value changed event
        void setValue(value_type _value)
        {
          value_type _oldValue = value_;

          value_ = _value;
          validate();

          if (value_ != _oldValue) valueChangedEvent();
        }

        /// Reset value to default value
        void reset()
        {
          setValue(defaultValue_);
        }

        /// Set new default value
        void setDefaultValue(value_type _value)
        {
          defaultValue_ = _value;
        }

        /// Set pivot value (0.0 by default)
        void setPivot(value_type _pivot)
        {
          pivot_ = _pivot;
        }

        protected:
          /// Apply ranged value to widget
          template<typename WIDGET>
          void apply(WIDGET *_widget) const
          {
            range_type::apply(_widget, value_);

            _widget->setSingleStep(step_type::singleStep());
          }

          /// Functor to apply for each step
          template<typename F>
          void for_each_step(F f)
          {
            step_type::for_each_step(
              range_type::minimum(), pivot_, f);
            step_type::for_each_step(pivot_,
                                     range_type::maximum(),
                                     f);
          }

          /// Validate minimum, maximum and value
          void validate()
          {
            range_type::validate();

            if (value_ <
                range_type::minimum()) value_ = range_type::minimum();

            if (value_ >
                range_type::maximum()) value_ = range_type::maximum();

            if (pivot_ <
                range_type::minimum()) pivot_ = range_type::minimum();

            if (pivot_ >
                range_type::maximum()) pivot_ = range_type::maximum();
          }

          /// Value changed event which is emitted on changed value
          virtual void valueChangedEvent()
          {}

        private:
          value_type value_        = 0.0;
          value_type defaultValue_ = 0.0;
          value_type pivot_        = 0.0;
      };
    }
  }
}

#endif /* OMNI_UI_MIXIN_RANGEDVALUE_H_ */
