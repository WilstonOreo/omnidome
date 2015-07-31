#ifndef OMNI_UI_CANVASPARAMETERS_H_
#define OMNI_UI_CANVASPARAMETERS_H_

#include <omni/canvas/Interface.h>
#include <omni/ui/ParameterWidget.h>

namespace omni
{
  namespace ui
  {
    /**@brief Parameter widget for getting and setting canvas parameters 
     **/
    class CanvasParameters : public ParameterWidget
    {
      Q_OBJECT
    public:
      CanvasParameters(QWidget* _parent);
      ~CanvasParameters();

      canvas::Interface* canvas();
      canvas::Interface const* canvas() const;
      void setCanvas(canvas::Interface* _canvas);

      /// Set parameters from sliders to canvas
      void updateParameters();
    private:
      slim::Rotation* rotation_ = nullptr;

      /// Show widgets parameter according to given canvas type
      void widgetVisibility();
      
      /// Clear and setup all parameter widgets
      void setup();

      canvas::Interface* canvas_ = nullptr;
    };
  }
}


#endif /* OMNI_UI_CANVASPARAMETERS_H_ */
