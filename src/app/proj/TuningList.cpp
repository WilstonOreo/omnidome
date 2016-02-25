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

#include "proj/TuningList.h"

#include <omni/Session.h>
#include <omni/proj/MultiSetup.h>
#include "proj/MultiSetupDialog.h"

#include <QVBoxLayout>
#include <QMessageBox>
#include <QKeyEvent>

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

      QWidget* TuningList::widget(int _index) {
         return (_index >= 0) && (_index < widgets_.size()) ?
            widgets_[_index].get() : nullptr;
      }

      QWidget const* TuningList::widget(int _index) const {
         return (_index >= 0) && (_index < widgets_.size()) ?
            widgets_[_index].get() : nullptr;
      }

      Session const* TuningList::session() const
      {
        return session_;
      }

      void TuningList::setSession(Session* _session)
      {
        session_=_session;
        removeWidgets();
        for (auto& _tuning : _session->tunings())
          addTuning(_tuning.get());

        sessionModeChange();
      }


      /// Return fullscreen and preview widget from index
      std::set<TuningGLView*> TuningList::getViews(int _index) const
      {
        std::set<TuningGLView*> _views;
        if (_index < 0 || _index >= widgets_.size()) return _views;

        auto* _widget = widgets_[_index].get();
        _views.insert(_widget->fullscreenWidget());
        _views.insert(_widget->previewWidget());

        return _views;
      }

      void TuningList::addTuning()
      {
        QString _setupId = "PeripheralSetup";
        if (!session_->tunings().empty())
        {
          // Set setup id from last tuning
          auto* _setup = session_->tunings()[session_->tunings().size()-1]->projector().setup();
          if (_setup)
            _setupId = _setup->getTypeId();
        }
        addTuning(_setupId);
      }

      /// Add tuning with specific projector setup
      void TuningList::addTuning(QString const& _projSetupId)
      {
        auto* _tuning = session_->tunings().add(false);

        if (!_tuning) return;

        _tuning->setColor(getTuningColor());
        _tuning->projector().setup(_projSetupId);
        addTuning(_tuning);

        // Select this tuning index
        setTuningIndex(session_->tunings().size()-1);
      }

      /// Opens multi setup dialog and appends/replaces new projections when dialogs was accepted
      void TuningList::addMultiSetup(QString const& _multiSetupId)
      {
        std::unique_ptr<omni::proj::MultiSetup> _multiSetup ;
        _multiSetup.reset(omni::proj::MultiSetupFactory::create(_multiSetupId));
        if (!_multiSetup) return;

        auto _result = proj::MultiSetupDialog::open(_multiSetup.get(),session_);
        auto&& _projectors = _multiSetup->projectors();

        int _numTunings = session_->tunings().size();

        switch (_result)
        {
        case MultiSetupDialog::Result::REPLACE:
          this->clear();
          _numTunings = 0; // Set number of tunings zero because it is not up-to-date anymore
        case MultiSetupDialog::Result::APPEND:
          for (auto& _projector : _projectors)
          {
            auto* _tuning = session_->tunings().add();
            if (!_tuning) return;
            _tuning->setColor(getTuningColor());
            _tuning->projector() = std::move(_projector);
            addTuning(_tuning);
            setTuningIndex(_numTunings);
          }

        case MultiSetupDialog::Result::CANCELLED:
        default:
          return;
        };

      }

      void TuningList::addTuning(omni::proj::Tuning* _tuning)
      {
        if (widgets_.size() >= maxNumberTunings())
        {
          QMessageBox::information(this,"Information", QString("You have reached the maximum of %1 projectors.").arg(maxNumberTunings()));
          return;
        }

        int _index = 0;
        for (auto& _sessionTuning : session_->tunings())
        {
          if (_sessionTuning.get() == _tuning)
          {
            break;
          }
          ++_index;
        }

        widgets_.emplace_back(new ui::proj::Tuning(_index,session_,this));
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
        static std::vector<QColor> _colors;

        if (_colors.empty())
        {
          _colors.reserve(maxNumberTunings());

          int _hue = 0;
          int _hueDiff = 120;
          for (int i = 0; i < maxNumberTunings(); ++i)
          {
            int _saturation = float(maxNumberTunings()/2 - i/2) / maxNumberTunings() * 2.0 * 255.0; //int(i * 2.0 / maxNumberTunings()) * 255;
            _colors.push_back(QColor::fromHsv(_hue,_saturation,255));

            _hue += 120;
            if (_hue >= 360)
            {
              _hueDiff /= 2;
              _hue += _hueDiff;
              _hue %= 360;
            }
          }
        }
        int _numTunings = session_->tunings().size();

        if (session_->tunings().size() > maxNumberTunings()) return QColor("#000000");

        /// Find first color in spectrum that is not already used
        for (int j = 0; j < maxNumberTunings(); ++j)
        {
          auto _color = _colors[j];
          bool _colorsEqual = false;

          for (int i = 0; i < _numTunings; ++i)
          {
            auto _tuningColor = session_->tunings()[i]->color();

            /// Compare R,G,B channels for both colors, alpha can be ignored
            _colorsEqual |= _color.red() == _tuningColor.red() &&
                _color.green() == _tuningColor.green() &&
                _color.blue() == _tuningColor.blue();
          }

          /// Return the first color which is not equal to an existing
          if (!_colorsEqual) return _color;
        }

        return _colors[_numTunings-1];
      }

      void TuningList::removeTuning(int _index)
      {
        if (_index < 0 || _index >= widgets_.size()) return;

        auto& _widget = widgets_[_index];
        contents_->layout()->removeWidget(_widget.get());
        _widget->setParent(nullptr);
        emit tuningToBeRemoved(_widget.get());
        _widget.reset();

        widgets_.erase(widgets_.begin() + _index);
        session_->tunings().remove(_index);

        // Re assign tuning indices to remaining widgets
        for (int i = 0; i < session_->tunings().size(); ++i)
        {
          widgets_[i]->setTuning(i,session_);
        }

        setTuningIndex(std::max(_index-1,0));

        emit projectorSetupChanged();
      }

      void TuningList::sessionModeChange()
      {
        for (auto& _widget : widgets_)
          _widget->sessionModeChange();
      }

      void TuningList::clear()
      {
        removeWidgets();
        session_->tunings().clear();

      }

      void TuningList::removeWidgets()
      {
        for (auto& _widget : widgets_)
        {
          contents_->layout()->removeWidget(_widget.get());
          _widget->setParent(nullptr);
        }
        widgets_.clear();
      }

      /// Updates/Repaints GL Views of all tunings widgets
      void TuningList::updateViews()
      {
        for (auto& _widget : widgets_)
          _widget->updateViews();
      }

      void TuningList::resizeEvent(QResizeEvent* event)
      {
        for (auto& _widget : widgets_)
          _widget->layout()->update();

        QScrollArea::resizeEvent(event);
      }

      void TuningList::keyPressEvent(QKeyEvent* _event) {
          if (_event->key() == Qt::Key_Up) {
              for (auto& _widget : widgets_) {
                  if (_widget->hasFocus()) continue;
                  _widget->focusPrev(false);
              }
          }
          if (_event->key() == Qt::Key_Down) {
              for (auto& _widget : widgets_) {
                  if (_widget->hasFocus()) continue;
                  _widget->focusNext(false);
              }
          }
      }

      bool TuningList::eventFilter(QObject *obj, QEvent *event) {
            if (event->type() == QEvent::KeyPress) {
                auto *keyEvent = static_cast<QKeyEvent*>(event);
                keyPressEvent(keyEvent);
            }
        return QWidget::eventFilter(obj,event);
      }

      void TuningList::setCurrentTuning()
      {
        if (!session_) return;

        int _index = 0;
        for (auto& _widget : widgets_)
        {
          if (_widget->isSelected())
          {
            setTuningIndex(_index);
            break;
          }
          ++_index;
        }
      }

      void TuningList::setTuningIndex(int _index)
      {
        int _oldIndex = session_->tunings().currentIndex();
        session_->tunings().setCurrentIndex(_index);
        if (_index != _oldIndex)
          emit currentIndexChanged(_index);
      }
    }
  }
}
