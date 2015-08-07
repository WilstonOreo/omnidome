#include <omni/ui/Warp.h>

#include "ui_omni_ui_Warp.h"
#include <QMessageBox>

#include <omni/Session.h>
#include <omni/WarpGrid.h>

namespace omni
{
  namespace ui
  {
    Warp::Warp(QWidget* _parent) :
      QGroupBox(_parent),
      ui_(new Ui::Warp)
    {
      ui_->setupUi(this);

      connect(ui_->btnResize,SIGNAL(clicked(bool)),this,SLOT(resizeWarpGrid(bool)));
      connect(ui_->btnReset,SIGNAL(clicked()),this,
              SLOT(resetWarpGrid()));

      auto setupSlider = [&](slim::RangedInt* _slider)
      {
        _slider->hide();
        _slider->setRange(2,12);
        _slider->setSingleStep(1);
        _slider->setPageStep(12);
        _slider->setDefaultValue(6);
        _slider->setUseDefaultValue(true);
        connect(_slider,SIGNAL(valueChanged()),this,SLOT(resizeWarpGrid()));
      };

      setupSlider(ui_->sliderHorz);
      ui_->sliderHorz->setLabel("Horizontal");

      setupSlider(ui_->sliderVert);
      ui_->sliderVert->setLabel("Vertical");
    }

    Warp::~Warp()
    {
    }

    Session const* Warp::session() const
    {
      return session_;
    }

    void Warp::setSession(Session* _session)
    {
      session_=_session;
      updateWarpGrid();
    }
      
    void Warp::updateWarpGrid()
    {
      if (warpGrid())
      {
        locked_ = true;
        ui_->sliderHorz->setValue(warpGrid()->horizontal());
        ui_->sliderVert->setValue(warpGrid()->vertical());
        locked_ = false;
      }
    }

    void Warp::resetWarpGrid()
    {
      if (!warpGrid() || locked_) return;

      warpGrid()->reset();

      emit warpGridChanged();
    }

    void Warp::resizeWarpGrid(bool _enabled)
    {
      if (!warpGrid() || locked_) return;

      if (_enabled && !warpGrid()->isReset())
      {
        QMessageBox::StandardButton _reply =
          QMessageBox::question(this,"Reset warp grid",
                                "Changing the warp grid resolution resets all warp grid points. Do you want to continue?",
                                QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
        if (_reply == QMessageBox::No)
        {
          ui_->btnResize->setChecked(false);
          return;
        }
      }

      ui_->sliderVert->setVisible(_enabled);
      ui_->sliderHorz->setVisible(_enabled);
    }

    void Warp::resizeWarpGrid()
    {
      if (!warpGrid() || locked_) return;

      warpGrid()->resize(ui_->sliderHorz->value(),ui_->sliderVert->value());
      emit warpGridChanged();
    }

    omni::WarpGrid const* Warp::warpGrid() const
    {
      if (!session_) return nullptr;

      return session_->tunings().current() ? &session_->tunings().current()->warpGrid() : nullptr;
    }

    omni::WarpGrid* Warp::warpGrid()
    {
      if (!session_) return nullptr;

      return session_->tunings().current() ? &session_->tunings().current()->warpGrid() : nullptr;
    }
  }
}
