#include <omni/ui/proj/TitleBar.h>
#include <omni/ui/proj/Tuning.h>

#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QHBoxLayout>

namespace omni
{
  namespace ui
  {
    namespace proj
    {
      TitleBar::TitleBar(Tuning* _tuning) :
        slim::Widget(_tuning),
        mixin_range_type(0,0,1,0.01,0.1),
        tuningWidget_(_tuning)
      {
        setup();
      }

      TitleBar::TitleBar(QString const& _label, Tuning* _tuning) :
        slim::Widget(_label,_tuning),
        mixin_range_type(0,0,1,0.01,0.1),
        tuningWidget_(_tuning)
      {
        setup();
      }

      TitleBar::~TitleBar()
      {
      }

      void TitleBar::setValue(double _value)
      {
        mixin_range_type::setValue(_value);
        update();
        emit valueChanged();
      }


      QColor TitleBar::color() const
      {
        QImage _image(width(),1,QImage::Format_ARGB32);
        QPainter _p(&_image);

        /// Draw gradient on image
        _p.setPen(Qt::NoPen);
        _p.setBrush(gradient_);
        _p.drawRect(QRect(0,0,width(),1));
        _p.end();

        double _pos = valueToPos();
        if (_pos <  0.0) _pos = 0.0;
        if (_pos >= width()-1) _pos = width()-1;

        /// Get pixel 
        return _image.pixel(_pos,0);
      }


      bool TitleBar::isMoving() const
      {
        return moving_;
      }

      void TitleBar::paintEvent(QPaintEvent* event)
      {
        QPainter _p(this);

        auto _rect = rect().adjusted(1,1,-1,-1);
        _p.setPen(Qt::NoPen);

        if (!moving_)
        {
          _p.setBrush(color());
        }
        else
        {
          _p.setBrush(gradient_);
        }

        _p.drawRect(_rect);

        // Draw Handle when moving
        if (moving_)
        {
          double _pos = valueToPos();

          QRectF _progressRect = rect();

          _progressRect.setRight(_pos);
          _p.setPen(QPen(colorSet().dark(),4));
          _p.drawLine(
            QPointF(_pos,_progressRect.bottom()+2),
            QPointF(_pos,_progressRect.top()- 2));
        }

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
        setupToolButton(displayButton_);
        
        menuButton_->setArrowType(Qt::DownArrow);

        menu_.reset(new QMenu);
        
        /// Generate and add popup menu
        menuButton_->setMenu(menu_.get());
        menuButton_->setPopupMode(QToolButton::InstantPopup);
        menu_->addAction("Reset color");
        menu_->addAction("Reset adjustment");
        menu_->addSeparator();
        auto* _peripheral = menu_->addAction("Peripheral Setup");
        _peripheral->setCheckable(true);
        
        auto* _free = menu_->addAction("Free Setup");
        _free->setCheckable(true);

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

        /// Spectral gradient for color selection
        QLinearGradient _gradient(QPointF(0.0,0.0),QPointF(1.0,0.0));
        _gradient.setCoordinateMode(QLinearGradient::ObjectBoundingMode);
        _gradient.setColorAt(0.0/6.0,"#FF0000");
        _gradient.setColorAt(1.0/6.0,"#FFFF00");
        _gradient.setColorAt(2.0/6.0,"#00FF00");
        _gradient.setColorAt(3.0/6.0,"#00FFFF");
        _gradient.setColorAt(4.0/6.0,"#0000FF");
        _gradient.setColorAt(5.0/6.0,"#FF00FF");
        _gradient.setColorAt(6.0/6.0,"#FF0000");
        gradient_ = _gradient;
      }
        
      void TitleBar::setMoving(bool _moving)
      {
        moving_ = _moving;
        /// Display buttons only when not moving 
        displayButton_->setVisible(!_moving);
        maximizeButton_->setVisible(!_moving);
        closeButton_->setVisible(!_moving);
        menuButton_->setVisible(!_moving); 
      }
  
      Tuning* TitleBar::tuningWidget()
      {
        return tuningWidget_;
      }

      double TitleBar::valueToPos() const
      {
        return valueToPos(value());
      }

      double TitleBar::valueToPos(double _value) const
      {
        auto&& _rect = rect();
        return _rect.left() + slim::mixin::Range<double>::ratio(_value)* double(_rect.width());
      }

      void TitleBar::mouseMoveEvent(QMouseEvent* e)
      {
        if (moving_)
        {
          setValue(valueFromPos(e->pos().x()));
        }
      }

      void TitleBar::mousePressEvent(QMouseEvent* e)
      {
        setMoving(true);
        setValue(valueFromPos(e->pos().x()));
      }

      void TitleBar::mouseReleaseEvent(QMouseEvent* e)
      {
        if (moving_)
        {
          setValue(valueFromPos(e->pos().x()));
          update();
        }
        setMoving(false);
      }

      void TitleBar::wheelEvent(QWheelEvent* e)
      {
        if (hasFocus())
          setValue(value() + e->delta()/15.0 / 8.0 * singleStep());
      }

      double TitleBar::valueFromPos(double _pos) const
      {
        auto&& _rect = rect();
        return (_pos - _rect.left()) / _rect.width() * (maximum() - minimum()) + minimum();
      }

    }
  }
}
