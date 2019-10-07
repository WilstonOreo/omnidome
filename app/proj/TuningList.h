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

#ifndef OMNI_UI_PROJ_TUNINGLIST_H
#define OMNI_UI_PROJ_TUNINGLIST_H

#include <memory>
#include <QScrollArea>
#include <omni/util.h>
#include <omni/Session.h>
#include "mixin/DataModel.h"
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
        public mixin::SharedDataModel<TuningList,Session>
      {
        Q_OBJECT
        OMNI_UI_SHARED_DATAMODEL(TuningList,Session)
      public:
        TuningList(QWidget* = nullptr);
        ~TuningList() override;

        /// Return fullscreen and preview widget from index
        std::set<TuningGLView*> getViews(int _index) const;

        /// Return tuning widget at index, nullptr if index is not valid
        Tuning* widget(int _index);

        /// Return tuning widget at index, nullptr if index is not valid (const)
        Tuning const* widget(int _index) const;

        Tuning* widgetFromTuning(omni::proj::Tuning*);

      public slots:
        /// Change current mode for all tuning widgets
        void sessionModeChange();

        /// Add a new tuning and tuning widget, with automatically detected settings
        void addTuning();

        /// Add tuning with specific projector setup
        void addTuning(Projector const&);

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

        /// Adjust sliders to scene scale
        void updateSceneSize(bool _rescaleValues);

        /// Set unit of sliders
        void updateUnits();

        /// Disable all tunings and fullscreen
        void disableAllTunings();

        /// Enable all tunings and fullscreen
        void enableAllTunings();

        /// Enable selected tuning and show only this in fullscreen
        void enableSelectedTuningOnly();

      protected:
        void resizeEvent(QResizeEvent*) override;
        void keyPressEvent(QKeyEvent*) override;
        bool eventFilter(QObject *obj, QEvent *event) override;

      signals:
        /// Signal which is emitted when the current tuning has changed
        void currentIndexChanged(int);

        /// Signal which is emitted when parameters of one tuning have changed
        void dataModelChanged();

        /// Signal is emitted when a tuning was added
        void tuningAdded();

        /// Signal is emitted when a tuning was removed
        void tuningRemoved();

        /// Emitted when a single tuning has changed
        void tuningChanged();

      private:
          /// Update sliders from current session
          void dataToFrontend() override;

          /// Assign slider values to current session
          inline bool frontendToData() override {
              return false;
          }

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

        std::vector<QWidgetPtr<Tuning>> widgets_;
      };
    }
  }
}

#endif /* OMNI_UI_PROJ_TUNINGLIST_H */
