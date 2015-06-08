#include <omni/input/List.h>

namespace omni
{
  namespace input
  {
    void List::clear()
    {
      inputs_.clear();
    }

    Input* List::add(Id const& _id, Id const& _typeId)
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
    }

    void List::fromStream(QDataStream& _stream) 
    {
      clear();

      // Deserialize map of inputs
      int _size = 0;
      _stream >> _size;
      for (int i = 0; i < _size; ++i)
      {
        Id _id, _typeId;
        _stream >> _id >> _typeId;
        if (!_id.valid() || !_typeId.valid()) continue;

        auto* _input = add(_id,_typeId);
        _input->fromStream(_stream);
      }
    }

    void List::toStream(QDataStream& _stream) const
    {
      // serialize map of inputs
      _stream << int(inputs_.size());
      for (auto& _idInput : inputs_)
      {
        auto& _id = _idInput.first;
        auto& _input = _idInput.second;
        auto _typeId = _input->getTypeId();

        _stream << _id << _typeId;
        _input->toStream(_stream);
      }
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
