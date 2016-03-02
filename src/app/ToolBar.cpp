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
#include <QPushButton>
#include <QSizePolicy>
#include <QDebug>

#include <omni/util.h>

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
            setIconSize(QSize(32,32));

            auto _addButton = [&](
                Session::Mode _mode)
                -> QAction* {

                auto* _button = new QPushButton();
                _button->setCheckable(true);
                _button->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
                _button->setMinimumSize(QSize(120,40));

                _button->setStyleSheet(
                    "QPushButton {"
                    " border: 0px solid #080808; "
                    " margin: 4px 4px 4px 0px; "
                    "}"
                    "QPushButton:checked {"
                    " border: 2.5px solid #5e5e5e ;"
                    " background-color: #5e5e5e ;"
                    " color : #f8f8f8 ; "
                    "}"
                );
                QString  _iconFile;
                QString  _caption;

                switch (_mode) {
                    default:
                    case Session::Mode::SCREENSETUP:
                    btnScreenSetup_ = _button;
                    _iconFile = ":/icons/screens.png";
                    _caption = "SCREEN SETUP";
                    connect(_button,SIGNAL(clicked()),this,SLOT(setScreenSetupMode()));
                    break;
                    case Session::Mode::ARRANGE:
                    btnArrange_ = _button;
                    _iconFile = ":/icons/arrange.png";
                    _caption = "ARRANGE";
                    connect(_button,SIGNAL(clicked()),this,SLOT(setArrangeMode()));
                    break;
                    case Session::Mode::WARP:
                    btnWarp_ = _button;
                    _iconFile = ":/icons/warp.png";
                    _caption = "WARP";
                    connect(_button,SIGNAL(clicked()),this,SLOT(setWarpMode()));
                    break;
                    case Session::Mode::BLEND:
                    btnBlend_ = _button;
                    _iconFile = ":/icons/blend.png";
                    _caption = "BLEND";
                    connect(_button,SIGNAL(clicked()),this,SLOT(setBlendMode()));
                    break;
                    case Session::Mode::COLORCORRECTION:
                    btnColorCorrection_ = _button;
                    _iconFile = ":/icons/color_correction.png";
                    _caption = "COLOR CORRECTION";
                    connect(_button,SIGNAL(clicked()),this,SLOT(setColorCorrectionMode()));
                    break;
                    case Session::Mode::EXPORT:
                    btnExport_ = _button;
                    _iconFile = ":/icons/export.png";
                    _caption = "EXPORT";
                    connect(_button,SIGNAL(clicked()),this,SLOT(setExportMode()));
                    break;
                    case Session::Mode::LIVE:
                    btnLive_ = _button;
                    _iconFile = ":/icons/live.png";
                    _caption = "LIVE";
                    connect(_button,SIGNAL(clicked()),this,SLOT(setLiveMode()));
                    break;
                }

                addSeparator();
                _button->setText(_caption);
                _button->setIcon(QIcon(_iconFile));

                return this->addWidget(_button);
            };
            QWidget* empty = new QWidget();
            empty->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
            addWidget(empty);

            for (int i = 0; i < util::enumToInt(Session::Mode::NUM_MODES); ++i) {
                _addButton(util::intToEnum<Session::Mode>(i));
            }
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

            emit sessionModeChanged(_mode);
        }

        void ToolBar::sessionParameters() {
            setMode(session()->mode());
        }
    }
}
