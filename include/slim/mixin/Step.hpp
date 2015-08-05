#ifndef SLIM_MIXIN_SNAP_HPP_
#define SLIM_MIXIN_SNAP_HPP_

#include <math.h>
#include <cmath>

namespace slim
{
  namespace mixin
  {
    /// Stepping value class template 
    template<typename VALUE>
    struct Step
    {
      typedef VALUE value_type;
    
      Step(
          value_type _singleStep = 1.0,
          value_type _pageStep = 10.0,
          bool _snap = false) : 
        singleStep_(_singleStep),
        pageStep_(_pageStep),
        snap_(_snap)
      {
      }
      
      qreal snapped(value_type _v) const
      {
        return value_type(round(qreal(_v)/singleStep()))*singleStep(); 
      }

      bool snap() const
      {
        return snap_;
      }

      void setSnap(bool _snap)
      {
        snap_=_snap;
      }
      
      value_type singleStep() const
      {
        return singleStep_;
      }

      value_type pageStep() const
      {
        return pageStep_;
      }
        
      void setSingleStep(value_type _singleStep)
      {
        singleStep_ = _singleStep;
      }
        
      void setPageStep(value_type _pageStep)
      {
        pageStep_ = _pageStep;
      }
      
      template<typename MIN, typename MAX, typename F>
      void for_each_step(MIN _min, MAX _max, F f)
      {
        int _step = 0;
        for (value_type i = _min; i <= _max; i += singleStep())
        {
          int _rI = int(std::round(double(i)/singleStep()));

          bool _isPage = int(pageStep()) == 0 ? false :
                         abs(_rI) % int(std::round(double(pageStep())/singleStep())) == 0;
          f(_step,i,_isPage);
          ++_step;
        }
      }

    private:
      value_type singleStep_;
      value_type pageStep_;
      bool snap_;
    };
  }
}

#endif /* SLIM_MIXIN_SNAP_HPP_ */
