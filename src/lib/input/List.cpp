#include <omni/input/List.h>

#include <omni/util.h>

namespace omni
{
  namespace input
  {
    void List::clear()
    {
      inputs_.clear();
    }

    Input* List::add(Id const& _typeId, Id const& _id)
    {
      if (inputs_.count(_id) != 0) return nullptr;

      auto& _input = inputs_[_id];
      _input.reset(Factory::create(_typeId));

      return _input.get();
    }

    Input* List::operator[](Id const& _id)
    {
      if (inputs_.count(_id) == 0) return nullptr;
      return inputs_.at(_id).get();;
    }

    Input const* List::operator[](Id const& _id) const
    {
      if (inputs_.count(_id) == 0) return nullptr;
      return inputs_.at(_id).get();;
    }

    void List::remove(Id const& _id)
    {
      if (inputs_.count(_id) == 0) return;

      inputs_.erase(_id);
      if (_id == currentId_) setCurrent("");
    }

    void List::fromStream(QDataStream& _stream) 
    {
      using namespace omni::util;
      clear();

      _stream >> currentId_;

      // Deserialize map of inputs
      int _size = 0;
      _stream >> _size;
      for (int i = 0; i < _size; ++i)
      {
        Id _inputId;
        _stream >> _inputId;
        if (!_inputId.valid()) continue;
 
        deserializePtr(_stream,[&](omni::Id const& _id) -> 
            input::Interface*
        {
          return add(_id,_inputId);
        });
      }
    }

    void List::toStream(QDataStream& _stream) const
    {
      using namespace omni::util;
      
      _stream << currentId_;

      // serialize map of inputs
      _stream << int(inputs_.size());
      for (auto& _idInput : inputs_)
      {
        _stream << _idInput.first;
        serializePtr(_stream,_idInput.second.get());
      }
    }
      
    Input const* List::current() const
    {
      if (!inputs_.count(currentId_)) return nullptr;

      return inputs_.at(currentId_).get();
    }
    
    Input* List::current()
    {
      if (!inputs_.count(currentId_)) return nullptr;
      
      return inputs_.at(currentId_).get();
    }

    Id List::currentId() const
    {
      return currentId_;
    }

    void List::setCurrent(Id const& _id)
    {
      if (!inputs_.count(_id)) 
      {
        currentId_ = "";
        return;
      }
      currentId_ = _id; 
    }
      
    bool List::empty() const
    {
      return inputs_.empty();
    }

    size_t List::size() const
    {
      return inputs_.size();
    }
 
    bool operator==(List const& _lhs, List const& _rhs)
    {
      if (_lhs.currentId_ != _rhs.currentId_) return false;
      if (_lhs.inputs_.size() != _rhs.inputs_.size()) return false;

      auto it = _lhs.inputs_.begin();
      auto jt = _rhs.inputs_.begin();

      for (; (it != _lhs.inputs_.end()) && (jt != _rhs.inputs_.end()); ++it,++jt)
      {
        if (it->first != jt->first) return false;

        if (!it->second->equal(it->second.get())) return false; 
      }
      return true;
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
