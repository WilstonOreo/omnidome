/* Copyright (c) 2014-2016 "Omnidome" by cr8tr
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
#include <QToolButton>
#include <QSizePolicy>
#include <QDebug>

#include <omni/util.h>
#include "About.h"

namespace omni {
    namespace ui {
        ToolBar::ToolBar(QWidget* _widget) :
            QToolBar(_widget) {

            setStyleSheet("QToolBar {"
              "border-bottom: 0px solid #080808 ;"
              "}"
            );

            setFloatable(false);
            setMovable(false);
            setIconSize(QSize(28,28));

            auto _makeButton = [&](QString const& _iconId,
                    QString const& _caption = QString(), int _iconSize = 24 ) ->
                QToolButton* {

                auto* _button = new QToolButton();
                _button->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
                _button->setMinimumSize(QSize(120,40));
                _button->setCheckable(true);
                _button->setText(_caption);
                _button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
                _button->setIconSize(QSize(_iconSize,_iconSize));

                auto _makeIconFile = [&](QString const& _s, QString const& _suffix = QString()) {

                    return QString(":/icons/") + _s + _suffix + ".png";
                };

                QIcon _icon(_makeIconFile(_iconId));
                _icon.addPixmap(QPixmap(_makeIconFile(_iconId,"_disabled")),QIcon::Disabled,QIcon::On);
                _icon.addPixmap(QPixmap(_makeIconFile(_iconId,"_disabled")),QIcon::Disabled,QIcon::Off);
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

            auto _makeAction = [&](QToolButton* _btn) {
                addSeparator();
                return this->addWidget(_btn);
            };

            btnSettings_ = _makeButton("logo","O M N I D O M E",40);
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
            btnSettings_->setMinimumSize(QSize(160,40));
            btnSettings_->setCheckable(false);
            btnSettings_->setIconSize(QSize(48,48));
            connect(btnSettings_,SIGNAL(clicked()),this,SLOT(showSettings()));
            addWidget(btnSettings_);

            QWidget* empty = new QWidget();
            empty->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
            addWidget(empty);

            btnScreenSetup_ = _makeButton("screens","SCREEN SETUP");
            connect(btnScreenSetup_,SIGNAL(clicked()),this,SLOT(setScreenSetupMode()));
            _makeAction(btnScreenSetup_);

            btnArrange_ = _makeButton("arrange","ARRANGE");
            connect(btnArrange_,SIGNAL(clicked()),this,SLOT(setArrangeMode()));
            _makeAction(btnArrange_);

            btnWarp_ = _makeButton("warp","WARP");
            connect(btnWarp_,SIGNAL(clicked()),this,SLOT(setWarpMode()));
            _makeAction(btnWarp_);

            btnBlend_ = _makeButton("blend","BLEND");
            connect(btnBlend_,SIGNAL(clicked()),this,SLOT(setBlendMode()));
            _makeAction(btnBlend_);

            btnColorCorrection_ = _makeButton("color_correction","COLOR CORRECTION");
            connect(btnColorCorrection_,SIGNAL(clicked()),this,SLOT(setColorCorrectionMode()));
            _makeAction(btnColorCorrection_);

            btnExport_ = _makeButton("export","EXPORT");
            connect(btnExport_,SIGNAL(clicked()),this,SLOT(setExportMode()));
            _makeAction(btnExport_);

            btnLive_ = _makeButton("live","LIVE");
            connect(btnLive_,SIGNAL(clicked()),this,SLOT(setLiveMode()));
            _makeAction(btnLive_);
        }

        ToolBar::~ToolBar() {

        }

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
            if (!session()) return;

            this->locked([&]{
                btnScreenSetup_->setChecked(_mode == Session::Mode::SCREENSETUP);
                btnArrange_->setChecked(_mode == Session::Mode::ARRANGE);
                btnWarp_->setChecked(_mode == Session::Mode::WARP);
                btnBlend_->setChecked(_mode == Session::Mode::BLEND);
                btnColorCorrection_->setChecked(_mode == Session::Mode::COLORCORRECTION);
                btnExport_->setChecked(_mode == Session::Mode::EXPORT);
                btnLive_->setChecked(_mode == Session::Mode::LIVE);
            });
            buttonState();

            emit sessionModeChanged(_mode);
        }

        void ToolBar::buttonState() {

            bool _hasTunings = session()->tunings().size() > 0;
            bool _hasInput = session()->inputs().current() != nullptr;

            this->locked([&]{
                btnWarp_->setEnabled(_hasTunings && _hasInput);
                btnBlend_->setEnabled(_hasTunings && _hasInput);
                btnColorCorrection_->setEnabled(_hasTunings && _hasInput);
                btnExport_->setEnabled(_hasTunings && _hasInput);
                btnLive_->setEnabled(_hasTunings && _hasInput);
            });
        }

        void ToolBar::sessionParameters() {
            setMode(session()->mode());
        }

        void ToolBar::showSettings()
{
    std::unique_ptr<About> _about(new About());
    _about->exec();
}
    }
}
