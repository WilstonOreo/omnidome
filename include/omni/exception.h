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
