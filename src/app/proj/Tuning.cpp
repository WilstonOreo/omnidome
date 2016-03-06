/* Copyright (c) 2014-2015 "Omnidome" by cr8tr
 * Dome Mapping Projection Software (http://omnido.me).
 * Omnidome was created by Michael Winkelmann aka Wilston Oreo (@WilstonOreo)
 *
 * This file is part of Omnidome.
 *
 * Omnidome is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "proj/Tuning.h"

#include <vector>

#include <QDebug>
#include <QResizeEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QDrag>
#include <QMimeData>
#include <QPixmap>


#include <omni/ui/TuningGLView.h>
#include <omni/proj/FreeSetup.h>
#include <omni/proj/PeripheralSetup.h>

#include "proj/TitleBar.h"
#include "proj/TuningLayout.h"


namespace omni
{
  namespace ui
  {
    namespace proj
    {
      Tuning::Tuning(
        int _index,
        omni::Session* _session,
        QWidget* _parent) :
        ParameterWidget(_parent)
      {
        setup();
        setTuning(_index,_session);
      }

      Tuning::Tuning(
        QWidget* _parent) :
        ParameterWidget(_parent)
      {
        setup();
      }

      Tuning::~Tuning()
      {
        delete glView_;
        delete titleBar_;
      }

      omni::proj::Tuning* Tuning::tuning()
      {
        return session_ ? session_->tunings()[index_] : nullptr;
      }

      omni::proj::Tuning const* Tuning::tuning() const
      {
        return session_ ? session_->tunings()[index_] : nullptr;
      }

      void Tuning::setTuning(int _index, omni::Session* _session)
      {
        bool _newTuning = (session_ != _session) || (index_ != _index);
        index_=_index;
        session_ = _session;

        if (_newTuning)
        {
          glView_->setSession(session_);
          glView_->setTuningIndex(index_);
          fullscreen_->setSession(session_);
          fullscreen_->setTuningIndex(index_);
          titleBar_->setColor(tuning()->color());

          setParamAsFloat("FOV",tuning()->projector().fov().degrees());
          setParamAsFloat("Throw Ratio",tuning()->projector().throwRatio());

          auto _projSetup = tuning()->projector().setup();

          if (_projSetup->getTypeId() == "FreeSetup")
          {
            auto* _p = static_cast<omni::proj::FreeSetup*>(_projSetup);
            setParamAsFloat("Yaw",_p->yaw().degrees());
            setParamAsFloat("Pitch",_p->pitch().degrees());
            setParamAsFloat("Roll",_p->roll().degrees());
            setParamAsFloat("X",_p->pos().x());
            setParamAsFloat("Y",_p->pos().y());
            setParamAsFloat("Z",_p->pos().z());
          }
          else if (_projSetup->getTypeId() == "PeripheralSetup")
          {
            auto* _p = static_cast<omni::proj::PeripheralSetup*>(_projSetup);
            setParamAsFloat("Yaw",_p->yaw().degrees());
            setParamAsFloat("Pitch",_p->pitch().degrees());
            setParamAsFloat("Roll",_p->roll().degrees());
            setParamAsFloat("Delta Yaw",_p->deltaYaw().degrees());
            setParamAsFloat("Distance",_p->distanceCenter());
            setParamAsFloat("Tower Height",_p->towerHeight());
            setParamAsFloat("Shift",_p->shift());
          }

          sessionModeChange();
          updateParameters();
        }
      }

      TuningGLView* Tuning::fullscreenWidget()
      {
        return fullscreen_.get();
      }

      TuningGLView const* Tuning::fullscreenWidget() const
      {
        return fullscreen_.get();
      }

      TuningGLView* Tuning::previewWidget()
      {
        return glView_;
      }

      TuningGLView const* Tuning::previewWidget() const
      {
        return glView_;
      }


      /// Enable or disable fullscreen display
      void Tuning::fullscreenToggle(bool _enabled)
      {
        glView_->setDrawingEnabled(_enabled);
        if (fullscreen_)
        {
          fullscreen_->setDrawingEnabled(_enabled);
        }
      }

      int Tuning::index() const
      {
        return index_;
      }

      Session const* Tuning::session() const
      {
        return session_;
      }

      void Tuning::updateParameters()
      {
        if (!tuning() || isLocked()) return;

        auto* _projSetup = tuning()->projector().setup();

        if (!_projSetup) return;

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
        tuning()->projector().setup();

        updateViews();
        emit projectorSetupChanged();
      }

      void Tuning::setFov()
      {
        if (!tuning() || isLocked()) return;

        tuning()->projector().setFov(getParamAsFloat("FOV"));
        setParamAsFloat("Throw Ratio",tuning()->projector().throwRatio());

        updateViews();
        emit projectorSetupChanged();
      }

      /// Set Throw Ratio to projector from slider
      void Tuning::setThrowRatio()
      {
        if (!tuning() || isLocked()) return;

        tuning()->projector().setThrowRatio(getParamAsFloat("Throw Ratio"));
        setParamAsFloat("FOV",tuning()->projector().fov().degrees());

        updateViews();
        emit projectorSetupChanged();
      }


      void Tuning::attachScreen(QScreen const* _screen, int _subScreenIndex)
      {
        tuning()->setScreen(_screen,_subScreenIndex);
        emit projectorSetupChanged();
        layout()->update();
      }

      void Tuning::detachScreen()
      {
        tuning()->setScreen(nullptr,0);
        emit projectorSetupChanged();
        layout()->update();
      }

      void Tuning::updateViews()
      {
        glView_->update();
        if (fullscreen_)
          fullscreen_->update();
      }

      void Tuning::setup()
      {
        this->setFocusPolicy(Qt::TabFocus);
        this->installEventFilter(this->parent());
        layout_ = new TuningLayout(this);
        setLayout(layout_);

        setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

        /// Setup title bar
        titleBar_ = new TitleBar("Projector",this);
        titleBar_->installEventFilter(this);
//        this->installEventFilter(titleBar_);
        connect(titleBar_,SIGNAL(closeButtonClicked()),this,SLOT(prepareRemove()));
        layout_->addWidget(titleBar_,TuningLayout::Role::TITLE);

        /// Setup preview window
        glView_ = new TuningGLView(this);
        QSizePolicy _sizePolicy(QSizePolicy::Ignored,QSizePolicy::Expanding);
        glView_->setSizePolicy(_sizePolicy);
        glView_->setKeepAspectRatio(true);
        glView_->setBorder(0.0);
        glView_->setViewOnly(true);
        glView_->installEventFilter(this);
        layout_->addWidget(glView_,TuningLayout::Role::PREVIEW);

        fullscreen_.reset(new TuningGLView());
        fullscreen_->setViewOnly(true);
        fullscreen_->setShowCursor(true);
        fullscreen_->hide();

        /// FOV view slider
        /// @todo Connect this with threshold slider
        auto* _fov =  addWidget("FOV",60.0,10.0,160.0);
        _fov->setSingleStep(1.0);
        _fov->setPageStep(5.0);
        _fov->setSuffix("°");
        connect(_fov,SIGNAL(valueChanged()),this,SLOT(setFov()));

        /// Throw ratio slider
        /// @todo Connect this with FOV slider
        auto* _throwRatio = addWidget("Throw Ratio",1.0,0.1,5.0);
        _throwRatio->setSingleStep(0.01);
        _throwRatio->setPageStep(0.05);
        connect(_throwRatio,SIGNAL(valueChanged()),this,SLOT(setThrowRatio()));

        /// Yaw angle slider (all projector setups)
        auto&& _yaw = addAngleWidget("Yaw",0.0,0.0,360.0);
        _yaw->setSingleStep(0.1);
        _yaw->setPageStep(1.0);

        /// Tower height slider (PeripheralSetup only)
        auto&& _towerHeight = addOffsetWidget("Tower Height",2.0,-5.0,10.0);

        /// Distance slider (PeripheralSetup only)
        auto&& _distance = addOffsetWidget("Distance",4.0,0.0,10.0);

        /// Shift offset slider (PeripheralSetup only)
        auto&& _shift = addOffsetWidget("Shift",0.0,-2.0,2.0);

        /// X offset slider (FreeSetup only)
        auto&& _x = addOffsetWidget("X",0.0,-10.0,10.0);

        /// Y offset slider (FreeSetup only)
        auto&& _y = addOffsetWidget("Y",0.0,-10.0,10.0);

        /// Z offset slider (FreeSetup only)
        auto&& _z = addOffsetWidget("Z",0.0,-10.0,10.0);

        /// Pitch angle slider (both setups)
        auto&& _pitch = addAngleWidget("Pitch",30.0,-180.0,180.0);
        _pitch->setSingleStep(0.1);
        _pitch->setPageStep(1.0);
        _pitch->setPivot(0.0);

        /// Roll angle slider (both setups)
        auto&& _roll = addAngleWidget("Roll",0.0,-180.0,180.0);
        _roll->setSingleStep(0.1);
        _roll->setPageStep(1.0);
        _roll->setPivot(0.0);

        /// Delta yaw angle slider (PeripheralSetup only)
        auto&& _deltaYaw = addAngleWidget("Delta Yaw",0.0,-45.0,45.0);
        _deltaYaw->setSingleStep(0.1);
        _deltaYaw->setPageStep(1.0);
        _deltaYaw->setPivot(0.0);

        widgetgroup_type _titleAndPreview(
        {
          { titleBar_, TuningLayout::Role::TITLE } ,
          { glView_, TuningLayout::Role::PREVIEW }
        });

        auto addParameters = [&](widgetgroup_type const& _group, std::vector<QWidget*> const& _widgets) -> widgetgroup_type
        {
          widgetgroup_type _result = _group;
          for (auto& _widget : _widgets)
            _result.emplace_back(_widget,TuningLayout::Role::PARAMETER);
          return _result;
        };

        addGroup("Minimized", { { titleBar_, TuningLayout::Role::TITLE } });

        /// Make slider groups
        addGroup("PreviewOnly",_titleAndPreview);
        addGroup("FOVSliders",addParameters(_titleAndPreview, { _fov, _throwRatio }));
        addGroup("FreeSetup",addParameters( _titleAndPreview, {_yaw, _pitch, _roll, _x, _y, _z, _fov, _throwRatio }));
        addGroup("PeripheralSetup",
                 addParameters( _titleAndPreview,
        { _yaw, _pitch, _distance, _towerHeight, _shift, _deltaYaw, _roll, _fov, _throwRatio }));

        /// Setup/update mode
        sessionModeChange();

        for (int i = 0; i < layout_->count(); ++i) {
            auto _widget = layout_->itemAt(i)->widget();
            if (!_widget) continue;
            _widget->installEventFilter(this);
            _widget->installEventFilter(this->parent());
        //    _widget->setFocusPolicy(Qt::TabFocus);
        }
      }

      /// Adds a new/changes a parameter group
      void Tuning::addGroup(QString const& _groupName, widgetgroup_type const& _widgets)
      {
        if (_groupName != "")
        {
          groups_[_groupName] = _widgets;
        }
      }

      void Tuning::setGroup(QString const& _groupName)
      {
        this->locked([&]() {

        /// Hide all widgets temporarily
        setParametersVisible(false);
        glView_->setVisible(false);

        if (groups_.count(_groupName) == 0)
        {
          // Show title bar only if group with that name does not exist
          layout_->setWidgets({{ titleBar_, TuningLayout::Role::TITLE }});

        }
        else
        {
          layout_->setWidgets(groups_[_groupName]);
        }
    });
      }

      Tuning::WindowState Tuning::windowState() const
      {
        return windowState_;
      }

      void Tuning::setWindowState(WindowState _windowState)
      {
        windowState_ = _windowState;
        sessionModeChange();
      }

      void Tuning::setNextWindowState()
      {
        int _numStates = int(NUM_WINDOW_STATES);
        if (session())
        {
          switch (session()->mode())
          {
          case Session::Mode::SCREENSETUP:
            _numStates = 3;
            break;
          case Session::Mode::WARP:
          case Session::Mode::BLEND:
          case Session::Mode::COLORCORRECTION:
          case Session::Mode::LIVE:
          case Session::Mode::EXPORT:
            _numStates = 2;
            break;
          default:
            _numStates = int(NUM_WINDOW_STATES);
          }
        }

        setWindowState(static_cast<WindowState>((int(windowState_) + 1) % _numStates));
      }

      void Tuning::setSelected(bool _isSelected)
      {
        isSelected_ = _isSelected;
        if (isSelected_ && !this->isLocked()) emit selected(index_);

        updateColor();
      }

      void Tuning::updateColor()
      {
        /// Widget color has the same color as tuning
        for (auto& _widget : this->parameters_)
        {
          QColor _color = isSelected_ ? titleBar_->color().name() : "#cccccc";
          _widget->setStyleSheet("selection-background-color  : "+_color.name());

          if (tuning() && isSelected_)
            tuning()->setColor(_color);
        }
        update();
      }

      void Tuning::prepareRemove()
      {
        glView_->destroy();
        emit closed(index_);
      }

      void Tuning::sessionModeChange()
      {
        if (!session()) return;

        this->locked([&]() {
        if (windowState_ == NO_DISPLAY)
        {
          setGroup("Minimized");
          return;
        }
        if (windowState_ == DISPLAY_ONLY)
        {
          setGroup("PreviewOnly");
          return;
        }

        auto _mode = session()->mode();

        // Show close button only in screen- and projection setup
        titleBar_->setCloseButtonVisible(
            _mode == Session::Mode::SCREENSETUP ||
            _mode == Session::Mode::ARRANGE);

        switch (_mode)
        {
        case Session::Mode::SCREENSETUP:
          setGroup("FOVSliders");
          break;
        case Session::Mode::ARRANGE:
          if (windowState_ == ADJUSTMENT_SLIDERS)
            setGroup(tuning()->projector().setup()->getTypeId().str());
          if (windowState_ == FOV_SLIDERS)
            setGroup("FOVSliders");
          break;
        case Session::Mode::WARP:
        case Session::Mode::BLEND:
        case Session::Mode::COLORCORRECTION:
        case Session::Mode::EXPORT:
        case Session::Mode::LIVE:
          setGroup("PreviewOnly");
          break;
        default:
          break;
        }
    });

        fullscreen_->update();
      }

      void Tuning::resizeEvent(QResizeEvent* event)
      {
        QWidget::resizeEvent(event);
        update();
      }

      void Tuning::showEvent(QShowEvent*)
      {
      }

      void Tuning::paintEvent(QPaintEvent* event)
      {
        if (!titleBar_ || !isEnabled()) return;

        QPainter _p(this);

        auto _rect = rect().adjusted(2,2,-2,-2);

        /// Paint board if active or color is selected
        if (isSelected_)
        {
          _p.setPen(QPen(titleBar_->color(),5));
        }
        else
        {
          _p.setPen(Qt::NoPen);
        }

        _p.setBrush(QColor("#212121"));
        _p.drawRect(_rect);

        QWidget::paintEvent(event);
      }

      /// Mouse Move Event and handler for dragging to ScreenSetup widget
      void Tuning::mouseMoveEvent(QMouseEvent* event)
      {
        // Handle drag to ScreenWidget
        if (event->button() == Qt::LeftButton)
        {
          startDrag();
        }
      }

      void Tuning::startDrag()
      {
        if (!session()) return;

        if (session()->mode() != Session::Mode::SCREENSETUP) return;

        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;

        // Generate pixmap for projector
        QPixmap _pixmap(128,128);
        {
          _pixmap.fill(tuning()->color());
          QPainter _p(&_pixmap);
          QRect _rect(0,0,128,128);

          QFont _font("Helvetica",32);
          _p.setFont(_font);

          _p.drawRect(_rect.adjusted(1,1,-2,-2));
          _p.drawText(_rect,Qt::AlignCenter, QString("%1").arg(index_+1));
          _p.end();
        }
        drag->setPixmap(_pixmap);

        mimeData->setText(QString("%1").arg(index_));
        drag->setMimeData(mimeData);
        drag->exec();
      }

      bool Tuning::eventFilter(QObject* _obj, QEvent* _event)
      {
        if (this->isLocked()) return QObject::eventFilter(_obj,_event);

        if (_event->type() == QEvent::MouseMove && (_obj == glView_ || _obj == titleBar_))
        {
          startDrag();
        }

        /// Handle focus events
        if (_event->type() == QEvent::FocusIn)
        {
          setSelected(true);
          return true;
        }
        else if (_event->type() == QEvent::FocusOut)
        {
          setSelected(false);
          return false;
        }

        return QObject::eventFilter(_obj,_event);
      }

      bool Tuning::isSelected() const
      {
        return isSelected_;
      }

      /// Focus event used by TuningList to set current tuning for session
      void Tuning::focusInEvent(QFocusEvent* event)
      {
          if (!this->isLocked())
            setSelected(true);
        QWidget::focusInEvent(event);
      }

      void Tuning::focusOutEvent(QFocusEvent* event)
      {
          if (!this->isLocked())
            setSelected(false);
        QWidget::focusOutEvent(event);
      }

    }
  }
}
