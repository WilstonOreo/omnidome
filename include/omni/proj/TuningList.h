#ifndef OMNI_TUNINGLIST_H
#define OMNI_TUNINGLIST_H

#include <memory>
#include <vector>
#include <omni/proj/Tuning.h>

namespace omni
{
  namespace proj
  {
    /**@brief Tuning List contains a list of tunings
      *@detail Tuning List is serializable via QDataStream
     **/
    class TuningList : private std::vector<std::unique_ptr<Tuning>>
    {
    public:
      typedef std::vector<std::unique_ptr<Tuning>> container_type;
      
      
      using container_type::size;
      using container_type::empty;
      using container_type::begin;
      using container_type::end;
      
      /**@brief Add tuning to list
       * @detail Returns pointer new tuning
       * @param _makeCurrent Flag whether to set current index when added successfully
      **/
      Tuning* add(bool _makeCurrent = true);

      /// Remove tuning with current index
      void remove();

      /// Remove tuning with custom index
      void remove(int);

      /**@brief Returns pointer to current tuning
       * @detail Returns nullptr if currentIdx_ == -1 or tuning list is empty
       **/
      Tuning* current();
      
      /**@brief Returns pointer to current tuning (const version)
       * @detail Returns nullptr if currentIdx_ == -1 or tuning list is empty
       **/
      Tuning const* current() const;

      /// Set new current index, must be between 0 and size()-1
      void setCurrentIndex(int);

      /// Return current index
      int currentIndex() const;

      /// Deletes all tunings
      void clear();

      /// Returns tuning at a specific index
      Tuning* operator[](int);
      
      /// Returns tuning at a specific index (const version)
      Tuning const* operator[](int) const;

      /// Deserialize list from stream
      void fromStream(QDataStream&);

      /// Serialize list to stream
      void toStream(QDataStream&) const;
      
      /// Test both lists for equality
      friend bool operator==(TuningList const&,TuningList const&);

    private:
      
      /// Test of index is between 0 and size()-1
      bool validIndex(int) const;

      int currentIdx_ = -1;
      std::vector<std::unique_ptr<Tuning>> tunings_;
    };
  }
}


QDataStream& operator<<(QDataStream&, omni::proj::TuningList const&);
QDataStream& operator>>(QDataStream&, omni::proj::TuningList&);

#endif /* OMNI_TUNINGLIST_H */
