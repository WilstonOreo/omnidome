#ifndef OMNI_UI_PROJECTORWIDGET_H_
#define OMNI_UI_PROJECTORWIDGET_H_

#include "GLView.h"
#include <QUndoCommand>

namespace omni 
{
  class Mapping;

  namespace ui
  {
    namespace Ui
    {
      class ProjectorWidget;
    }

    class ProjectorWidget : public QWidget 
    {
      Q_OBJECT
    public:
      typedef std::set<GLView*> view_set_type;

      ProjectorWidget(Mapping* _mapping, const view_set_type& _views);
      ~ProjectorWidget();
      void getSettings();
      
      proj::Projector const* proj() const;
      proj::Projector * proj();

      TBD_PROPERTY_RO(Mapping*,mapping)
    signals:
      void command(QUndoCommand*); 

    public slots:
      void toggleSettings();
      void updateSettings(double);
      void projectorVisible(bool);

    private:
      bool updateMode_;
      view_set_type views_; 
      std::unique_ptr<Ui::ProjectorWidget> ui_;
    };
  }
}

#endif /* OMNI_UI_PROJECTORWIDGET_H_ */
