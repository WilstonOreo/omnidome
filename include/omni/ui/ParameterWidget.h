#ifndef OMNI_UI_PARAMETERWIDGET_H_
#define OMNI_UI_PARAMETERWIDGET_H_

#include <set>
#include <QWidget>
#include <slim/RangedFloat.h>
#include <slim/Rotation.h>
#include <omni/util.h>

namespace omni
{
  namespace ui
  {
    /**@brief A widget which holds sub-widgets for manipulating parameters
    **/
    class ParameterWidget : public QWidget
    {
      Q_OBJECT
    public:
      /// Default constructor 
      ParameterWidget(QWidget* _parent = nullptr);
        
      /// Default Constructor  
      ~ParameterWidget();

      /// Adds a widget with a name, a value and min-max range
      slim::RangedFloat* addWidget(QString const&,float,float,float);
        
      /// Adds an angle widget with a name, a value and min-max range
      slim::RangedFloat* addAngleWidget(QString const&,float,float,float);
        
      /// Adds an offset widget with a name, a value and min-max range
      slim::RangedFloat* addOffsetWidget(QString const&,float,float,float);

      /// Adds an Rotation widget with three dials for X,Y and Z axis
      slim::Rotation* addRotationWidget(QString const&);

    public slots:
      /// Removes all parameter widgets
      void clear();

      virtual void updateParameters();
      
      /// Utility method for retrieving the value from a slider with id
      double getParamAsFloat(QString const&) const;

      /// Get parameter widget from parameter map with Id
      QWidget* getWidget(QString const&);

    signals:
      void parametersUpdated();

    protected: 

      template<typename F>
      void locked(F f) 
      {
        bool _oldLocked = locked_;
        locked_ = true;
        f();
        locked_ = _oldLocked;
      }

      inline bool isLocked() const 
      {
        return locked_;
      }

      /// Slider parameter widgets
      std::vector<QUniquePtr<QWidget>> parameters_;
        
      /// Slider widgets, sorted by their label
      std::map<QString,QWidget*> parameterMap_;

    private:
      /// Flag which sets if widgets are currently locked
      bool locked_ = false;
    };
  }
}

#endif /* OMNI_UI_PARAMETERWIDGET_H_ */
