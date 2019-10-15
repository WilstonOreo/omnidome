/* Copyright (c) 2014-2015 "Omnidome" by Michael Winkelmann
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

#include <QVBoxLayout>
#include <QMessageBox>
#include <QKeyEvent>

#include <QDebug>

namespace omni {
    namespace ui {
        namespace proj {
            TuningList::TuningList(QWidget *_parent) :
                QScrollArea(_parent),
                layout_(new QVBoxLayout())
            {
                // Set contents and layout, so tuning widgets can be placed
                // vertically using
                // QScrollArea funcionality
                layout_->setSizeConstraint(QLayout::SetMinAndMaxSize);
                layout_->setSpacing(3);
                layout_->setContentsMargins(0, 0, 16, 0);

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

            Tuning * TuningList::widget(int _index) {
                return (_index >= 0) && (_index < widgets_.size()) ?
                       widgets_[_index].get() : nullptr;
            }

            Tuning const * TuningList::widget(int _index) const {
                return (_index >= 0) && (_index < widgets_.size()) ?
                       widgets_[_index].get() : nullptr;
            }

            Tuning* TuningList::widgetFromTuning(omni::proj::Tuning* _tuning) {
              for (auto& _widget : widgets_) {
                if (_widget->tuning() == _tuning) return _widget.get();
              }
              return nullptr;
            }

            void TuningList::dataToFrontend() {
                removeWidgets();

                for (auto& _tuning : dataModel()->tunings()) addTuning(
                        _tuning.get());

                sessionModeChange();
            }

            /// Return fullscreen and preview widget from index
            std::set<TuningGLView *>TuningList::getViews(int _index) const
            {
                std::set<TuningGLView *> _views;

                if ((_index < 0) || (_index >= widgets_.size())) return _views;

                auto *_widget = widgets_[_index].get();
                _views.insert(_widget->previewWidget());

                return _views;
            }

            void TuningList::addTuning()
            {
                omni::proj::Projector _projector;
                _projector.scale(dataModel()->scene().size());
                if (dataModel()->tunings().current())
                {
                    // Set setup id from last tuning
                    _projector =
                      dataModel()->tunings().current()->projector();
                }
                addTuning(_projector);
            }

            /// Add tuning with specific projector setup
            void TuningList::addTuning(Projector const& _proj)
            {
                auto *_tuning = dataModel()->tunings().add(false);

                if (!_tuning) return;

                _tuning->setColor(getTuningColor());
                _tuning->projector() = _proj;
                addTuning(_tuning);

                // Select this tuning index
                setTuningIndex(dataModel()->tunings().size() - 1);
            }

            void TuningList::addTuning(omni::proj::Tuning *_tuning)
            {
                int _index = 0;

                for (auto& _sessionTuning : dataModel()->tunings())
                {
                    if (_sessionTuning.get() == _tuning)
                    {
                        break;
                    }
                    ++_index;
                }

                widgets_.emplace_back(new ui::proj::Tuning(_index, dataModel(),
                                                           this));
                auto _widget = widgets_.back().get();
                contents_->layout()->addWidget(_widget);

                _widget->connect(_widget, SIGNAL(selected(
                                                     int)),    this,
                                 SLOT(setTuningIndex(int)));
                _widget->connect(_widget, SIGNAL(closed(
                                                     int)),    this,
                                 SLOT(removeTuning(int)));
                _widget->connect(_widget, SIGNAL(
                                     projectorSetupChanged()), this,
                                 SIGNAL(tuningChanged()));
                _widget->connect(_widget, SIGNAL(
                                     dataModelChanged()), this,
                                 SIGNAL(tuningChanged()));
                _widget->sessionModeChange();
                _widget->updateParameters();

                emit tuningAdded();
            }

            QColor TuningList::getTuningColor()
            {
                // Generate standard colors, for tuning
                static std::vector<QColor> _colors;

                constexpr int numColors = 16;

                if (_colors.empty())
                {
                    _colors.reserve(numColors);

                    int _hue     = 0;
                    int _hueDiff = 120;

                    for (int i = 0; i < numColors; ++i)
                    {
                        int _saturation = float(numColors / 2 - i / 2) /
                                          numColors * 2.0 * 255.0;
                        _colors.push_back(QColor::fromHsv(_hue, _saturation,
                                                          255));

                        _hue += 120;

                        if (_hue >= 360)
                        {
                            _hueDiff /= 2;
                            _hue     += _hueDiff;
                            _hue     %= 360;
                        }
                    }
                }
                int _numTunings = dataModel()->tunings().size();

                /// Find first color in spectrum that is not already used
                for (int j = 0; j < numColors; ++j)
                {
                    auto _color       = _colors[j];
                    bool _colorsEqual = false;

                    for (int i = 0; i < _numTunings; ++i)
                    {
                        auto _tuningColor = dataModel()->tunings()[i]->color();

                        /// Compare R,G,B channels for both colors, alpha can be
                        // ignored
                        _colorsEqual |= _color.red() == _tuningColor.red() &&
                                        _color.green() == _tuningColor.green() &&
                                        _color.blue() == _tuningColor.blue();
                    }

                    /// Return the first color which is not equal to an existing
                    if (!_colorsEqual) return _color;
                }

                return _colors[(_numTunings - 1) % numColors];
            }

            void TuningList::removeTuning(int _index)
            {
                if ((_index < 0) || (_index >= widgets_.size())) return;

                auto& _widget = widgets_[_index];
                contents_->layout()->removeWidget(_widget.get());
                _widget->setParent(nullptr);
                widgets_.erase(widgets_.begin() + _index);

                dataModel()->tunings().remove(_index);

                // Re assign tuning indices to remaining widgets
                for (int i = 0; i < dataModel()->tunings().size(); ++i)
                {
                    dataModel()->tunings()[i]->makeVisualizer();
                    widgets_[i]->setIndex(i);
                    widgets_[i]->setDataModel(dataModel());
                }
                setTuningIndex(std::max(_index - 1, 0));

                emit tuningRemoved();
                emit dataModelChanged();
            }

            void TuningList::sessionModeChange()
            {
                if (!dataModel()) return;

                auto _currentIndex = dataModel()->tunings().currentIndex();

                for (auto& _widget : widgets_) {
                    _widget->sessionModeChange();
                }

                if ((_currentIndex >= 0) && (_currentIndex < widgets_.size())) {
                    widgets_[_currentIndex]->setFocus();
                }
            }

            void TuningList::clear()
            {
                removeWidgets();
                if (dataModel()) {
                  dataModel()->tunings().clear();
                }
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
                for (auto& _widget : widgets_) _widget->updateViews();
            }

            /// Adjust sliders to scene scale
            void TuningList::updateSceneSize(bool _rescaleValues) {
                if (!dataModel() || signalsBlocked()) return;

                for (auto& _widget : widgets_) {
                    _widget->setRescaleValues(_rescaleValues);
                    _widget->setScale(dataModel()->scene().size());
                }
            }

            void TuningList::updateUnits() {
                if (!dataModel() || signalsBlocked()) return;

                for (auto& _widget : widgets_) {
                    _widget->setUnit(dataModel()->scene().unit().abbreviation());
                }
            }

            /// Disable all tunings and fullscreen
            void TuningList::disableAllTunings() {
              for (auto& _widget : widgets_) {
                _widget->fullscreenToggle(false);
              }
            }

            /// Enable all tunings and fullscreen
            void TuningList::enableAllTunings() {
              for (auto& _widget : widgets_) {
                _widget->fullscreenToggle(true);
              }
            }

            /// Enable selected tuning and show only this in fullscreen
            void TuningList::enableSelectedTuningOnly() {
              for (auto& _widget : widgets_) {
                _widget->fullscreenToggle(_widget->index() == dataModel()->tunings().currentIndex());
              }
            }

            void TuningList::resizeEvent(QResizeEvent *event)
            {
                for (auto& _widget : widgets_) _widget->layout()->update();

                QScrollArea::resizeEvent(event);
            }

            void TuningList::keyPressEvent(QKeyEvent *_event) {
                int   _tuningIndex = dataModel()->tunings().currentIndex();
                auto *_widget      = widget(_tuningIndex);

                if (!_widget) return;

                if (_widget->hasFocus() && !_widget->isSelected()) return;

                if (_event->key() == Qt::Key_Up) {
                    if (!_widget->focusPrev(false) && (_tuningIndex > 0)) {
                        setTuningIndex(_tuningIndex - 1);
                        widget(_tuningIndex - 1)->focusLast();
                    }
                }

                if (_event->key() == Qt::Key_Down) {
                    if (!_widget->focusNext(false) &&
                        (_tuningIndex < dataModel()->tunings().size() - 1)) {
                        setTuningIndex(_tuningIndex + 1);
                        widget(_tuningIndex + 1)->focusFirst();
                    }
                }
            }

            bool TuningList::eventFilter(QObject *obj, QEvent *event) {
                if (event->type() == QEvent::KeyPress) {
                    auto *keyEvent = static_cast<QKeyEvent *>(event);
                    keyPressEvent(keyEvent);
                }
                return QWidget::eventFilter(obj, event);
            }

            void TuningList::setCurrentTuning()
            {
                if (!dataModel()) return;

                int _index = 0;

                for (auto& _widget : widgets_)
                {
                    if (_widget->isSelected())
                    {
                        setTuningIndex(_index);
                    } else {
                        _widget->setSelected(false);
                    }
                    ++_index;
                }
            }

            void TuningList::setTuningIndex(int _index)
            {
                int _oldIndex = dataModel()->tunings().currentIndex();

                dataModel()->tunings().setCurrentIndex(_index);

                for (auto& _widget : widgets_) {
                    if ((_widget->index() != _index) && _widget->isSelected()) {
                        _widget->setSelected(false);
                    }
                }

                if (_index != _oldIndex) emit currentIndexChanged(_index);

                emit dataModelChanged();
            }
        }
    }
}
