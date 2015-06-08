#include <omni/proj/TuningList.h>

#include <omni/proj/Tuning.h>

namespace omni
{
  namespace proj
  {
    Tuning* TuningList::add(bool _makeCurrent)
    {
      tunings_.emplace_back(new Tuning());
      auto* _tuning = tunings_.back().get();
      if (_tuning && _makeCurrent)
        setCurrentIndex(tunings_.size()-1);

      auto* _setup = _tuning->setupProjector("PeripheralSetup");

      return _tuning;
    }

    void TuningList::remove()
    {
      remove(currentIdx_);
    }

    Tuning* TuningList::current()
    {
      return validIndex(currentIdx_) ? tunings_[currentIdx_].get() : nullptr;
    }
    Tuning const* TuningList::current() const
    {
      return validIndex(currentIdx_) ? tunings_[currentIdx_].get() : nullptr;
    }

    void TuningList::setCurrentIndex(int _currentIdx)
    {
      if (_currentIdx < 0)
      {
        currentIdx_ = 0;
        return;
      }
      if (_currentIdx >= size())
      {
        currentIdx_ = size() - 1;
        return;
      }

      currentIdx_ = _currentIdx;
    }

    int TuningList::currentIndex() const
    {
      return currentIdx_;
    }

    void TuningList::remove(int _idx)
    {
      if (!validIndex(_idx)) return;
      tunings_.erase(tunings_.begin() + _idx);
    }

    bool TuningList::empty() const
    {
      return tunings_.empty();
    }

    int TuningList::size() const
    {
      return tunings_.size();
    }

    void TuningList::clear()
    {
      tunings_.clear();
    }

    Tuning* TuningList::operator[](int _index)
    {
      return validIndex(_index) ? tunings_[_index].get() : nullptr;
    }

    Tuning const* TuningList::operator[](int _index) const
    {
      return validIndex(_index) ? tunings_[_index].get() : nullptr;
    }


    void TuningList::fromStream(QDataStream& _stream)
    {
      clear();
      int _size = 0;
      _stream >> _size;

      int _currentIdx = 0;
      _stream >> _currentIdx;

      for (int i = 0; i < _size; ++i)
      {
        auto* _tuning = add(false /* Dont modify current tuning index */);
        if (!_tuning) continue;
        _stream >> *_tuning;
      }
      setCurrentIndex(_currentIdx);
    }

    void TuningList::toStream(QDataStream& _stream) const
    {
      _stream << size();
      _stream << currentIndex();
      for (auto& _tuning : tunings_)
        _stream << *_tuning;
    }

    bool TuningList::validIndex(int _idx) const
    {
      return (_idx >= 0) && (_idx < tunings_.size());
    }
  }
}

QDataStream& operator<<(QDataStream& _stream, omni::proj::TuningList const& _list)
{
  _list.toStream(_stream);
  return _stream;
}

QDataStream& operator>>(QDataStream& _stream, omni::proj::TuningList& _list)
{
  _list.fromStream(_stream);
  return _stream;
}
