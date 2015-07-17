#include <omni/input/List.h>

#include <omni/util.h>
#include <omni/input/TestImage.h>

namespace omni
{
  namespace input
  {
    List::List()
    {
      clear();
    }

    void List::clear()
    {
      container_type::clear();

      // At test image by default
      add<TestImage>();
      setCurrentIndex(0);
    }

    Input* List::add(Id const& _typeId)
    {
      std::unique_ptr<Input> _input(Factory::create(_typeId));
      if (!_input) return nullptr;

      container_type::push_back(std::move(_input));
      return container_type::back().get();
    }

    Input* List::operator[](int _index)
    {
      return validIndex(_index) ? container_type::operator[](_index).get() : nullptr;
    }

    Input const* List::operator[](int _index) const
    {
      return validIndex(_index) ? 
        container_type::operator[](_index).get() : 
        nullptr;
    }

    void List::remove(int _index)
    {
      if (!validIndex(_index) || _index == 0) return;

      container_type::erase(this->begin() + _index);
    }

    void List::fromStream(QDataStream& _stream) 
    {
      using namespace omni::util;
      clear();

      _stream >> currentIndex_;

      // Deserialize map of inputs
      int _size = 0;
      _stream >> _size;
      for (int i = 0; i < _size; ++i)
      {
        deserializePtr(_stream,[&](omni::Id const& _id) -> 
            input::Interface*
        {
          return add(_id);
        });
      }
      setCurrentIndex(currentIndex_);
    }

    void List::toStream(QDataStream& _stream) const
    {
      using namespace omni::util;
      
      _stream << currentIndex_;

      // serialize map of inputs
      _stream << int(size());
      for (auto& _input : (*this))
      {
        serializePtr(_stream,_input.get());
      }
    }
      
    Input const* List::current() const
    {
      return container_type::at(currentIndex_).get();
    }
    
    Input* List::current()
    {
      return container_type::at(currentIndex_).get();
    }

    int List::currentIndex() const
    {
      return currentIndex_;
    }

    void List::setCurrentIndex(int _index)
    {
      if (!validIndex(_index)) return;
      
      currentIndex_ = _index;
    }
      
 
    bool operator==(List const& _lhs, List const& _rhs)
    {
      if (_lhs.currentIndex_ != _rhs.currentIndex_) return false;
      if (_lhs.size() != _rhs.size()) return false;

      auto it = _lhs.begin();
      auto jt = _rhs.begin();

      for (; (it != _lhs.end()) && (jt != _rhs.end()); ++it,++jt)
      {
        if (!(*it)->equal(jt->get())) return false;
      }
      return true;
    }
 
    bool List::validIndex(int _index) const
    {
      return (_index >= 0) && (_index < this->size()); 
    }
  }
}

QDataStream& operator<<(QDataStream& _stream, omni::input::List const& _list)
{
  _list.toStream(_stream);
  return _stream;
}

QDataStream& operator>>(QDataStream& _stream, omni::input::List& _list)
{
  _list.fromStream(_stream);
  return _stream;
}

