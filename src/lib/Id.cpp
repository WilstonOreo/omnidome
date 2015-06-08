#include <omni/Id.h>

#include <QString>
#include <QDebug>

namespace omni
{ 
  Id::Id()
  {
  }

  Id::Id(QString const& _str) 
  {
    make(_str);
  }
  
  Id::Id(const char* _c) 
  {
    make(QString(_c));
  }

  QString const& Id::str() const
  {
    return str_;
  }

  Id::operator QString() const
  {
    return str_;
  }

  bool Id::valid() const
  {
    return !str_.isEmpty();
  }
    
  bool Id::operator<(const Id& _rhs) const
  {
    return this->str_ < _rhs.str_;
  }
    
  bool Id::operator!=(const Id& _rhs) const
  {
    return *this != _rhs;
  }

  bool Id::operator==(const Id& _rhs) const
  {
    return this->str_ == _rhs.str_;
  }

  void Id::make(QString const& _str)
  {
    str_ = "";
    // An id must not be empty
    if (_str.isEmpty()) return;

    // An Id must begin with a letter
    if (!_str[0].isLetter()) return;

    // An id must only contain alpha numerical characters 
    // and underscores
    for (auto& c : _str)
    {
      if (!c.isLetterOrNumber() && (!c.isPunct())) return;
    }
    str_=_str;
  }
}

QDataStream& operator<<(QDataStream& _stream, omni::Id const& _id)
{
  _stream << _id.str();
  return _stream;
}

QDataStream& operator>>(QDataStream& _stream, omni::Id& _id)
{
  QString _str;
  _stream >> _str;
  _id = omni::Id(_str);
  return _stream;
}

