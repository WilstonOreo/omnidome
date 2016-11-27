/* Copyright (c) 2014-2016 "Omnidome" by Michael Winkelmann
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

#include "ToolBar.h"

#include <QIcon>
#include <QPixmap>
#include <QAction>
#include <QToolButton>
#include <QSizePolicy>
#include <QDebug>

#include <omni/util.h>
#include "About.h"

namespace omni {
  namespace ui {
    ToolBar::ToolBar(QWidget *_widget) :
      QToolBar(_widget) {
      setStyleSheet("QToolBar {"
                    "border-bottom: 0px solid #080808 ;"
                    "}"
                    );

      setFloatable(false);
      setMovable(false);
      setIconSize(QSize(28, 28));

      auto _makeButton = [&](QString const& _iconId,
                             QString const& _caption = QString(),
                             int _iconSize = 24) ->
                         QToolButton * {
                           auto *_button = new QToolButton();

                           _button->setSizePolicy(QSizePolicy::Maximum,
                                                  QSizePolicy::Expanding);
                           _button->setMinimumSize(QSize(120, 40));
                           _button->setCheckable(true);
                           _button->setText(_caption);
                           _button->setToolButtonStyle(
                             Qt::ToolButtonTextBesideIcon);
                           _button->setIconSize(QSize(_iconSize, _iconSize));

                           auto _makeIconFile =
                             [&](QString const& _s,
                                 QString const& _suffix = QString()) {
                               return QString(":/icons/") +
                                      _s + _suffix + ".png";
                             };

                           QIcon _icon(_makeIconFile(_iconId));
                           _icon.addPixmap(QPixmap(_makeIconFile(_iconId,
                                                                 "_disabled")), QIcon::Disabled,
                                           QIcon::On);
                           _icon.addPixmap(QPixmap(_makeIconFile(_iconId,
                                                                 "_disabled")), QIcon::Disabled,
                                           QIcon::Off);
                           _button->setIcon(_icon);

                           _button->setStyleSheet(
                             "QToolButton {"
                             " border: 0px solid #080808; "
                             " margin: 4px 4px 4px 0px; "
                             "}"
                             "QToolButton:checked {"
                             " border: 2.5px solid #5e5e5e ;"
                             " background-color: #5e5e5e ;"
                             " color : #f8f8f8 ; "
                             "}"
                             );

                           return _button;
                         };

      auto _makeAction =
        [&](QToolButton *_btn, QString const& _shortCut = QString()) {
          addSeparator();
          auto *_action = this->addWidget(_btn);
          this->connect(_action, SIGNAL(
                          triggered()), _btn, SIGNAL(clicked()));
          _action->setShortcut(QKeySequence(_shortCut));
          return _action;
        };

      btnSettings_ = _makeButton("logo", "O M N I D O M E", 40);
      btnSettings_->setStyleSheet(
        "QToolButton {"
        " border: 0px solid #080808; "
        " margin: 4px 4px 4px 0px; "
        " font-size: 16px; "
        "}"
        "QToolButton:checked {"
        " border: 2.5px solid #5e5e5e ;"
        " background-color: #5e5e5e ;"
        " color : #f8f8f8 ; "
        "}");
      btnSettings_->setMinimumSize(QSize(160, 40));
      btnSettings_->setCheckable(false);
      btnSettings_->setIconSize(QSize(48, 48));
      btnSettings_->setToolTip("About Omnidome");
      connect(btnSettings_, SIGNAL(clicked()), this, SLOT(showAbout()));
      auto *_actionSettings = addWidget(btnSettings_);
      this->connect(_actionSettings, SIGNAL(triggered()), btnSettings_,
                    SIGNAL(clicked()));
      _actionSettings->setShortcut(QKeySequence("Ctrl+F1"));

      QWidget *empty = new QWidget();
      empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
      addWidget(empty);

      btnScreenSetup_ = _makeButton("screens", "SCREEN SETUP");
      btnScreenSetup_->setToolTip("Setup projector screens (Ctrl + 1)");
      connect(btnScreenSetup_, SIGNAL(clicked()), this,
              SLOT(setScreenSetupMode()));
      _makeAction(btnScreenSetup_, "Ctrl+1");

      btnArrange_ = _makeButton("arrange", "ARRANGE");
      btnArrange_->setToolTip("Arrange projectors, mapping and canvas (Ctrl + 2)");
      connect(btnArrange_, SIGNAL(clicked()), this, SLOT(setArrangeMode()));
      _makeAction(btnArrange_, "Ctrl+2");

      btnWarp_ = _makeButton("warp", "WARP");
      btnWarp_->setToolTip("Edit warp grid (Ctrl + 3)");
      connect(btnWarp_, SIGNAL(clicked()), this, SLOT(setWarpMode()));
      _makeAction(btnWarp_, "Ctrl+3");

      btnBlend_ = _makeButton("blend", "BLEND");
      btnBlend_->setToolTip("Edit blend mask with blend brush (Ctrl + 4)");
      connect(btnBlend_, SIGNAL(clicked()), this, SLOT(setBlendMode()));
      _makeAction(btnBlend_, "Ctrl+4");

      btnColorCorrection_ = _makeButton("color_correction", "COLOR CORRECTION");
      btnColorCorrection_->setToolTip(
        "Color Correction with brightness, gamma and contrast (Ctrl + 5)");
      connect(btnColorCorrection_, SIGNAL(clicked()), this,
              SLOT(setColorCorrectionMode()));
      _makeAction(btnColorCorrection_, "Ctrl+5");

      btnExport_ = _makeButton("export", "EXPORT");
      btnExport_->setToolTip("Export calibration data (Ctrl + 6)");
      connect(btnExport_, SIGNAL(clicked()), this, SLOT(setExportMode()));
      _makeAction(btnExport_, "Ctrl+6");

      btnLive_ = _makeButton("live", "LIVE");
      btnLive_->setToolTip("Play input live (Ctrl + 7)");
      connect(btnLive_, SIGNAL(clicked()), this, SLOT(setLiveMode()));
      _makeAction(btnLive_, "Ctrl+7");

      connect(this, &ToolBar::dataModelChanged, this, &ToolBar::updateFrontend);
    }

    ToolBar::~ToolBar() {}

    void ToolBar::setScreenSetupMode() {
      setMode(Session::Mode::SCREENSETUP);
    }

    void ToolBar::setArrangeMode() {
      setMode(Session::Mode::ARRANGE);
    }

    void ToolBar::setWarpMode() {
      setMode(Session::Mode::WARP);
    }

    void ToolBar::setBlendMode() {
      setMode(Session::Mode::BLEND);
    }

    void ToolBar::setColorCorrectionMode() {
      setMode(Session::Mode::COLORCORRECTION);
    }

    void ToolBar::setExportMode() {
      setMode(Session::Mode::EXPORT);
    }

    void ToolBar::setLiveMode() {
      setMode(Session::Mode::LIVE);
    }

    void ToolBar::setMode(Session::Mode _mode) {
      if (!dataModel()) return;

      dataModel()->setMode(_mode);
      emit dataModelChanged();
    }

    void ToolBar::dataToFrontend() {
      auto _mode = dataModel()->mode();

      btnScreenSetup_->setChecked(_mode == Session::Mode::SCREENSETUP);
      btnArrange_->setChecked(_mode == Session::Mode::ARRANGE);
      btnWarp_->setChecked(_mode == Session::Mode::WARP);
      btnBlend_->setChecked(_mode == Session::Mode::BLEND);
      btnColorCorrection_->setChecked(_mode == Session::Mode::COLORCORRECTION);
      btnExport_->setChecked(_mode == Session::Mode::EXPORT);
      btnLive_->setChecked(_mode == Session::Mode::LIVE);
      buttonStates();
    }

    void ToolBar::buttonStates() {
      if (!dataModel()) return;
      bool _hasOutput = dataModel()->hasOutput();

      btnWarp_->setEnabled(_hasOutput);
      btnBlend_->setEnabled(_hasOutput);
      btnColorCorrection_->setEnabled(_hasOutput);
      btnExport_->setEnabled(_hasOutput);
      btnLive_->setEnabled(_hasOutput);
    }

    void ToolBar::showAbout()
    {
      std::unique_ptr<About> _about(new About());

      _about->exec();
    }
  }
}
