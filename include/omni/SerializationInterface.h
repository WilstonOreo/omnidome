#ifndef OMNI_SERIALIZATIONINTERFACE_H_
#define OMNI_SERIALIZATIONINTERFACE_H_

#include <omni/TypeIdInterface.h>

namespace omni
{
  /**@brief Abstract interface for serialization
   * @detail Holds two member functions for serialization and deserialization
   */
  class SerializationInterface : public TypeIdInterface
  {
  public:
    /// Serialize to stream
    virtual void toStream(QDataStream&) const = 0;
    
    /// Deserialize from stream
    virtual void fromStream(QDataStream&) = 0;
  };
}

#endif /* OMNI_SERIALIZATIONINTERFACE_H_ */
