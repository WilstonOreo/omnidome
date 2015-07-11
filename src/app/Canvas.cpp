#include <omni/ui/Canvas.h>

#include "ui_omni_ui_Canvas.h"

#include <QVBoxLayout>

#include <omni/Session.h>
#include <omni/canvas/Interface.h>

namespace omni
{
  namespace ui
  {
    Canvas::Canvas(QWidget* _parent) :
      QGroupBox(_parent),
      ui_(new Ui::Canvas)
    {
      ui_->setupUi(this);
 
      for (auto& _idCanvasClass : omni::canvas::Factory::classes())
      {
        QString _id = _idCanvasClass.first.str();
        ui_->boxCanvasSelect->addItem(QIcon(QString(":/canvas/")+ _id + QString(".png")),_id);
      }
      
      QLayout* _layout = new QVBoxLayout;

      _layout->setSpacing(0);

      ui_->widget->setLayout(new QVBoxLayout);
    }

    Canvas::~Canvas()
    {
    }

    Session const* Canvas::session() const
    {
      return session_;
    }

    void Canvas::setSession(Session* _session)
    {
      session_=_session;
    }
      
    void Canvas::canvasTypeSelected(QString const& _id)
    {
      if (!session_) return;
        
      ui_->widget->clear();
        
      session_->setCanvas(_id); 

      if (!session_->canvas()) return;

      if (_id == "HalfDome") 
      {
        ui_->widget->addOffsetWidget("Radius",0.0,0.1,10.0);
      } else
      if (_id == "Radome") 
      {
        ui_->widget->addOffsetWidget("Radius",0.0,0.1,10.0);
        ui_->widget->addOffsetWidget("Z Offset",0.0,0.1,10.0);
      } else
      if (_id == "InflatableDome")
      {
        ui_->widget->addOffsetWidget("Radius",0.0,0.1,10.0);
        ui_->widget->addOffsetWidget("Z Offset",0.0,0.1,10.0);
      } else
      if (_id == "Planar")
      {
        ui_->widget->addOffsetWidget("Height",1.0,0.1,10.0);
        ui_->widget->addOffsetWidget("Width",1.0,0.1,10.0);
        ui_->widget->addOffsetWidget("X",1.0,-10.0,10.0);
        ui_->widget->addOffsetWidget("Y",1.0,-10.0,10.0);
        ui_->widget->addOffsetWidget("Z",1.0,-10.0,10.0);
        ui_->widget->addAngleWidget("Yaw",0.0,0.0,360.0);
        ui_->widget->addAngleWidget("Pitch",0.0,0.0,360.0);
        ui_->widget->addAngleWidget("Roll",0.0,0.0,360.0);
      } else if (_id == "Box") 
      {
        ui_->widget->addOffsetWidget("Height",1.0,0.1,10.0);
        ui_->widget->addOffsetWidget("Width",1.0,0.1,10.0);
        ui_->widget->addOffsetWidget("Length",1.0,0.1,10.0);
      }  

      emit canvasTypeChanged();
    }    
  }
}
