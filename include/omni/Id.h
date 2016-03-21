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

    /// Returns true if string is valid
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
