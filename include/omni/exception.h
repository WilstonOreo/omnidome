#ifndef OMNI_EXCEPTION_H_
#define OMNI_EXCEPTION_H_

#include <exception>
#include <string>
#include <QString>
#include <omni/Id.h>

namespace omni
{
  namespace exception
  {
    /// Base class for all exception. Throws a message with QString type 
    class Exception : public std::exception
    {
    public:
      virtual const char* what() const throw()
      {
        return message().toStdString().c_str();
      }

      /// This is the method which throw the message string
      virtual QString message() const throw() = 0;
    };

    /// An exception that occurs during Serialization 
    class Serialization: public Exception
    {
    public:
      Serialization(omni::Id _id) :
        id_(_id)
      {}
      
      QString message() const throw()
      {
        return QString("Serialization Exception. Invalid Id: ") + id_.str(); 
      }

    private:
      omni::Id id_;
    };
  }
}

#endif /* OMNI_EXCEPTION_H_ */
