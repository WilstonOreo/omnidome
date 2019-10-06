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

#include "SyphonWidget.h"

#include <QComboBox>
#include <QSignalBlocker>

#include "SyphonServerDescription.h"
#include "SyphonServerItemModel.h"
#include "SyphonServerManager.h"

namespace omni {
  namespace ui {
    namespace input {
      Syphon::Syphon(omni::input::Syphon *_input,
                     QWidget *_parent) :
        ParameterWidget(_parent),
        input_(_input) {
        setup();
      }

      Syphon::~Syphon() {}

      void Syphon::updateServerList() {
        boxServerList_->clear();
        auto&& _serverList = serverManager_->getServerList();

        int _index = 0;
        int _inputIndex = -1;
        for (auto& _server : _serverList) {
          model_->addDescription(_server);

          // Search server for input
          if (input_) {
            if (input_->description() == _server) {
              _inputIndex = _index;
            }
          }
          ++_index;
        }

        if (_inputIndex != -1) {
          QSignalBlocker blocker(this);
          boxServerList_->setCurrentIndex(_inputIndex);
        } else {
          boxServerList_->setCurrentIndex(0);
        }
      }

      void Syphon::triggerUpdate() {
        if (!preview_) return;

        preview_->triggerUpdate();
      }

      void Syphon::setDescriptionToInput() {
        if (signalsBlocked()) return;

        input_->setDescription(model_->getDescription(boxServerList_->currentIndex()));
      }

      void Syphon::setup() {
        serverManager_.reset(new omni::input::SyphonServerManager());

        QLayout *_layout = new QVBoxLayout;
        _layout->setSpacing(2);
        _layout->setContentsMargins(0, 0, 0, 0);
        _layout->setSizeConstraint(QLayout::SetMaximumSize);
        setLayout(_layout);

        model_.reset(new SyphonServerItemModel);

        boxServerList_.reset(new QComboBox());
        boxServerList_->setModel(model_.get());
        _layout->addWidget(boxServerList_.get());
        connect(serverManager_.get(), SIGNAL(serverAnnounced(ServerList const&)),
          this, SLOT(updateServerList()));
        connect(serverManager_.get(), SIGNAL(serverUpdated(ServerList const&)),
          this, SLOT(updateServerList()));
        connect(serverManager_.get(), SIGNAL(serverRetired(ServerList const&)),
          this, SLOT(updateServerList()));

        preview_.reset(new InputPreview(input_));
        connect(preview_.get(), SIGNAL(inputChanged()), this,
                SIGNAL(inputChanged()));
        _layout->addWidget(preview_.get());

        connect(boxServerList_.get(), SIGNAL(currentIndexChanged(int)),
          this, SLOT(setDescriptionToInput()));

        serverManager_->setup();
      }
    }
  }
}
