#include <omni/Session.h>

#include <QDataStream>

namespace omni
{
  Session::Session()
  {
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
    mapping_.reset(MappingFactory::create(_id,inputs_));
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
}

QDataStream& operator<<(QDataStream& _stream, omni::Session const& _session)
{
  _stream << _session.tunings();
  _stream << _session.mapping()->getTypeId();
  _session.mapping()->toStream(_stream);
  _stream << _session.inputs();
  _stream << _session.canvas()->getTypeId();
  _session.canvas()->toStream(_stream);
  return _stream;
}

QDataStream& operator>>(QDataStream& _stream, omni::Session& _session)
{
  _stream >> _session.tunings();
  
  omni::Id _mappingId;
  _stream >> _mappingId;
  auto* _mapping = _session.setMapping(_mappingId);
  if (_mapping)
    _mapping->fromStream(_stream);

  _stream >> _session.inputs();

  omni::Id _id;
  _stream >> _id;
  auto* _canvas = _session.setCanvas(_id);
  if (_canvas)
    _canvas->fromStream(_stream);
  return _stream;
}
