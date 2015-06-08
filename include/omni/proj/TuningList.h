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
    class TuningList
    {
    public:
      /**@brief Add tuning to list
       * @detail Returns pointer new tuning
       * @param _makeCurrent Flag whether to set current index when added successfully
      **/
      Tuning* add(bool _makeCurrent = true);

      void remove();
      void remove(int);

      Tuning* current();
      Tuning const* current() const;
      void setCurrentIndex(int);
      int currentIndex() const;

      bool empty() const;
      int size() const;
      void clear();

      Tuning* operator[](int);
      Tuning const* operator[](int) const;

      void fromStream(QDataStream&);
      void toStream(QDataStream&) const;

    private:
      bool validIndex(int) const;

      int currentIdx_ = -1;
      std::vector<std::unique_ptr<Tuning>> tunings_;
    };
  }
}


QDataStream& operator<<(QDataStream&, omni::proj::TuningList const&);
QDataStream& operator>>(QDataStream&, omni::proj::TuningList&);

#endif /* OMNI_TUNINGLIST_H */
