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

#ifndef OMNI_UI_MIXIN_EDITOR_H
#define OMNI_UI_MIXIN_EDITOR_H

#include <QLayout>

namespace omni {
        namespace ui {
                namespace mixin {
                        template<typename WIDGET, typename EDITOR>
                        class Editor
                        {
                        public:
                                typedef WIDGET widget_type;
                                typedef EDITOR editor_type;

                                Editor(widget_type* _widget) : widget_(_widget) {}

                                template<typename T = editor_type>
                                inline T* createEditor()
                                {
                                        if (editor_) return nullptr;

                                        editor_ = new T(widget_);
                                        T* _e = editorAs<T>();
                                        _e->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Ignored);
                                        _e->setStyleSheet("background : transparent");
                                        if (widget_->layout())
                                            widget_->layout()->addWidget(_e);
                                        editorSetup();
                                        hideEditor();
                                        return _e;
                                }

                        editor_type const* editor() const {
                            return editor_;
                        }

                        editor_type* editor() {
                            return editor_;
                        }

                        template<typename T>
                        T* editorAs()
                        {
                                return static_cast<T*>(editor_);
                        }

                        template<typename T>
                        T const* editorAs() const
                        {
                                return static_cast<T const*>(editor_);
                        }
                        virtual void setEditorVisible(bool _visible) {
                            if (!editor_) return;
                            editor_->setVisible(_visible);

                            if (_visible) {
                                editor_->setFocus();
                            }
                        }
                        void showEditor() {
                            setEditorVisible(true);
                        }
                        void hideEditor() {
                            setEditorVisible(false);
                        }

                        private:
                                virtual void editorSetup() = 0;
                                editor_type* editor_ = nullptr;
                                widget_type* widget_ = nullptr;
                        };
                }
        }
}

#endif /* OMNI_UI_MIXIN_EDITOR_H */
