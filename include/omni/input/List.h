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
      /**@brief Add new input with id and type id. Returns nullptr if mapping already exists 
        *@param _typeId Type id of input to determine which kind of input is created
        *@param _id Id for input
       **/
      Input* add(Id const& _typeId, Id const& _id);
 
      /**@brief Add new input with id and type id. Returns nullptr if mapping already exists 
         @tparam TYPE Type of input
         @tparam ...ARGS Type of arguments passed to constructor of input of TYPE
        *@param _id Id for input
        *@param ..._args Arguments passed to constructor 
       **/
      template<typename TYPE, typename...ARGS>
      TYPE* add(Id const& _id, ARGS&&..._args)
      {
        if (inputs_.count(_id) != 0) return nullptr;
        
        auto& _input = inputs_[_id];

        _input.reset(new TYPE(_args...));
        return static_cast<TYPE*>(_input.get());
      }

      /// Return input with id 
      Input* operator[](Id const&);
      
      /// Returns input with id (const version)
      Input const* operator[](Id const&) const;

      /// Removes input with custom ID
      void remove(Id const& _id);

      /// Delete all inputs
      void clear();
      
      /// Deserialize from stream
      void fromStream(QDataStream&);

      /// Serialize to stream
      void toStream(QDataStream&) const;

      /**@brief Returns pointer to current input
       * @detail Returns nullptr if currentIdx_ == -1 or input list is empty
       **/
      Input* current();
      
      /**@brief Returns pointer to current input (const version)
       * @detail Returns nullptr if currentIdx_ == -1 or input list is empty
       **/
      Input const* current() const;
      
      /// Return ID of current input
      Id currentId() const; 

      /// Set current input by ID
      void setCurrent(Id const& _id);

      /// Test whether list is empty
      bool empty() const;

      /// Returns number of inputs
      size_t size() const;


      /// Test for equality
      friend bool operator==(List const&,List const&);

    private:
      Id currentId_;
      std::map<Id,std::unique_ptr<Input>> inputs_;
    };
  }

  typedef input::List InputList;
}

QDataStream& operator<<(QDataStream&, omni::input::List const&);
QDataStream& operator>>(QDataStream&, omni::input::List&);


#endif /* OMNI_INPUT_LIST_H */
