#ifndef OMNI_UI_PARAMETERWIDGET_H_
#define OMNI_UI_PARAMETERWIDGET_H_

#include <set>
#include <QWidget>
#include <slim/RangedFloat.h>
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

    public slots:
      /// Removes all parameter widgets
      void clear();

      virtual void updateParameters();

    signals:
      void parametersUpdated();

    protected: 
      /// Slider parameter widgets
      std::vector<QUniquePtr<QWidget>> parameters_;
        
      /// Slider widgets, sorted by their label
      std::map<QString,QWidget*> parameterMap_;
    };
  }
}

#endif /* OMNI_UI_PARAMETERWIDGET_H_ */
