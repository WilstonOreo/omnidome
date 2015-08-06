#include <omni/proj/TuningList.h>

#include <omni/util.h>
#include <omni/proj/Tuning.h>

namespace omni
{
  namespace proj
  {
    Tuning* TuningList::add(bool _makeCurrent)
    {
      container_type::emplace_back(new Tuning());
      auto* _tuning = container_type::back().get();
      if (_makeCurrent)
        setCurrentIndex(container_type::size()-1);

      auto* _setup = _tuning->setupProjector("PeripheralSetup");

      return _tuning;
    }

    void TuningList::remove()
    {
      remove(currentIdx_);
    }

    Tuning* TuningList::current()
    {
      return validIndex(currentIdx_) ? container_type::at(currentIdx_).get() : nullptr;
    }
    Tuning const* TuningList::current() const
    {
      return validIndex(currentIdx_) ? container_type::at(currentIdx_).get() : nullptr;
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
      container_type::erase(container_type::begin() + _idx);
    }

    void TuningList::clear()
    {
      container_type::clear();
      currentIdx_ = -1;
    }

    Tuning* TuningList::operator[](int _index)
    {
      return validIndex(_index) ? 
        container_type::at(_index).get() : nullptr;
    }

    Tuning const* TuningList::operator[](int _index) const
    {
      return validIndex(_index) ? 
        container_type::at(_index).get() : nullptr;
    }


    void TuningList::fromStream(QDataStream& _stream)
    {
      clear();
      int _size = 0;
      _stream >> _size;

      int _currentIdx = -1;
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
      _stream << int(container_type::size());
      _stream << currentIndex();
      for (auto& _tuning : *this)
        _stream << *_tuning;
    }

    bool operator==(TuningList const& _lhs, TuningList const& _rhs)
    {
      // Lambda for testing if pointers of two tunings are equal
      auto _tuningsEqual = [](Tuning const* _a, Tuning const* _b) -> bool
      {
        return _a && _b // Test if pointer have same address 
          ? 
          // Derefence pointers and use equality operator to test equality
          ((*_a) == (*_b)) 
          : 
          // Compare pointers and if test of pointer are not nullptrs 
          (_a == _b);
      };

      return 
        OMNI_TEST_MEMBER_EQUAL(currentIdx_) &&
        util::testPtrVectorEqual(_lhs,_rhs,_tuningsEqual);
    }

    bool TuningList::validIndex(int _idx) const
    {
      return (_idx >= 0) && (_idx < container_type::size());
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
