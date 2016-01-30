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

#ifndef OMNI_UI_INPUT_H_
#define OMNI_UI_INPUT_H_

#include <memory>
#include <QStandardItemModel>
#include <omni/input/Interface.h>
#include <omni/ui/CollapsibleGroupBox.h>

namespace omni
{
  class Session;

  namespace ui
  {
    namespace Ui
    {
      class Input;
    }

    /// Input List widget
    class Input : public CollapsibleGroupBox
    {
      Q_OBJECT
    public:
      Input(QWidget* = nullptr);
      ~Input();

      Session const* session() const;
      void setSession(Session* _session);

    public slots:
      void addInput(QAction*);

      /// Removes all selected inputs
      void removeSelection();

      /// Removes all inputs
      void clear();

      /// Select input at model index
      void changeSelection(QModelIndex);

    signals:
      void inputChanged();

    private:
      /// Setup the item model with columns etc
      void prepareModel();

      bool showSettingsDialog(input::Interface*);

      /// Add an item to input list widget
      void addItem(input::Interface const*);

      Session* session_ = nullptr;
      std::unique_ptr<Ui::Input> ui_;
      std::unique_ptr<QStandardItemModel> model_;
    };
  }
}


#endif /* OMNI_UI_INPUT_H_ */
