#include <omni/ui/Mapping.h>

#include "ui_omni_ui_Mapping.h"


#include <QVBoxLayout>

#include <omni/Session.h>
#include <omni/canvas/Interface.h>



namespace omni
{
  namespace ui
  {
    Mapping::Mapping(QWidget* _parent) :
      QGroupBox(_parent),
      ui_(new Ui::Mapping)
    {
      ui_->setupUi(this);
    
      for (auto& _idMappingClass : omni::mapping::Factory::classes())
      {
        QString _id = _idMappingClass.first.str();
        ui_->boxMappingSelect->addItem(QIcon(QString(":/mapping/")+ _id + QString(".png")),_id);
      }

      ui_->widget->setLayout(new QVBoxLayout);
    
    }

    Mapping::~Mapping()
    {
    }

    Session const* Mapping::session() const
    {
      return session_;
    }

    void Mapping::setSession(Session* _session)
    {
      session_=_session;
    }
    
    void Mapping::mappingTypeSelected(QString const& _id)
    {
      if (!session_) return;
        
      ui_->widget->clear();
        
      session_->setMapping(_id); 

      if (!session_->mapping()) return;
      
      if (_id == "Fisheye")
      {
        ui_->widget->addWidget("Stretch",0.0,0.0,1.0);
      } else if (_id == "Equirectangular") 
      {
        ui_->widget->addWidget("Strip Bottom",0.0,0.0,1.0);
        ui_->widget->addWidget("Strip Top",0.0,0.0,1.0);
      } else if (_id == "Planar") 
      {
        ui_->widget->addWidget("Stretch X",1.0,0.0,10.0);
        ui_->widget->addWidget("Stretch Y",1.0,0.0,10.0);
        ui_->widget->addWidget("Offset X",0.0,0.0,1.0);
        ui_->widget->addWidget("Offset Y",0.0,0.0,1.0);
        // setTileHorizontal + setTileVertical
      } else if (_id == "Cylindrical")
      {
        ui_->widget->addWidget("Height",1.0,0.1,10.0);
        ui_->widget->addWidget("Radius",0.5,0.1,1.0);
      } else if (_id == "CubeMapSingle")
      {
      } else if (_id == "CubeMapSeparate")
      {
      }
    }
  }
}
