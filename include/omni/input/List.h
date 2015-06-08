#ifndef OMNI_INPUT_LIST_H
#define OMNI_INPUT_LIST_H

#include <memory>
#include <map>
#include <omni/Id.h>
#include <omni/input/Interface.h>

namespace omni
{
  namespace input
  {
    /**@brief Mapping List contains a map of mappings (Id -> Mapping)
      *@detail Mapping List is serializable via QDataStream
     **/
    class List
    {
    public:
      void clear();
      
      /**@brief Add new input with id and type id. Returns nullptr if mapping already exists 
        *@param _id Id for input
        *@param _typeId Type id of input to determine which kind of input is created
       **/
      Input* add(Id const& _id, Id const& _typeId);

      /// Return input with id 
      Input* operator[](Id const&);
      
      /// Returns input with id (const version)
      Input const* operator[](Id const&) const;

      void remove(Id const& _id);

      void fromStream(QDataStream&);
      void toStream(QDataStream&) const;


    private:
      std::map<Id,std::unique_ptr<Input>> inputs_;
    };
  }

  typedef input::List InputList;
}

QDataStream& operator<<(QDataStream&, omni::input::List const&);
QDataStream& operator>>(QDataStream&, omni::input::List&);


#endif /* OMNI_INPUT_LIST_H */
