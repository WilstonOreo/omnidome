#include <omni/Session.h>


namespace omni
{
  Session::Session()
  {
  }

  Session::~Session()
  {
  }

  MappingList& Session::mappings()
  {
    return mappings_;
  }

  MappingList const& Session::mappings() const
  {
    return mappings_;
  }
}
