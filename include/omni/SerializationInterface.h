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

    /// Optional virtual member method for testing equality of two interfaces 
    inline virtual bool equal(SerializationInterface const* _that) const
    {
      return this->getTypeId() == _that->getTypeId();
    }
  };
}

#define OMNI_DECL_STREAM_OPERATORS(CLASS)\
  inline QDataStream& operator>>(QDataStream& _stream, CLASS& _cls)\
  {\
    _cls.fromStream(_stream);\
    return _stream;\
  }\
  inline QDataStream& operator<<(QDataStream& _stream, CLASS const& _cls)\
  {\
    _cls.toStream(_stream);\
    return _stream;\
  }
  

#endif /* OMNI_SERIALIZATIONINTERFACE_H_ */
