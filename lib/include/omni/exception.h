/* Copyright (c) 2014-2015 "Omnidome" by Michael Winkelmann
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
#include <omni/Factory.h>

namespace omni {
  /// Base class for all exception. Throws a message with QString type
  class OMNI_EXPORT Exception : public QException {
    public:
      enum Type {
        EX_WARNING,
        EX_ERROR,
        EX_FATAL
      };

      /// This is the method which throw the message string
      virtual QString message() const throw() = 0;

      /// Return type of exception
      virtual Type    type() const = 0;

      inline virtual QString typeAsString() const {
        switch (type()) {
        case EX_WARNING: return "WARNING";

        case EX_ERROR: return "ERROR";

        case EX_FATAL: return "FATAL";
        }
        return "INVALID";
      }
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
          return EX_WARNING;
        }
    };

    class Error : public Exception {
      public:
        inline Type type() const {
          return EX_ERROR;
        }
    };

    class Fatal : public Exception {
      public:
        inline Type type() const {
          return EX_FATAL;
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
          return QString("Serialization Exception. Invalid Id: ") + id_;
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
