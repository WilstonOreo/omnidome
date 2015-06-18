#include <omni/ui/proj/TuningList.h>

#include <omni/ui/proj/Tuning.h>
#include <omni/Session.h>

#include <QVBoxLayout>

#include <QDebug>

namespace omni
{
  namespace ui
  {
    namespace proj
    {
      TuningList::TuningList(QWidget* _parent) :
        QScrollArea(_parent),
        layout_(new QVBoxLayout())
      {
        // Set contents and layout, so tuning widgets can be placed vertically using
        // QScrollArea funcionality
        layout_->setSizeConstraint(QLayout::SetMinAndMaxSize);
        layout_->setSpacing(3);
        layout_->setContentsMargins(0,0,0,0);

        this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

        contents_ = new QWidget(this);
        contents_->setLayout(layout_);
        this->setWidget(contents_);
        contents_->show();
  
        this->setWidgetResizable(true);
      }

      TuningList::~TuningList()
      {
      }

      Session const* TuningList::session() const
      {
        return session_;
      }

      void TuningList::setSession(Session* _session)
      {
        session_=_session;
       
        
        clear();
      }

      void TuningList::addTuning()
      {
        auto* _tuning = session_->tunings().add();

        if (!_tuning) return;

        widgets_.emplace_back(new ui::proj::Tuning(_tuning,this));
        contents_->layout()->addWidget(widgets_.back().get()); 
      }

      void TuningList::removeTuning()
      {
        session_->tunings().remove();
      }

      void TuningList::clear()
      {
        widgets_.clear();
        session_->tunings().clear();
      }
        
      void TuningList::resizeEvent(QResizeEvent* event)
      {
        QScrollArea::resizeEvent(event);
        for (auto& _widget : widgets_) 
          _widget->reorderWidgets();
      }
    }
  }
}
