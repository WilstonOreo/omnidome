#ifndef OMNI_UI_PROJECTORGROUPBOX_H_
#define OMNI_UI_PROJECTORGROUPBOX_H_

#include <QGroupBox>
#include <omni/core.hpp>

namespace omni
{  
  namespace ui
  {
    class ProjectorGroupBox : public QGroupBox
    {
      Q_OBJECT
    public:
      ProjectorGroupBox(QWidget* _parent = nullptr);
      ProjectorGroupBox(const Color4f& _color,
          size_t _index);

      void setColorAndIndex(const Color4f& _color, size_t _index);
    protected:
      void focusInEvent ( QFocusEvent * event );
    signals:
      void setFocus(int);

    private:
      int index_;
    };
  }
}

#endif /* OMNI_UI_PROJECTORGROUPBOX_H_ */
