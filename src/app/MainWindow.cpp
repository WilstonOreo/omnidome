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

#include "MainWindow.h"

#include "ui_omni_ui_MainWindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCloseEvent>
#include <QMenu>
#include <QIcon>

#include <omni/Session.h>
#include <omni/proj/MultiSetup.h>
#include <omni/ui/GLView3D.h>
#include <omni/ui/TuningGLView.h>

#include "About.h"

#include "proj/Tuning.h"
#include "proj/TuningList.h"
#include "proj/MultiSetupDialog.h"

#include "Arrange.h"
#include "ScreenSetup.h"
#include "Export.h"
#include "Mapping.h"
#include "Input.h"

using namespace omni::ui;

MainWindow::MainWindow(QMainWindow *parent) :
    QMainWindow(parent),
    modified_(false),
    ui_(new Ui::MainWindow)
{
    session_.reset(new Session());
    ui_->setupUi(this);

    // Make and setup pages
    {
        ui_->tabs->setIconSize(QSize(32,32));
        QLayout *_layout = new QHBoxLayout();
        screenSetup_.reset(new ScreenSetup(this));
        _layout->addWidget(screenSetup_.get());
        ui_->tabs->addTab(QIcon(":/icons/screens.png"),"SCREEN SETUP");

        arrange_.reset(new Arrange(this));
        _layout->addWidget(arrange_.get());
        ui_->tabs->addTab(QIcon(":/icons/arrange.png"), "ARRANGE");

        warp_.reset(new TuningGLView(this));
        _layout->addWidget(warp_.get());
        warp_->setBorder(0.5);
        warp_->setKeepAspectRatio(true);
        ui_->tabs->addTab(QIcon(":/icons/warp.png"),"WARP");

        blend_.reset(new TuningGLView(this));
        _layout->addWidget(blend_.get());
        blend_->setBorder(0.5);
        blend_->setKeepAspectRatio(true);
        ui_->tabs->addTab(QIcon(":/icons/blend.png"),"BLEND");

        colorCorrection_.reset(new TuningGLView(this));
        _layout->addWidget(colorCorrection_.get());
        colorCorrection_->setBorder(0.5);
        colorCorrection_->setKeepAspectRatio(true);
        ui_->tabs->addTab(QIcon(":/icons/color_correction.png"),"COLOR CORRECTION");

        export_.reset(new Export(session_.get()));
        _layout->addWidget(export_.get());
        ui_->pages->setLayout(_layout);
        ui_->tabs->addTab(QIcon(":/icons/export.png"),"EXPORT");

        live_.reset(new GLView3D(this));
        _layout->addWidget(live_.get());
        ui_->tabs->addTab(QIcon(":/icons/live.png"),"LIVE");

        _layout->setContentsMargins(0,0,0,0);
        ui_->pages->setLayout(_layout);

        connect(ui_->tabs,SIGNAL(currentChanged(int)),this,SLOT(setMode(int)));
    }

    ui_->statusbar->showMessage(QString("Omnidome v") +
                                QString(OMNIDOME_VERSION_STRING));


    // Setup add projector template menu
    {
        QMenu *_menu = new QMenu();

        _menu->addAction("PeripheralSetup")->setData(QString("PeripheralSetup"));
        _menu->addAction("FreeSetup")->setData(QString("FreeSetup"));
        _menu->addSeparator();

        for (auto& _idMultiSetup : omni::proj::MultiSetupFactory::classes())
        {
            auto& _id     = _idMultiSetup.first;
            auto *_action = _menu->addAction(_id);
            _action->setData(QString(_id));
        }

        connect(_menu, SIGNAL(triggered(QAction *)), this,
                SLOT(addProjector(QAction *)));

        ui_->btnAddTuning->setMenu(_menu);
    }

    /// Set dock widget tabs
    {
        tabifyDockWidget(ui_->dockInputs, ui_->dockMapping);
        tabifyDockWidget(ui_->dockInputs, ui_->dockColorCorrection);
    }

    // Connect signals and slots
    {
        // Connect projector position change with view update
        connect(ui_->tuningList, SIGNAL(projectorSetupChanged()), this,
                SLOT(modified()));
        connect(ui_->tuningList, SIGNAL(currentIndexChanged(int)), this,
                SLOT(modified()));

        // Detach tuning from screen setup when it was removed
        connect(ui_->tuningList,
                SIGNAL(tuningToBeRemoved(omni::ui::proj::Tuning *)),
                screenSetup_.get(),
                SLOT(detachTuning(omni::ui::proj::Tuning *)));

        // Connect canvas parameter change with view update
        connect(ui_->dockCanvasWidget, SIGNAL(
                    canvasChanged()),                     this,
                SLOT(modified()));
        connect(ui_->dockCanvasWidget, SIGNAL(canvasTypeChanged()),
                ui_->dockMappingWidget, SLOT(setDefaultMappingForCanvas()));
        connect(ui_->dockCanvasWidget, SIGNAL(displayInputToggled(
                                                  bool)), arrange_.get(),
                SLOT(setDisplayInput(bool)));
        connect(ui_->dockCanvasWidget, SIGNAL(displayInputToggled(
                                                  bool)), arrange_.get(),
                SLOT(setDisplayInput(bool)));
        connect(ui_->dockCanvasWidget,
                SIGNAL(projectorViewModeChanged(
                           ProjectorViewMode)),           arrange_.get(),
                SLOT(setProjectorViewMode(ProjectorViewMode)));

        // Update all views when input has changed
        connect(ui_->dockInputsWidget, SIGNAL(
                    inputChanged()),                      this,
                SLOT(modified()));
        connect(ui_->dockInputsWidget, SIGNAL(
                    inputIndexChanged()),                 this,
                SLOT(modified()));

        // Update all views when mapping mode has changed
        connect(ui_->dockMappingWidget, SIGNAL(
                    mappingChanged()),                    this,
                SLOT(modified()));
        connect(ui_->dockMappingWidget, SIGNAL(
                    mappingTypeChanged()),                this,
                SLOT(modified()));

        // Update all views when warp grid has changed
        connect(ui_->dockWarpWidget, SIGNAL(
                    warpGridChanged()),                   this,
                SLOT(modified()));

        // Update all views when blend mask has changed
        connect(ui_->dockBlendWidget, SIGNAL(
                    blendMaskChanged()),                  this,
                SLOT(modified()));

        // Update all views when color correction has changed
        connect(ui_->dockColorCorrectionWidget, SIGNAL(
                    colorCorrectionChanged()),                  this,
                SLOT(modified()));

        // Connect tuning index of tuning list for warp, blend and color correction
        connect(ui_->tuningList, SIGNAL(currentIndexChanged(
                                            int)),        this,
                SLOT(setTuningIndex(int)));

        // Connect add tuning button with tuning list
        connect(ui_->btnAddTuning, SIGNAL(
                    clicked()),                           ui_->tuningList,
                SLOT(addTuning()));
        connect(ui_->btnAddTuning, SIGNAL(
                    clicked()),                           this,
                SLOT(buttonState()));
    }

    setupSession();
    raise();
    show();
}

MainWindow::~MainWindow()
{}

void MainWindow::setupSession()
{
    using util::enumToInt;
    qDebug() << "setupSession: " << enumToInt(session_->mode());
    locked_ = true;
    {
        // Set session to pages
        arrange_->setSession(session_.get());
        live_->setSession(session_.get());

        warp_->setSession(session_.get());
        blend_->setSession(session_.get());
        colorCorrection_->setSession(session_.get());
        export_->setSession(session_.get());

        ui_->tuningList->setSession(session_.get());
        ui_->dockMappingWidget->setSession(session_.get());
        ui_->dockCanvasWidget->setSession(session_.get());
        ui_->dockInputsWidget->setSession(session_.get());
        ui_->dockWarpWidget->setSession(session_.get());
        ui_->dockBlendWidget->setSession(session_.get());
        ui_->dockColorCorrectionWidget->setTuning(session_->tunings().current());
    }
    locked_ = false;

    qDebug() << "setupSession: " << enumToInt(session_->mode());
    setMode(session_->mode());
    qDebug() << "setupSession: " << enumToInt(session_->mode());
}

void MainWindow::newProjection()
{
    if (saveChangesPrompt() == QMessageBox::Cancel)
    {
        return;
    }

    session_.reset(new Session());
    setupSession();
    modified_ = false;
    buttonState();
}

void MainWindow::saveProjection()
{
    if (filename_.isEmpty())
    {
        saveProjectionAs();
    }
    else
    {
        session_->save(filename_);
        modified_ = false;
        buttonState();
    }
}

void MainWindow::saveProjectionAs()
{
    QString _filename = QFileDialog::getSaveFileName(this,
                                                     "Save omni Projection...",
                                                     ".",
                                                     "omni Projections (*.omni)");

    if (!_filename.isEmpty())
    {
        filename_ = _filename;
        session_->save(filename_);
        modified_ = false;
        buttonState();
    }
}

void MainWindow::openProjection()
{
    if (saveChangesPrompt() == QMessageBox::Cancel)
    {
        return;
    }

    QString _filename = QFileDialog::getOpenFileName(this,
                                                     "Open omni Projection...",
                                                     ".",
                                                     "omni Projections (*.omni)");

    if (!_filename.isEmpty())
    {
        openProjection(_filename);
    }
}

bool MainWindow::openProjection(const QString& _filename)
{
    filename_ = _filename;
    session_.reset(new Session());
    session_->load(filename_);
    setupSession();
    modified_ = false;
    filename_ = _filename;
    return true;
}

void MainWindow::editAsNew()
{
    if (!filename_.isEmpty() || modified_)
    {
        if (saveChangesPrompt() == QMessageBox::Cancel)
        {
            return;
        }
    }

    filename_ = "";
    modified_ = false;
    buttonState();
}

void MainWindow::showSettings()
{
    std::unique_ptr<About> _about(new About());

    _about->exec();
}

void MainWindow::updateAllViews()
{
    arrange_->update();
    warp_->update();
    blend_->update();
    colorCorrection_->update();
    live_->update();
    ui_->tuningList->updateViews();
}

/// Sets modified flag to true
void MainWindow::modified()
{
    if (locked_) return;

    modified_ = true;

    if ((session_->mode() != Session::Mode::WARP) &&
        (session_->mode() != Session::Mode::BLEND)) ui_->dockMapping->setVisible(
            session_->inputs().current() != nullptr &&
            session_->mode() !=
            Session::Mode::SCREENSETUP);
    updateAllViews();
    buttonState();
}

void MainWindow::showEvent(QShowEvent *event)
{}

void MainWindow::closeEvent(QCloseEvent *_event)
{
    if (saveChangesPrompt() == QMessageBox::Cancel)
    {
        _event->ignore();
        return;
    }


      // Delete screen setup manually, so all fullscreen widgets are free'd too
      screenSetup_->hide();
      screenSetup_->setParent(nullptr);
      screenSetup_.reset();
}

void MainWindow::buttonState()
{
    QString _str("Omnidome");

    if (!filename_.isEmpty())
    {
        _str += " - " + filename_;
    }

    if (modified_) _str += "*";
    setWindowTitle(_str);

    ui_->actionEditAsNew->setEnabled(!filename_.isEmpty());
    ui_->actionSave->setEnabled(modified_);
}

/// Set current tuning index
void MainWindow::setTuningIndex(int _index)
{
    ui_->tuningList->setTuningIndex(_index);
    warp_->setTuningIndex(_index);
    blend_->setTuningIndex(_index);
    colorCorrection_->setTuningIndex(_index);
    warp_->setChildViews(ui_->tuningList->getViews(_index));
    blend_->setChildViews(ui_->tuningList->getViews(_index));
    colorCorrection_->setChildViews(ui_->tuningList->getViews(_index));

    ui_->dockColorCorrectionWidget->setTuning(session_->tunings().current());
    ui_->dockBlendWidget->updateBlendMask();
    ui_->dockWarpWidget->updateWarpGrid();
}

void MainWindow::addProjector(QAction *_action)
{
    auto _id = _action->data().toString();

    /// Check for single projector setups
    for (auto& _idSetup : omni::proj::SetupFactory::classes())
    {
        if (_idSetup.first.str() == _id)
        {
            ui_->tuningList->addTuning(_id);
            return;
        }
    }

    /// Check for multi projector setups
    for (auto& _idMultiSetup : omni::proj::MultiSetupFactory::classes())
    {
        if (_idMultiSetup.first.str() == _id)
        {
            ui_->tuningList->addMultiSetup(_id);
            return;
        }
    }
}

void MainWindow::setMode(int _index) {
    setMode(util::intToEnum<Session::Mode>(_index));
}

void MainWindow::setMode(Session::Mode _mode)
{
    session_->setMode(_mode);

    bool _hasTunings = session_->tunings().size() > 0;

    // Select first tuning if there is no tuning selected yet
    if (_hasTunings && (session_->tunings().currentIndex() < 0))
    {
        setTuningIndex(0);
    }

    screenSetup_->setVisible(_mode == Session::Mode::SCREENSETUP);
    arrange_->setVisible(_mode == Session::Mode::ARRANGE);
    warp_->setVisible(_mode == Session::Mode::WARP && _hasTunings);
    blend_->setVisible(_mode == Session::Mode::BLEND && _hasTunings);
    colorCorrection_->setVisible(_mode == Session::Mode::COLORCORRECTION && _hasTunings);
    export_->setVisible(_mode == Session::Mode::EXPORT && _hasTunings);
    live_->setVisible(_mode == Session::Mode::LIVE && _hasTunings);

    using omni::ui::proj::Tuning;

    ui_->btnAddTuning->setVisible(
        _mode == Session::Mode::SCREENSETUP ||
        _mode == Session::Mode::ARRANGE);
    ui_->dockCanvas->setVisible(_mode == Session::Mode::ARRANGE);
    ui_->dockWarp->setVisible(_mode == Session::Mode::WARP);
    ui_->dockBlend->setVisible(_mode == Session::Mode::BLEND);
    ui_->dockColorCorrection->setVisible(_mode == Session::Mode::COLORCORRECTION);
    ui_->dockScene->setVisible(_mode == Session::Mode::ARRANGE ||
                        _mode == Session::Mode::LIVE);

    switch (_mode)
    {
    case Session::Mode::SCREENSETUP:
        ui_->dockMapping->hide();
        ui_->dockInputs->show();
        break;

    case Session::Mode::ARRANGE:
        ui_->dockInputs->show();
        ui_->dockMapping->setVisible(session_->inputs().current());
        break;

    case Session::Mode::WARP:
        ui_->dockInputs->show();
        ui_->dockMapping->setVisible(session_->inputs().current());
        break;

    case Session::Mode::BLEND:
        ui_->dockInputs->setVisible(true);
        ui_->dockMapping->setVisible(false);
        break;
    case Session::Mode::COLORCORRECTION:
        ui_->dockInputs->setVisible(true);
        ui_->dockMapping->setVisible(true);
        break;
    case Session::Mode::EXPORT:
        ui_->dockInputs->show();
        ui_->dockMapping->setVisible(session_->inputs().current());
        break;

    case Session::Mode::LIVE:
        ui_->dockInputs->show();
        ui_->dockMapping->hide();
    default:
        break;
    }

    ui_->tuningList->sessionModeChange();
    updateAllViews();
    buttonState();
}

QMessageBox::StandardButton MainWindow::saveChangesPrompt()
{
    if (modified_)
    {
        QMessageBox::StandardButton _reply = QMessageBox::question(this,
                                                                   "Screen update",
                                                                   "There are unsaved modifications. Do you want to save them?",
                                                                   QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                                                                   QMessageBox::Yes);

        if (_reply == QMessageBox::Yes) saveProjection();
        return _reply;
    }
    return QMessageBox::No;
}
