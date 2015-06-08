#include <omni/ui/proj/Tuning.h>

#include <vector>

#include <QDebug>
#include <QResizeEvent>
#include <QPaintEvent>
#include <QPainter>

#include <omni/ui/proj/TitleBar.h>
#include <omni/ui/GLView2D.h>
#include <omni/proj/FreeSetup.h>
#include <omni/proj/PeripheralSetup.h>

namespace omni
{
  namespace ui
  {
    namespace proj
    {
      Tuning::Tuning(
        omni::proj::Tuning* _tuning,
        QWidget* _parent) :
        QWidget(_parent)
      {
        setup();
        setTuning(_tuning);
      }

      Tuning::Tuning(
        QWidget* _parent) :
        QWidget(_parent)
      {
        setup();
      }

      Tuning::~Tuning()
      {
      }

      omni::proj::Tuning* Tuning::tuning()
      {
        return tuning_;
      }

      omni::proj::Tuning const* Tuning::tuning() const
      {
        return tuning_;
      }

      void Tuning::setTuning(omni::proj::Tuning* _tuning)
      {
        tuning_=_tuning;
        setViewMode(mode_);
      }

      slim::RangedFloat* Tuning::addWidget(QString const& _str,float _value,float _min,float _max)
      {
        auto* _widget = new slim::RangedFloat(_str,_value,_min,_max,this);
        _widget->setUseDefaultValue(true);
        
        /// Install event filter to pipe through focus event to parent widget
        _widget->installEventFilter(this);

        /// Signal-slot connection for updating the data model 
        connect(_widget,SIGNAL(valueChanged()),this,SLOT(updateParameters()));
        parameterMap_[_str] = _widget;
        parameters_.push_back(_widget);
        return _widget;
      }

      slim::RangedFloat* Tuning::addAngleWidget(QString const& _str,float _value,float _min,float _max)
      {
        auto* _angle = addWidget(_str,_value,_min,_max);
        _angle->setSuffix("°");
        _angle->setSingleStep(5.0);
        _angle->setPageStep(45.0);
        return _angle;
      }

      slim::RangedFloat* Tuning::addOffsetWidget(QString const& _str,float _value, float _min, float _max)
      {
        auto* _offset = addWidget(_str,_value,_min,_max);
        _offset->setSuffix("m");
        _offset->setSingleStep(0.1);
        _offset->setPageStep(0.1);
        return _offset;
      }
        
      void Tuning::updateParameters()
      {
        if(!tuning_) return;

        auto* _projSetup = tuning_->projectorSetup();
        
        if (!_projSetup) return;
 
        /// Lambda for retrieving the value from a slider
        auto getParamAsFloat = [this](QString const& _str) -> double
        {
          auto* _widget = static_cast<slim::RangedFloat*>(parameterMap_.at(_str));
          return _widget->value();
        };

        /// Handle free projector setup
        if (_projSetup->getTypeId() == "FreeSetup")
        {
          auto* _p = static_cast<omni::proj::FreeSetup*>(_projSetup);
          _p->setYaw(getParamAsFloat("Yaw"));
          _p->setPitch(getParamAsFloat("Pitch"));
          _p->setRoll(getParamAsFloat("Roll"));
          _p->setPos(
              getParamAsFloat("X"),
              getParamAsFloat("Y"),
              getParamAsFloat("Z"));
        } 

        /// Handle Peripheral projector setup 
        if (_projSetup->getTypeId() == "PeripheralSetup")
        {
          auto* _p = static_cast<omni::proj::PeripheralSetup*>(_projSetup);
         
          _p->setYaw(getParamAsFloat("Yaw"));
          _p->setPitch(getParamAsFloat("Pitch"));
          _p->setRoll(getParamAsFloat("Roll"));
          _p->setDeltaYaw(getParamAsFloat("Delta Yaw"));
          _p->setDistanceCenter(getParamAsFloat("Distance"));
          _p->setTowerHeight(getParamAsFloat("Tower Height"));
          _p->setShift(getParamAsFloat("Shift"));
        }
      }

      void Tuning::setup()
      {
        setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

        /// Setup title bar
        titleBar_ = new TitleBar("Projector",this);
        titleBar_->installEventFilter(this);
        connect(titleBar_,SIGNAL(valueChanged()),this,SLOT(updateColor()));

        /// Setup preview window
        glView_ = new GLView2D(this);
        QSizePolicy _sizePolicy(QSizePolicy::Ignored,QSizePolicy::Expanding);
        glView_->setSizePolicy(_sizePolicy);
        glView_->installEventFilter(this);

        /// FOV view slider
        /// @todo Connect this with threshold slider
        auto* _fov =  addWidget("FOV",60.0,10.0,160.0);
        _fov->setSingleStep(4.0);
        _fov->setPageStep(45.0);
        _fov->setSuffix("°");

        /// Throw ratio slider
        /// @todo Connect this with FOV slider
        auto* _throwRatio = addWidget("Throw Ratio",1.0,0.1,5.0);
        _throwRatio->setSingleStep(0.1);
        _throwRatio->setPageStep(0.3);

        /// Yaw angle slider (all projector setups)
        auto&& _yaw = addAngleWidget("Yaw",0.0,0.0,360.0);

        /// Tower height slider (PeripheralSetup only)
        auto&& _towerHeight = addOffsetWidget("Tower Height",2.0,-5.0,10.0);
        _towerHeight->setSingleStep(0.1);
        _towerHeight->setPageStep(1.0);
        _towerHeight->setPivot(0.0);

        /// Distance slider (PeripheralSetup only)
        auto&& _distance = addOffsetWidget("Distance",5.0,0.0,10.0);
        _distance->setPageStep(1.0);

        /// Shift offset slider (PeripheralSetup only)
        auto&& _shift = addOffsetWidget("Shift",0.0,-2.0,2.0);
        _shift->setPageStep(1.0);
        _shift->setPivot(0.0);
        
        /// X offset slider (FreeSetup only)
        auto&& _x = addOffsetWidget("X",0.0,-10.0,10.0);
        _x->setPageStep(1.0);
        _x->setPivot(0.0);
        
        /// Y offset slider (FreeSetup only)
        auto&& _y = addOffsetWidget("Y",0.0,-10.0,10.0);
        _y->setPageStep(1.0);
        _y->setPivot(0.0);
        
        /// Z offset slider (FreeSetup only)
        auto&& _z = addOffsetWidget("Z",0.0,-10.0,10.0);
        _z->setPageStep(1.0);
        _z->setPivot(0.0);

        /// Pitch angle slider (both setups)
        auto&& _pitch = addAngleWidget("Pitch",30.0,-90.0,90.0);
        _pitch->setPivot(0.0);

        /// Roll angle slider (both setups)
        auto&& _roll = addAngleWidget("Roll",0.0,-45.0,45.0);
        _roll->setSingleStep(1.0);
        _roll->setPageStep(5.0);
        _roll->setPivot(0.0);

        /// Delta yaw angle slider (PeripheralSetup only)
        auto&& _deltaYaw = addAngleWidget("Delta Yaw",0.0,-45.0,45.0);
        _deltaYaw->setSingleStep(1.0);
        _deltaYaw->setPageStep(5.0);
        _deltaYaw->setPivot(0.0);
        
        /// Make slider groups 
        sliderGroups_["FreeSetup"] = {_yaw,_roll,_pitch,_x,_y,_z};
        sliderGroups_["PeripheralSetup"] = {_yaw,_distance,_shift,_towerHeight,_pitch,
          _deltaYaw,_roll};
        sliderGroups_["FOV"] = { _fov, _throwRatio };
        
        /// Setup/update view mode
        setViewMode(mode_);
      }

      void Tuning::reorderWidgets()
      {
        if (!titleBar_ || !tuning_) return;

        const int _border = 2;
        int _height = _border;

        /// Hide all widgets
        glView_->hide();
        for (auto& _slider : parameters_)
          _slider->hide();


        /// Our widget list
        std::vector<QWidget*> _widgets = 
        {
          titleBar_
        };

        /// Add preview widget 
        if (mode_ != NO_DISPLAY)
        {
            _widgets.push_back(glView_); 
        }

        /// Add adjustment sliders
        if (mode_ == ADJUSTMENT_SLIDERS)
        {
          for (auto& _slider : sliderGroups_.at(tuning_->projectorSetup()->getTypeId().str()))
          {
            _widgets.push_back(_slider);
          } 
        }

        /// Add FOV sliders
        if (mode_ == FOV_SLIDERS)
        {
              for (auto& _slider : sliderGroups_.at("FOV"))
              {
                _widgets.push_back(_slider);
              } 
        }

        /// Adjust geometry for each widget
        for (auto& _widget : _widgets)
        {
          /// Widget height is constant except for preview
          int _widgetHeight = _widget == glView_ ? width() / 4.0 * 3.0 : 25;
          _widget->setParent(this);
          _widget->setGeometry(_border,_height,width()-_border*2,_widgetHeight);
          _widget->show();
          
          /// Increase height
          _height += _widgetHeight;
        }
        _height += _border;
        
        /// Set minimum size and resize 
        setMinimumSize(0,_height);
        resize(width(),_height);
      }
        
      Tuning::ViewMode Tuning::mode() const
      {
        return mode_;
      }
 
      void Tuning::setViewMode(ViewMode _mode)
      {
        mode_=_mode;
        reorderWidgets();
      }

      void Tuning::setNextViewMode()
      {
        setViewMode(static_cast<ViewMode>((int(mode_) + 1) % int(NUM_MODES)));
      }

      void Tuning::setActive(bool _active)
      {
        active_=_active;
      }

      void Tuning::updateColor()
      {
        /// Widget color has the same color as tuning
        for (auto& _widget : parameters_)
        { 
          QColor _color = active_ ? titleBar_->color().name() : "#cccccc";
          _widget->setStyleSheet("selection-background-color  : "+_color.name());
        }
        update();
      }

      void Tuning::resizeEvent(QResizeEvent* event)
      {
        QWidget::resizeEvent(event);
        update();
      }
   
      void Tuning::showEvent(QShowEvent*)
      {
        reorderWidgets();
      }

      void Tuning::paintEvent(QPaintEvent* event)
      {
        if (!titleBar_) return;

        QPainter _p(this);

        auto _rect = rect().adjusted(2,2,-2,-2);
        
        /// Paint board if active or color is selected
        if (titleBar_->isMoving() || active_)
        {
          _p.setPen(QPen(titleBar_->color(),5));
        } else
        {
          _p.setPen(Qt::NoPen);
        }
        
        _p.setBrush(titleBar_->color());
        _p.drawRect(_rect);

        QWidget::paintEvent(event);
      }

        
      bool Tuning::eventFilter(QObject* _obj, QEvent* _event)
      {
        /// Handle focus events

        if (_event->type() == QEvent::FocusIn)
        {
          active_ = true;
          updateColor();
          return true;
        } else
        if (_event->type() == QEvent::FocusOut)
        {
          active_ = false;
          updateColor();
          return false;
        }
          
        return false;
      }
    }
  }
}
