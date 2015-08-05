#include <omni/ui/proj/TuningListWidget.h>

#include <omni/ui/proj/TuningWidget.h>
#include <omni/Session.h>

#include <QVBoxLayout>

namespace omni
{
  namespace ui
  {
    namespace proj
    {
      TuningListWidget::TuningListWidget(QWidget* _parent) :
        QScrollArea(_parent)
      {
        // Set contents and layout, so mapping widgets can be placed vertically using
        // QScrollArea funcionality
        contents_ = new QWidget(this);
        layout_ = new QVBoxLayout(contents_);
        layout_->setSizeConstraint(QLayout::SetMinAndMaxSize);
        this->setWidget(contents_);
      }

      TuningListWidget::~TuningListWidget()
      {
      }

      Session const* TuningListWidget::session() const
      {
        return session_.get();
      }

      void TuningListWidget::setSession(std::shared_ptr<Session> _session)
      {
        session_=_session;
        clear();
      }

      void TuningListWidget::addTuning()
      {
        auto* _mapping = session_->mappings().add();

        if (!_mapping) return;

        widgets_.emplace_back(new TuningWidget(_mapping));
        this->layout()->addWidget(widgets_.back().get());
      }

      void TuningListWidget::removeTuning()
      {
        session_->mappings().remove();
      }

      void TuningListWidget::clear()
      {
        widgets_.clear();
        session_->mappings().clear();
      }
    }
  }
}
