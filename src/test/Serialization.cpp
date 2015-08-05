
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
    _inputs.add("TestImage","test_image");
    _inputs.add("Image","image");
    test::Serialization<InputList>("inputs",_inputs);
  }

  /// Test session
  {
    Session _session;
    test::Serialization<Session> _test("Session",_session);
  }
}
