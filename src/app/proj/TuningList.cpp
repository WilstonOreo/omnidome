#include <omni/ui/proj/TuningList.h>

#include <omni/Session.h>

#include <QVBoxLayout>
#include <QMessageBox>

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

        for (auto& _tuning : _session->tunings())
          addTuning(_tuning.get());

        sessionModeChange();
      }

      void TuningList::addTuning()
      {
        auto* _tuning = session_->tunings().add();

        if (!_tuning) return;

        _tuning->setColor(getTuningColor());
        addTuning(_tuning);
      }
        
      void TuningList::addTuning(omni::proj::Tuning* _tuning)
      {
        if (widgets_.size() >= maxNumberTunings()) 
        {
          QMessageBox::information(this,"Information", QString("You have reached the maximum of %1 projectors.").arg(maxNumberTunings()));
          return;
        }

        widgets_.emplace_back(new ui::proj::Tuning(session_->tunings().size()-1,session_,this));
        auto _widget = widgets_.back().get();
        contents_->layout()->addWidget(_widget); 
 
        _widget->connect(_widget,SIGNAL(selected()),this,SLOT(setCurrentTuning()));
        _widget->connect(_widget,SIGNAL(closed(int)),this,SLOT(removeTuning(int)));
        _widget->connect(_widget,SIGNAL(projectorSetupChanged()),this,SIGNAL(projectorSetupChanged()));
        _widget->sessionModeChange();
      }

      QColor TuningList::getTuningColor() 
      {
        // Generate standard colors, for tuning
        std::vector<QColor> _colors;

        int _hue = 0;
        int _hueDiff = 120;
        for (int i = 0; i < maxNumberTunings(); ++i)
        {
          int _saturation = float(maxNumberTunings()/2 - i/2) / maxNumberTunings() * 2.0 * 255.0; //int(i * 2.0 / maxNumberTunings()) * 255;
          _colors.push_back(QColor::fromHsv(_hue,_saturation,255));

          _hue += 120;
          if (_hue >= 360) {
            _hueDiff /= 2;
            _hue += _hueDiff;
            _hue %= 360;
          }
        }
        int _numTunings = session_->tunings().size();

        if (session_->tunings().size() > maxNumberTunings()) return QColor("#000000");

        return _colors[_numTunings-1];
      }

      void TuningList::removeTuning(int _index)
      {
        if (_index == -1) return;

        auto* _widget = widgets_[_index].get();
        contents_->layout()->removeWidget(_widget); 
        widgets_.erase(widgets_.begin() + _index);
        session_->tunings().remove(_index);

        // Reorder position indices
        for (int i = 0; i < session_->tunings().size(); ++i)
        {
          widgets_[i]->setTuning(i,session_);
        }
      }

      void TuningList::sessionModeChange()
      {
        for (auto& _widget : widgets_) 
          _widget->sessionModeChange();
      }

      void TuningList::clear()
      {
        widgets_.clear();
        session_->tunings().clear();
      }
        
      /// Updates/Repaints GL Views of all tunings widgets
      void TuningList::updateViews()
      {
        for (auto& _widget : widgets_) 
          _widget->updateViews();
      }
 
      void TuningList::resizeEvent(QResizeEvent* event)
      {
        QScrollArea::resizeEvent(event);
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
            emit currentIndexChanged(_index);
            break;
          }
          ++_index;
        }
      }
    }
  }
}
