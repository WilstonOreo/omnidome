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

#ifndef OMNI_UI_MIXIN_EDITOR_H
#define OMNI_UI_MIXIN_EDITOR_H

#include <QLayout>

namespace omni {
  namespace ui {
    namespace mixin {
      /// Mixin for a widget that holds an editor widget
      template<typename WIDGET, typename EDITOR>
      class Editor {
        public:
          /// Widget type
          typedef WIDGET widget_type;

          /// Editor widget type, e.g. QDoubleSpinBox
          typedef EDITOR editor_type;

          Editor(widget_type *_widget) : widget_(_widget) {}

          /// Create editor with type
          template<typename T = editor_type>
          inline T* createEditor()
          {
            if (editor_) return nullptr;

            editor_ = new T(widget_);
            T *_e = editorAs<T>();
            _e->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Ignored);
            _e->setStyleSheet("background : transparent");

            if (widget_->layout()) widget_->layout()->addWidget(_e);
            editorSetup();
            hideEditor();
            return _e;
          }

          /// Return pointer editor (const version)
          editor_type const* editor() const {
            return editor_;
          }

          /// Return pointer editor
          editor_type* editor() {
            return editor_;
          }

          /// Return editor as type T
          template<typename T>
          T* editorAs()
          {
            return static_cast<T *>(editor_);
          }

          /// Return editor as type T (const version)
          template<typename T>
          T const* editorAs() const
          {
            return static_cast<T const *>(editor_);
          }

          /// Set editor visibility and set focus
          virtual void setEditorVisible(bool _visible) {
            if (!editor_) return;

            editor_->setVisible(_visible);

            if (_visible) {
              editor_->setFocus();
            }
          }

          /// Show editor and set focus
          void showEditor() {
            setEditorVisible(true);
          }

          /// Hide editor
          void hideEditor() {
            setEditorVisible(false);
          }

        private:
          virtual void editorSetup() = 0;
          editor_type *editor_ = nullptr;
          widget_type *widget_ = nullptr;
      };
    }
  }
}

#endif /* OMNI_UI_MIXIN_EDITOR_H */
