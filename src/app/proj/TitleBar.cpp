#include <omni/ui/proj/TitleBar.h>
#include <omni/ui/proj/Tuning.h>

#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QHBoxLayout>
#include <QColorDialog>

namespace omni
{
  namespace ui
  {
    namespace proj
    {
      TitleBar::TitleBar(Tuning* _tuning) :
        slim::Widget(_tuning),
        tuningWidget_(_tuning)
      {
        setup();
      }

      TitleBar::TitleBar(QString const& _label, Tuning* _tuning) :
        slim::Widget(_label,_tuning),
        tuningWidget_(_tuning)
      {
        setup();
      }

      TitleBar::~TitleBar()
      {
      }

      void TitleBar::setColor(QColor const& _color)
      {
        color_=_color;
        emit colorChanged(_color);
        update(); 
      }

      void TitleBar::selectColor()
      {
        QColorDialog _colorDialog;
        _colorDialog.setCurrentColor(color());
        _colorDialog.setOptions(
                /* do not use native dialog */
                QColorDialog::DontUseNativeDialog);
        
        if (_colorDialog.exec())
          setColor(_colorDialog.selectedColor());

      }

      QColor TitleBar::color() const
      {
        return color_;
      }

      void TitleBar::paintEvent(QPaintEvent* event)
      {
        QPainter _p(this);

        auto _rect = rect().adjusted(1,1,-1,-1);
        _p.setPen(Qt::NoPen);
        _p.setBrush(color());
        _p.drawRect(_rect);
        QWidget::paintEvent(event);
      }
        
      void TitleBar::setup()
      {
        this->label_->setAlignment(Qt::AlignCenter);

        layout()->removeWidget(this->label_);

        ///////////////////// Setup buttons
        auto setupToolButton = [this](QUniquePtr<QToolButton>& _btn)
        {
          _btn.reset(new QToolButton());
          _btn->setAutoRaise(true);
          _btn->setStyleSheet("background : transparent");
          _btn->installEventFilter(this->parent());
          _btn->installEventFilter(this);
        };
 
        setupToolButton(menuButton_);
        
        menuButton_->setArrowType(Qt::DownArrow);

        menu_.reset(new QMenu);
        
        /// Generate and add popup menu
        menuButton_->setMenu(menu_.get());
        menuButton_->setPopupMode(QToolButton::InstantPopup);
        auto* _changeColor = menu_->addAction("Change color...");
        connect(_changeColor,SIGNAL(triggered()),this,SLOT(selectColor()));

        menu_->addSeparator();
        auto* _peripheral = menu_->addAction("Peripheral Setup");
        _peripheral->setCheckable(true);
        
        auto* _free = menu_->addAction("Free Setup");
        _free->setCheckable(true);
        
        setupToolButton(displayButton_);
        displayButton_->setCheckable(true);
        connect(displayButton_.get(),SIGNAL(clicked(bool)),tuningWidget(),SLOT(fullscreenToggle(bool)));


        setupToolButton(maximizeButton_);
        connect(maximizeButton_.get(),SIGNAL(clicked()),tuningWidget(),SLOT(setNextWindowState()));

        setupToolButton(closeButton_);
        connect(closeButton_.get(),SIGNAL(clicked()),this,SIGNAL(closeButtonClicked()));

        ///////////////////// END Setup buttons
        
        /// Add widget in this left-to-right order to layout
        layout()->addWidget(menuButton_.get());
        layout()->addWidget(displayButton_.get());
        layout()->addWidget(this->label_);
        layout()->addWidget(maximizeButton_.get());
        layout()->addWidget(closeButton_.get());
        layout()->setSpacing(0);
        layout()->setContentsMargins(0,0,0,0);
        
        if (tuningWidget_ && tuningWidget_->tuning())
          setColor(tuningWidget_->tuning()->color());
      }
  
      Tuning* TitleBar::tuningWidget()
      {
        return tuningWidget_;
      }

    }
  }
}
