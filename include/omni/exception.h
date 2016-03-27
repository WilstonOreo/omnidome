/* Copyright (c) 2014-2015 "Omnidome" by cr8tr
 * Dome Mapping Projection Software (http://omnido.me).
 * Omnidome was created by Michael Winkelmann aka Wilston Oreo (@WilstonOreo)
 *
 * This file is part of Omnidome.
 *
 * Omnidome is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef OMNI_EXCEPTION_H_
#define OMNI_EXCEPTION_H_

#include <QException>
#include <QString>
#include <omni/Id.h>

namespace omni {
  /// Base class for all exception. Throws a message with QString type
  class Exception : public QException {
    public:
      enum Type {
        WARNING,
        ERROR,
        FATAL
      };

      /// This is the method which throw the message string
      virtual QString message() const throw() = 0;

      /// Return type of exception
      virtual Type    type() const = 0;
  };

#define OMNI_EXCEPTION(EXCEPTION)     \
  inline virtual void raise() const { \
    throw *this;                      \
  }                                   \
  inline QException *clone() const {  \
    return new EXCEPTION(*this);      \
  }

  namespace exception {
    class Warning : public Exception {
      public:
        inline Type type() const {
          return WARNING;
        }
    };

    class Error : public Exception {
      public:
        inline Type type() const {
          return ERROR;
        }
    };

    class Fatal : public Exception {
      public:
        inline Type type() const {
          return FATAL;
        }
    };

    /// An exception that occurs during Serialization
    class Serialization : public Error {
      public:
        OMNI_EXCEPTION(Serialization)

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

  using exception::Error;
  using exception::Warning;
  using exception::Fatal;
}

#endif /* OMNI_EXCEPTION_H_ */
