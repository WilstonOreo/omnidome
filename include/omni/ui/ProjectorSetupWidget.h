#ifndef OMNI_UI_PROJECTORSETUPWIDDGET_H_
#define OMNI_UI_PROJECTORSETUPWIDDGET_H_

#include <omni/gl.hpp>
#include <set>

#include <omni/proj/Screen.hpp>
#include "MappingGL.h"
#include "GLView.h"

namespace omni
{
  class Mapping;
  class Session;

  namespace ui
  {
    namespace Ui
    {
      class ProjectorSetupWidget;
    }

    class ProjectorSetupWidget : public QWidget
    {
      Q_OBJECT
    public:
      typedef std::set<GLView*> view_set_type;

      ProjectorSetupWidget(
          Session* _session,
          int _mapId, 
          const view_set_type& _views);
      ~ProjectorSetupWidget();

      void getSettings();
      void updateScreens();
      TBD_PROPERTY_REF_RO(int,mapId)
    
    signals:
      void screenChanged();
      void command(QUndoCommand*);

    public slots:
      void toggleSettings(bool);
      
      void fovChanged(double);
      void throwRatioChanged(double);
      void screenChanged(int);
      void subScreenChanged(int);
      void resolutionChanged();

    private:
      void getScreen();
      Session* session_;
      bool updateMode_;
      Mapping* mapping();
      view_set_type views_;
      int lastScreenIdx_;
      std::unique_ptr<Ui::ProjectorSetupWidget> ui_;
    };
  }
}

#endif /* OMNI_UI_PROJECTORSETUPWIDDGET_H_ */
