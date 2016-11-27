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

#include "RecentSessions.h"

#include <QFileInfo>

#include "Application.h"
#include <omni/util.h>

namespace omni {
  namespace ui {
    RecentFileAction::RecentFileAction(QString const& _file, QObject *_parent) :
      QAction(_file, _parent),
      file_(_file)
    {
      connect(this, SIGNAL(triggered()), this, SLOT(loadSession()));
      setState();
    }

    RecentFileAction::~RecentFileAction() {}

    void RecentFileAction::loadSession() {
      setState();
      emit fileToBeLoaded(file_);
    }

    bool RecentFileAction::fileExists() const {
      return util::fileExists(file_);
    }

    void RecentFileAction::setState() {
      setEnabled(fileExists());
    }

    RecentSessions::RecentSessions(QObject *_parent) : QObject(_parent) {
      generateMenu();
    }

    RecentSessions::~RecentSessions() {}


    void RecentSessions::setNumberOfFiles(int _numberOfFiles) {
      numberOfFiles_ = _numberOfFiles;
    }

    int RecentSessions::numberOfFiles() const {
      return numberOfFiles_;
    }

    void RecentSessions::addFile(QString const& _file) {
      addFile(_file,true);
    }

    void RecentSessions::addFile(QString const& _file, bool _regenerateMenu) {
      int _index = fileInList(_file);

      if (_index != -1) {
        recentFiles_.erase(recentFiles_.begin() + _index);
      }

      recentFiles_.push_front(_file);

      if (recentFiles_.size() > numberOfFiles_) {
        recentFiles_.pop_back();
      }

      if (_regenerateMenu) {
        generateMenu();
      }
    }

    int RecentSessions::fileInList(QString const& _file) const {
      int _index = 0;

      for (auto& _sessionFile : recentFiles_) {
        if (_sessionFile == _file) return _index;

        ++_index;
      }
      return -1;
    }

    QMenu * RecentSessions::menu() {
      return menu_.get();
    }

    void RecentSessions::readFromSettings() {

      auto& _settings = Application::settings();

      int _size = _settings.beginReadArray("recentFiles");
      for (int i = 0; i < _size; ++i) {
        _settings.setArrayIndex(i);
        addFile(_settings.value("file").toString(),false);
      }
      _settings.endArray();
      generateMenu();
    }

    void RecentSessions::writeToSettings() const {
      auto& _settings = Application::settings();

      _settings.beginWriteArray("recentFiles");
      for (int i = 0; i < recentFiles_.size(); ++i) {
        auto& _file = recentFiles_[i];
        _settings.setArrayIndex(i);
        _settings.setValue("file",_file);
      }
      _settings.endArray();
    }

    void RecentSessions::clear() {
      recentFiles_.clear();
      generateMenu();
    }

    void RecentSessions::setActionStates() {
      for (auto& _action : actions_) {
        _action->setState();
      }
    }

    void RecentSessions::generateMenu() {
      if (!menu_) {
        menu_.reset(new QMenu());
        connect(menu_.get(),SIGNAL(aboutToShow()),this,SLOT(setActionStates()));
      }

      menu_->clear();
      actions_.clear();

      for (auto& _file : recentFiles_) {
        auto* _action = new RecentFileAction(_file);
        connect(_action,SIGNAL(fileToBeLoaded(QString const&)),
                this,SIGNAL(fileToBeLoaded(QString const&)));
        menu_->addAction(_action);
        actions_.push_back(_action);
      }

      if (!recentFiles_.empty()) {
        menu_->addSeparator();
        auto *_clearAction = menu_->addAction("Clear menu");
        connect(_clearAction, SIGNAL(triggered()), this, SLOT(clear()));
      } else {
        auto *_placeholder = menu_->addAction("No recent files");
        _placeholder->setEnabled(false);
      }
    }
  }
}
