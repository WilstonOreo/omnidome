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

#ifndef OMNI_UI_MIXIN_SNAP_H_
#define OMNI_UI_MIXIN_SNAP_H_

#include <math.h>
#include <cmath>

namespace omni {
  namespace ui {
    namespace mixin {
      /**@brief Mixin Stepping value class template
         @detail Singla and page step
       **/
      template<typename VALUE>
      struct Step
      {
        typedef VALUE value_type;

        /// Construct with a single step, page step value
        Step(
          value_type _singleStep = 1.0,
          value_type _pageStep = 10.0,
          bool       _snap = false) :
          singleStep_(_singleStep),
          pageStep_(_pageStep),
          snap_(_snap)
        {}

        /// Return snapped value
        qreal snapped(value_type _v) const
        {
          return value_type(round(qreal(_v) / singleStep())) *
                 singleStep();
        }

        /// Snap value (align to single step value)
        bool snap() const
        {
          return snap_;
        }

        /// Set boolean if value is to be snapped
        void setSnap(bool _snap)
        {
          snap_ = _snap;
        }

        /// Return value for single step
        value_type singleStep() const
        {
          return singleStep_;
        }

        /// Return value for page step
        value_type pageStep() const
        {
          return pageStep_;
        }

        /// Set new single step value
        void setSingleStep(value_type _singleStep)
        {
          singleStep_ = _singleStep;
        }

        /// Set new value for page step
        void setPageStep(value_type _pageStep)
        {
          pageStep_ = _pageStep;
        }

        protected:
          /// Functor to be applied for each step between min and maximum
          template<typename MIN, typename MAX, typename F>
          void for_each_step(MIN _min, MAX _max, F f)
          {
            int _step = 0;

            for (value_type i = _min; i <= _max; i += singleStep())
            {
              int _rI = int(std::round(double(i) / singleStep()));

              bool _isPage = int(pageStep()) == 0 ? false :
                             abs(_rI) %
                             int(std::round(double(pageStep()) /
                                            singleStep())) == 0;
              f(_step, i, _isPage);
              ++_step;
            }
          }

        private:
          value_type singleStep_;
          value_type pageStep_;
          bool       snap_;
      };
    }
  }
}

#endif /* OMNI_UI_MIXIN_SNAP_H_ */
