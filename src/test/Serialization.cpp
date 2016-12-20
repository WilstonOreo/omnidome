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


#include <QString>
#include <QDataStream>
#include <QFile>
#include <QDebug>
#include <QApplication>
#include <omni/util.h>
#include <omni/visual/Tuning.h>
#include <omni/PluginLoader.h>


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
          } catch (omni::Exception& e)
          {
            qDebug() << e.message();
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

#include <omni/serialization/PropertyMap.h>


int main(int ac, char* av[])
{
  using namespace omni;

  {
      try {

      QBuffer _buf;
      _buf.open(QIODevice::WriteOnly);
      QDataStream _os(&_buf);
      using serialization::PropertyMap;

      PropertyMap _input;
      _input.put("test","test");

      QString _string;
      _input.get("test",_string);

      _os << _input;

      QDataStream _is(_buf.data());

      PropertyMap _output;
      _is>> _output;

      _output.get("test",_string);
      qDebug() << _string;

  } catch (Exception& e) {
      qDebug() << e.message();
  }
  }

  QApplication _app(ac,av);
  PluginLoader::load(PluginLoader::defaultPaths());


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

  /// Test projector
  {
      proj::Projector _proj;
      test::Serialization<Projector>("Projector",_proj);
  }

  /// Test InputList
  {
    InputList _inputs;
    _inputs.addInput("TestImage");
    _inputs.addInput("Image");
    test::Serialization<InputList>("inputs",_inputs);
  }
}
