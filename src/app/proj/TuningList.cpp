#include <omni/ui/proj/TuningList.h>

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

        widgets_.emplace_back(new ui::proj::Tuning(session_->tunings().size()-1,_tuning,this));
        
        auto _widget = widgets_.back().get();
        _widget->setViewMode(viewMode_);
        contents_->layout()->addWidget(_widget); 
 
        _widget->connect(_widget,SIGNAL(selected()),this,SLOT(setCurrentTuning()));
        _widget->connect(_widget,SIGNAL(closed(int)),this,SLOT(removeTuning(int)));
        _widget->connect(_widget,SIGNAL(projectorSetupChanged()),this,SIGNAL(projectorSetupChanged()));
      }

      void TuningList::removeTuning(int _index)
      {
        if (_index == -1) return;

        qDebug() << _index << " " << session_->tunings().size();
        auto* _widget = widgets_[_index].get();
        contents_->layout()->removeWidget(_widget); 
        widgets_.erase(widgets_.begin() + _index);
        session_->tunings().remove(_index);

        // Reorder position indices
        for (int i = 0; i < session_->tunings().size(); ++i)
        {
          widgets_[i]->setTuning(i,session_->tunings()[i]);
        }
      }
        
      /// Set view mode for all tuning widgets
      void TuningList::setViewMode(Tuning::ViewMode _mode)
      {
        viewMode_ = _mode;
        for (auto& _widget : widgets_) { 
          _widget->setViewMode(_mode);
        }
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
        
      void TuningList::setCurrentTuning()
      {
        if (!session_) return;

        int _index = 0;
        for (auto& _widget : widgets_) 
        {
          if (_widget->isSelected()) 
          {
            session_->tunings().setCurrentIndex(_index);
            break;
          }
          ++_index;
        }
      }
    }
  }
}
