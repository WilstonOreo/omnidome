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

#ifndef OMNI_UI_PROJ_TUNING_H
#define OMNI_UI_PROJ_TUNING_H

#include <omni/proj/Tuning.h>
#include <omni/ui/ParameterWidget.h>
#include "mixin/DataModel.h"
#include "mixin/TuningFromIndex.h"
#include "proj/TuningLayout.h"

namespace omni {
  class Session;

  namespace proj {
    class Tuning;
  }

  namespace ui {
    class TuningGLView;

    namespace proj {
      class TitleBar;
      class TuningLayout;

      /**@brief Widget for manipulating projector parameters
         @detail Also holds a preview OpenGL widget
       **/
      class Tuning :
        public ParameterWidget,
        public mixin::SharedDataModel<Tuning,Session>,
        public mixin::TuningFromIndex<Tuning> {
          Q_OBJECT
          OMNI_UI_SHARED_DATAMODEL(Tuning,Session)
        public:
          /// View mode (determines which elements are to be displayed)
          enum WindowState
          {
            NO_DISPLAY,         // Widget is minimized
            DISPLAY_ONLY,       // Only preview is displayed
            FOV_SLIDERS,        // Only FOV sliders are displayed
            ADJUSTMENT_SLIDERS, // Only sliders for adjustment are displayed
            NUM_WINDOW_STATES
          };

          typedef TuningLayout::widgetgroup_type widgetgroup_type;

          /// Default constructor
          Tuning(QWidget *_parent = nullptr);

          /// Constructs with a given tuning (called by default from TuningList)
          Tuning(int _index,
                 std::shared_ptr<omni::Session>_session,
                 QWidget *_parent = nullptr);

          /// Destructor
          ~Tuning() override;

          /// Return pointer to preview widget
          TuningGLView      * previewWidget();

          /// Return pointer to preview widget (const version)
          TuningGLView const* previewWidget() const;

          /// Return selected flag
          bool                isSelected() const;

          /// Return current window state
          WindowState         windowState() const;

        public slots:
          /// Sets Window State which determines which elements are visible
          void setWindowState(WindowState _mode);

          /// Selects succeeding window state
          void setNextWindowState();

          /**@brief Sets flag if Tuning Widget is active
             @detail A tuning widget should only be active when
           **/
          void setSelected(bool);

          /// Display content and widget for specific session mode
          void sessionModeChange();

          /// Attaches a screen to this tuning
          void attachScreen(QScreen const *_screen,
                            int _subScreenIndex);

          /**@brief Detaches screen from this tuning
             @detail Also triggered when QGuiApplication::screenAdded or
                     QGuiApplication::screenRemoved were fired
           **/
          void detachScreen();

          /// Update GL View and Fullscreen view, if there is any
          void updateViews();

          /// Enable or disable fullscreen display
          void fullscreenToggle(bool);

          /// Reset to free projector setup (discards previous projector setup)
          void resetToFreeSetup();

          /// Reset to peripheral projector setup (discards previous projector
          // setup)
          void resetToPeripheralSetup();
          /// Set parameters from sliders to tuning
          void updateParameters() override;

        signals:
          void selected(int);
          void closed(int);
          void projectorSetupChanged();
          void dataModelChanged();

        protected:
          /// Handles resizing of sliders and preview
          void resizeEvent(QResizeEvent *) override;

          /// Handles proper resizing of the widget
          void showEvent(QShowEvent *) override;

          /// Paint border
          void paintEvent(QPaintEvent *) override;

          /// Mouse Move Event and handler for dragging to ScreenSetup widget
          void mouseMoveEvent(QMouseEvent *) override;

          /// Handles focus events from child widgets
          bool eventFilter(QObject *,
                           QEvent *) override;

          /// Focus in event used by TuningList to set current tuning for
          // session
          void focusInEvent(QFocusEvent *) override;

          /// Focus out for deselecting tuning
          void focusOutEvent(QFocusEvent *) override;

        private slots:
          void startDrag();


          /// Set FOV to projector from slider
          void setFov();

          /// Set Throw Ratio to projector from slider
          void setThrowRatio();

          /// Set keystone correction from slider
          void setKeyStone();

          /// Update slider and border color
          void updateColor();

          /// Clean up.
          void prepareRemove();

        private:
          void dataToFrontend() override;
          bool frontendToData() override;

          /// Setup (only called in constructor)
          void setup();

          /// Id of first focussed widget
          inline virtual int firstFocusId() const override {
            return std::min(layout()->count()-1,2); // Title bar and view cannot be focussed
          }

          /// Adds a new/changes a parameter group
          void addGroup(QString const& _groupName,
                        widgetgroup_type const& _widgets);

          void setGroup(QString const& _groupName);

          /// Title bar widget
          QWidgetPtr<TitleBar> titleBar_;

          /// GL preview widget
          QWidgetPtr<TuningGLView> glView_;

          /// Window State
          WindowState windowState_ = ADJUSTMENT_SLIDERS;

          /// Is true when this widgets index and current tuning index are equal
          bool isSelected_ = true;

          /// Layout
          TuningLayout *layout_ = nullptr;

          std::map<QString, widgetgroup_type> groups_;
      };
    }
  }
}

#endif /* OMNI_UI_PROJ_TUNING_H */
