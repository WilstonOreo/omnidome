#include <omni/Session.h>

#include <QDataStream>

#include <omni/util.h>

#include <omni/canvas/HalfDome.h>
#include <omni/canvas/FullSphere.h>
#include <omni/canvas/InflatableDome.h>
#include <omni/canvas/Box.h>
#include <omni/canvas/Planar.h>
#include <omni/input/Image.h>
#include <omni/input/TestImage.h>

#include <omni/mapping/Equirectangular.h>
#include <omni/mapping/CubeMap.h>
#include <omni/mapping/Fisheye.h>
#include <omni/mapping/Planar.h>

#include <omni/proj/FreeSetup.h>
#include <omni/proj/PeripheralSetup.h>


namespace omni
{
  Session::Session()
  { 
    // Register all classes
    
    // Register canvases
    {
      using namespace canvas;
      Factory::reg<HalfDome>();
      Factory::reg<FullSphere>();
      Factory::reg<InflatableDome>();
      Factory::reg<HalfDome>();
      Factory::reg<canvas::Box>();
      Factory::reg<Planar>();
    }
    // END Register canvases

    // Register inputs
    {
      using namespace input;
      Factory::reg<Image>();
      Factory::reg<TestImage>();
    }
    // END Register inputs

    // Register mappings
    {
      using namespace mapping;
      Factory::reg<Equirectangular>();
      Factory::reg<CubeMap>();
      Factory::reg<Fisheye>();
      Factory::reg<Planar>();
    }
    // END Register mappings

    // Register Projector Setups
    {
      using namespace proj;
      SetupFactory::reg<FreeSetup>();
      SetupFactory::reg<PeripheralSetup>();
    }
    // END Register Projector Setups


    // Set default settings
    setCanvas("HalfDome");
    setMapping("Planar");
  }

  Session::~Session()
  {
  }

  proj::TuningList& Session::tunings()
  {
    return tunings_;
  }

  proj::TuningList const& Session::tunings() const
  {
    return tunings_;
  }
 
  Mapping* Session::setMapping(Id const& _id)
  {
    mapping_.reset(MappingFactory::create(_id));
    return mapping();
  }

  Mapping* Session::mapping()
  {
    return mapping_.get();
  }

  Mapping const* Session::mapping() const
  {
    return mapping_.get();
  }

  InputList& Session::inputs() 
  {
    return inputs_;
  }
  
  InputList const& Session::inputs() const 
  {
    return inputs_;
  }

  Canvas* Session::setCanvas(Id const& _id)
  {
    qDebug() << "Hallo";
    for (auto& _canvas : canvas::Factory::classes())
      qDebug() << _canvas.first;

    canvas_.reset(canvas::Factory::create(_id));
    return canvas();
  }

  Canvas* Session::canvas() 
  {
    return canvas_.get();
  }

  Canvas const* Session::canvas() const
  {
    return canvas_.get();
  }

  ScreenSetup& Session::screenSetup()
  {
    return screenSetup_;
  }

  ScreenSetup const& Session::screenSetup() const
  {
    return screenSetup_;
  }

  Session::Mode Session::mode() const
  {
    return mode_;
  }

  void Session::setMode(Mode _mode)
  {
    mode_=_mode;
  }

  bool operator==(Session const& _lhs,Session const& _rhs)
  {
    return 
      OMNI_TEST_MEMBER_EQUAL(tunings_) &&
      OMNI_TEST_PTR_MEMBER_EQUAL(mapping_) &&
      OMNI_TEST_MEMBER_EQUAL(inputs_) &&
      OMNI_TEST_PTR_MEMBER_EQUAL(canvas_);
  }
}

QDataStream& operator<<(QDataStream& _stream, omni::Session const& _session)
{
  using namespace omni::util;

  _stream << _session.tunings();
  
  serializePtr(_stream,_session.mapping());

  _stream << _session.inputs();
  
  serializePtr(_stream,_session.canvas()); 
  return _stream;
}

QDataStream& operator>>(QDataStream& _stream, omni::Session& _session)
{
  using namespace omni::util;
  _stream >> _session.tunings();
  
  deserializePtr(_stream,[&](omni::Id const& _id) -> 
      omni::mapping::Interface* 
  {
    return _session.setMapping(_id); 
  });

  _stream >> _session.inputs();

  deserializePtr(_stream,[&](omni::Id const& _id) -> 
      omni::canvas::Interface*
  {
    return _session.setCanvas(_id);
  });

  return _stream;
}

