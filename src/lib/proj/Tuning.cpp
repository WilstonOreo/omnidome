#include <omni/proj/Tuning.h>

#include <omni/proj/Setup.h>
#include <omni/proj/ScreenSetup.h>
#include <QColor>


namespace omni
{
  namespace proj
  {
    Tuning::Tuning() :
      color_("#FFFFFF"),
      blendMask_(*this)
    {
    }

    Tuning::Tuning(const QColor& _color) :
      color_(_color),
      blendMask_(*this)
    {
    }

    void Tuning::setScreen(Screen const* _screen)
    {
      projector_.setScreen(_screen);
    }

    Screen const* Tuning::screen() const
    {
      return projector_.screen();
    }

    Projector const& Tuning::projector() const
    {
      return projector_;
    }

    ProjectorSetup* Tuning::setupProjector(Id const& _setupId)
    {
      projectorSetup_.reset(proj::SetupFactory::create(_setupId));
      return projectorSetup_.get();
    }

    ProjectorSetup* Tuning::projectorSetup()
    {
      return projectorSetup_.get();
    }

    ProjectorSetup const* Tuning::projectorSetup() const
    {
      return projectorSetup_.get();
    }

    WarpGrid& Tuning::warpGrid()
    {
      return warpGrid_;
    }

    WarpGrid const& Tuning::warpGrid() const
    {
      return warpGrid_;
    }

    BlendMask& Tuning::blendMask()
    {
      return blendMask_;
    }

    BlendMask const& Tuning::blendMask() const
    {
      return blendMask_;
    }

    QColor Tuning::color() const
    {
      return color_;
    }

    void Tuning::setColor(QColor const& _color)
    {
      color_=_color;
    }

    bool Tuning::hasScreen() const
    {
      return projector_.screen() != nullptr;
    }

    bool Tuning::resolutionToBeChanged(proj::Screen const* _screen) const
    {
      return false;
    }

    int Tuning::width() const
    {
      return !projector_.screen() ? ScreenSetup::standardScreen().width() : projector_.screen()->width();
    }

    int Tuning::height() const
    {
      return !projector_.screen() ? ScreenSetup::standardScreen().height() : projector_.screen()->width();
    }
  }
}

QDataStream& operator>>(QDataStream& _stream, omni::proj::Tuning& _tuning)
{
  QColor _color;
  _stream >> _color;
  _tuning.setColor(_color);

  omni::Id _id;
  _stream >> _id;
  auto* _projSetup = _tuning.setupProjector(_id);
  _projSetup->fromStream(_stream);
  _stream >> _tuning.warpGrid();
  _stream >> _tuning.blendMask();

  return _stream;
}

QDataStream& operator<<(QDataStream& _stream, omni::proj::Tuning const& _tuning)
{
  _stream << _tuning.color();
  _stream << _tuning.projectorSetup()->getTypeId();
  _tuning.projectorSetup()->toStream(_stream);
  _stream << _tuning.warpGrid();
  _stream << _tuning.blendMask();
  return _stream;
}


