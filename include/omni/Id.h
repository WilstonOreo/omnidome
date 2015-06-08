#ifndef OMNI_ID_H_
#define OMNI_ID_H_

#include <set>
#include <QString>
#include <QDataStream>
#include <boostx/type_info.hpp>
#include <boostx/factory.hpp>

namespace omni
{

  /**@brief Id type for classes
   * @detail An Id must only contain alpha numerical characters 
   *         and must begin with a letter
  **/
  struct Id
  {
    /// Default constructor
    Id();
    
    /// Constructor from string
    Id(QString const&);

    /// Constructor from const char*
    Id(const char*);

    /// Returns string representation (const)
    QString const& str() const;

    /// Cast operator returns copy of string representation
    operator QString() const;
    
    /// Returns true if string is not valid
    bool valid() const;

    /// Comparison operator 
    bool operator<(const Id&) const;
    
    /// Returns true if two ids do not match
    bool operator!=(const Id&) const;
    
    /// Returns true if two ids match
    bool operator==(const Id&) const;

  private:
    /// Make Id from string
    void make(QString const&);
    
    /// Internal string buffer
    QString str_;
  };

  BOOSTX_DECL_TYPE_INFO(
      TypeInfo,
      Id,
      typeId,
      getTypeId
  )

  /// Alias template for factories
  template<typename INTERFACE, typename...ARGS>
  using AbstractFactory = boostx::factory<INTERFACE,TypeInfo,ARGS...>;

  /// Id set typedef
  typedef std::set<Id> IdSet;

}

#define OMNI_TYPEID(type_id)\
  BOOSTX_TYPE_ID(omni::Id,type_id,typeId,getTypeId)  

namespace std 
{
  /// Template specialization for Id to make it hashable
  template<> 
  struct hash<omni::Id>
  {
    /// Use hash<std::string> to produce hash value
    inline size_t operator()(const omni::Id& _id) const
    {
      return hash<std::string>()(_id.str().toStdString());
    }
  };
}


/// Serialize omni::Id to stream
QDataStream& operator<<(QDataStream&, omni::Id const&);

/// Deserialize omni::Id from stream 
QDataStream& operator>>(QDataStream&, omni::Id&);

#endif /* OMNI_ID_H_ */


