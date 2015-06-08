#ifndef OMNI_TYPEIDINTERFACE_H_
#define OMNI_TYPEIDINTERFACE_H_

#include <omni/Id.h>

namespace omni
{
  /// Abstract Interface with a single virtual member function which returns type id
  class TypeIdInterface
  {
  public:
    /// Returns type id of object
    virtual Id getTypeId() const = 0;
  };
}

#endif /* OMNI_TYPEIDINTERFACE_H_ */
