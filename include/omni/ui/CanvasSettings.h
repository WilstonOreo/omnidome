#ifndef OMNI_UI_CANVASSETTINGS_H_
#define OMNI_UI_CANVASSETTINGS_H_

#include <omni/canvas/Interface.h>
#include <omni/ui/ParameterWidget.h>

namespace omni
{
  namespace ui
  {
    class CanvasSettings : public ParameterWidget
    {
      Q_OBJECT
    public:
      CanvasSettings(QWidget* _parent);
      ~CanvasSettings();

      Canvas* canvas();
      Canvas const* canvas() const;
      void setCanvas(omni::canvas::Interface* _canvas);

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


#endif /* OMNI_UI_CANVASSETTINGS_H_ */
