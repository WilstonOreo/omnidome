#ifndef OMNI_UTIL_H_
#define OMNI_UTIL_H_

#include <memory>
#include <QFile>
#include <QByteArray>
#include <QString>

namespace omni
{
  namespace util
  {
    /// Pointer deleter functor
    struct QtDeleter
    {
      template<typename QOBJECT>
      void operator()(QOBJECT* _obj)
      {
        if (!_obj->parent())
          delete _obj;
      }
    };

    /// QUniquePtr for QObjects
    template<typename T>
    using QUniquePtr = std::unique_ptr<T,QtDeleter>;
  
    /// Linear interpolation between two values
    template<typename T, typename A>
    T mix(const T& _x, const T& _y, A _a)
    {
      return _x*(1.0-_a) + _y * _a;
    } 

    /// Return content of file from a file name
    inline static QString fileToStr(const QString& _filename)
    {
      QFile _f(_filename);
      _f.open(QIODevice::ReadOnly|QIODevice::Text);
      return _f.readAll();
    }
  }

  using util::mix;
  using util::QUniquePtr;
}

#endif /* OMNI_UTIL_H_ */
