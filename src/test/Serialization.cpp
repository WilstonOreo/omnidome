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


#include <QString>
#include <QDataStream>
#include <QFile>
#include <QDebug>
#include <omni/util.h>

namespace omni
{
  namespace test
  {
    template<typename T>
    struct Serialization
    {
      typedef T obj_type;

      template<typename...ARGS>
      Serialization(QString _filename, 
          obj_type const& _serialized, 
          ARGS&&..._args) : 
        filename_(QString("src/test/data/")+_filename+".omni"),
        serialized_(_serialized),
        deserialized_(_args...)
      {
        if (test())
        {
          qDebug() << "Test successful: " << _filename;
        } else
        {
          qDebug() << "Test failed: " << _filename;
        }
      }

      obj_type const& deserialized() const
      {
        return deserialized_;
      }

    private:
      bool test()
      {
        QFile _outfile(filename_);
        if (_outfile.open(QIODevice::WriteOnly))
        { 
          QDataStream _out(&_outfile);
          _out << serialized_;
        }
        _outfile.close();

        QFile _infile(filename_);
        if (_infile.open(QIODevice::ReadOnly))
        {
          QDataStream _in(&_infile);
          try
          {
            _in >> deserialized_;
          } catch (omni::exception::Serialization& e)
          {
            qDebug() << e.what();
            return false;
          }
        }
        _infile.close();

        return serialized_ == deserialized_;
      }

      QString filename_;
      obj_type const& serialized_;
      obj_type deserialized_;
    };
  }
}


#include <omni/Session.h>
#include <omni/WarpGrid.h>
#include <omni/BlendMask.h>
#include <omni/proj/Tuning.h>
#include <omni/input/List.h>


int main(int ac, char* av[])
{
  using namespace omni;

  Session _session;

  /// Test WarpPoint
  {
    WarpPoint _pointA(QPointF(10.0,10.0));
    test::Serialization<WarpPoint>("WarpPoint",_pointA);
  }

  /// Test warp grid
  {
    WarpGrid _grid;
    test::Serialization<WarpGrid>("WarpGrid",_grid);
  }

  /// Test Tuning
  {
    proj::Tuning _tuning;
    test::Serialization<proj::Tuning>("proj_Tuning",_tuning);
  }

  /// Test TuningList 
  {
    proj::TuningList _tuningList;
    _tuningList.add();
    _tuningList.add();
    test::Serialization<proj::TuningList>("proj_TuningList",_tuningList);
  }

  /// Test InputList
  {
    InputList _inputs;
    _inputs.add("TestImage");
    _inputs.add("Image");
    test::Serialization<InputList>("inputs",_inputs);
  }

  /// Test session
  {
    Session _session;
    test::Serialization<Session> _test("Session",_session);
  }
}
