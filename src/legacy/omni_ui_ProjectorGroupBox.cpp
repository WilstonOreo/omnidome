#include <omni/ui/ProjectorGroupBox.h>

#include <QVBoxLayout>
#include <QFocusEvent>

using namespace omni::ui;

ProjectorGroupBox::ProjectorGroupBox(QWidget* _parent) :
  QGroupBox(_parent),
  index_(0)
{
  setCheckable(true);
}

ProjectorGroupBox::ProjectorGroupBox(const Color4f& _color, size_t _index) :
  QGroupBox(nullptr)
{
  setColorAndIndex(_color,_index);
}

void ProjectorGroupBox::setColorAndIndex(const Color4f& _color, size_t _index)
{
  setCheckable(true);
  setTitle(QString("Projector #%1").arg(_index+1));
  index_=_index;
  QString _qColorStr = QColor(_color.r()*255,_color.g()*255,_color.b()*255).name();
  QString s = QString(
                "QGroupBox { border: 2px solid %1;"
                "border-radius: 9px; margin-top: 18px; } "
                "QGroupBox::title { color: %2; "
                " background-color: qradialgradient(cx:0.5, cy:0.5, radius: 0.5,"
                " fx:0.5, fy:0.5, stop:0.2 black, stop:1 transparent) ; subcontrol-origin: margin;"
                "subcontrol-position: top left; padding-left: 10px;"
                "padding-right: 10px; }");

  setStyleSheet(s.arg(_qColorStr).arg(_qColorStr));
}

void ProjectorGroupBox::focusInEvent ( QFocusEvent * event )
{
  emit setFocus(index_);
  QGroupBox::focusInEvent(event);
}
