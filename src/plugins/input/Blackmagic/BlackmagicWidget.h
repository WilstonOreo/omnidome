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

#ifndef OMNI_INPUT_BLACKMAGICWIDGET_H_
#define OMNI_INPUT_BLACKMAGICWIDGET_H_

#include <QStandardItemModel>

#include <omni/util.h>
#include <omni/ui/InputPreview.h>
#include <omni/ui/ParameterWidget.h>
#include "Blackmagic.h"

class QComboBox;
class QPushButton;
class IDeckLink;

class IDeckLinkDisplayMode;

namespace omni {
  namespace ui {
    namespace input {
      class BlackmagicDeviceItemModel : public QStandardItemModel {
        Q_OBJECT 
      public:
        BlackmagicDeviceItemModel(QObject* parent = nullptr);
        ~BlackmagicDeviceItemModel();
        
        void update();

        IDeckLink* device(int) const;
      };

      class BlackmagicDisplayModeItemModel : public QStandardItemModel {
        Q_OBJECT 
      public:
          BlackmagicDisplayModeItemModel(QObject* parent = nullptr);
          ~BlackmagicDisplayModeItemModel();
        
          void update(IDeckLink* deckLink);

          IDeckLinkDisplayMode* displayMode(IDeckLink*,int) const;
      private:

          void addDisplayMode(int idx, QString const& str, QSize const& size, double fps);
      };

      /// Blackmagic widget
      class Blackmagic : public omni::ui::ParameterWidget {
        Q_OBJECT
        public:
          Blackmagic(omni::input::Blackmagic*,
                 QWidget * = nullptr);
          ~Blackmagic();

        public slots:
          void updateDisplayModes();
          void detectDisplayMode();

          /// Pass-through signal to update input preview
          void triggerUpdate();

        signals:
          void inputChanged();

        private:
          void setup();

          omni::input::Blackmagic *input_ = nullptr;
          QWidgetPtr<InputPreview> preview_;
          QWidgetPtr<QComboBox>    deviceList_;
          QWidgetPtr<QComboBox>    displayModeList_;
          QWidgetPtr<QPushButton>    btnDetect_;
          QUniquePtr<BlackmagicDeviceItemModel> deviceModel_;
          QUniquePtr<BlackmagicDisplayModeItemModel> displayModeModel_;
      };
    }
  }
}




#endif /* OMNI_INPUT_BLACKMAGICWIDGET_H_ */ 
