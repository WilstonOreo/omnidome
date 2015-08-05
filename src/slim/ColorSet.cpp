#include <slim/ColorSet.h>

using namespace slim;

ColorSet::ColorSet(QWidget* _parent) :
  parent_(_parent)
{
}

QColor ColorSet::color(QPalette::ColorRole _role) const
{
  return parent_->palette().color(colorGroup(),_role);
}

QPalette::ColorGroup ColorSet::colorGroup() const
{
  return parent_->isEnabled() ?
         (parent_->hasFocus() ? QPalette::Active : QPalette::Inactive) :
           QPalette::Disabled;
}

#define COLORSET_COLOR_GETTER(name,colorrole)\
  QColor ColorSet::name() const\
  {\
    return this->color(colorrole);\
  }

COLORSET_COLOR_GETTER(window,QPalette::Window)
COLORSET_COLOR_GETTER(windowText,QPalette::WindowText)
COLORSET_COLOR_GETTER(button,QPalette::Button)
COLORSET_COLOR_GETTER(toolTipBase,QPalette::ToolTipBase)
COLORSET_COLOR_GETTER(toolTipText,QPalette::ToolTipText)
COLORSET_COLOR_GETTER(text,QPalette::Text)
COLORSET_COLOR_GETTER(buttonText,QPalette::ButtonText)
COLORSET_COLOR_GETTER(brightText,QPalette::BrightText)
COLORSET_COLOR_GETTER(light,QPalette::Light)
COLORSET_COLOR_GETTER(midlight,QPalette::Midlight)
COLORSET_COLOR_GETTER(dark,QPalette::Dark)
COLORSET_COLOR_GETTER(shadow,QPalette::Shadow)
COLORSET_COLOR_GETTER(highlight,QPalette::Highlight)
COLORSET_COLOR_GETTER(highlightedText,QPalette::HighlightedText)
COLORSET_COLOR_GETTER(link,QPalette::Link)
COLORSET_COLOR_GETTER(linkVisited,QPalette::LinkVisited)

QColor ColorSet::base(bool _alternate) const
{
  return _alternate ? 
    color(QPalette::AlternateBase) : 
    color(QPalette::Base);
}

