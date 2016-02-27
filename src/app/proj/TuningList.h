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

#ifndef OMNI_UI_PROJ_TUNINGLIST_H
#define OMNI_UI_PROJ_TUNINGLIST_H

#include <memory>
#include <QScrollArea>
#include <omni/util.h>
#include <omni/ui/mixin/SessionWidget.h>
#include "proj/Tuning.h"

namespace omni
{
  class Session;

  namespace ui
  {
    namespace proj
    {
      class Tuning;

      class TuningList :
        public QScrollArea,
        public mixin::SessionWidget
      {
        Q_OBJECT
      public:
        // Maximum number of tunings one can add
        inline static constexpr int maxNumberTunings() { return 16; }

        TuningList(QWidget* = nullptr);
        ~TuningList();

        /// Return fullscreen and preview widget from index
        std::set<TuningGLView*> getViews(int _index) const;

        /// Return tuning widget at index, nullptr if index is not valid
        QWidget* widget(int _index);

        /// Return tuning widget at index, nullptr if index is not valid (const)
        QWidget const* widget(int _index) const;

      public slots:
        /// Change current mode for all tuning widgets
        void sessionModeChange();

        /// Add a new tuning and tuning widget, with automatically detected settings
        void addTuning();

        /// Add tuning with specific projector setup
        void addTuning(QString const& _projSetupId);

        /// Opens multi setup dialog and appends/replaces new projections when dialogs was accepted
        void addMultiSetup(QString const& _multiSetupId);

        /// Remove tuning
        void removeTuning(int _index);

        /// Set current tuning from currently selected widget
        void setCurrentTuning();

        /// Set current tuning index manually (currentIndexChanged signal is emitted)
        void setTuningIndex(int);

        /// Remove all tunings from session and all associated widgets
        void clear();

        /// Updates/Repaints GL Views of all tunings widgets
        void updateViews();

      protected:
        void resizeEvent(QResizeEvent*);
        void keyPressEvent(QKeyEvent*);
        bool eventFilter(QObject *obj, QEvent *event);

      signals:
        /// Signal which is emitted when the current tuning has changed
        void currentIndexChanged(int);

        /// Signal which is emitted when parameters of one tuning have changed
        void projectorSetupChanged();

        /// Signal which is returned after a tuning is to be removed
        void tuningToBeRemoved(omni::ui::proj::Tuning*);

      private:
        void sessionParameters();

        /// Add widget from existing tuning
        void addTuning(omni::proj::Tuning* _tuning);


        /**@brief Removes all widgets in the scroll area
          *@detail Unlike clear() it does not remove tunings from session
        **/
        void removeWidgets();

        /// Get most differing color for a new tuning
        QColor getTuningColor();

        QWidget* contents_;
        QLayout* layout_;

        std::vector<QUniquePtr<Tuning>> widgets_;
      };
    }
  }
}

#endif /* OMNI_UI_PROJ_TUNINGLIST_H */
