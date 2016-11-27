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

#include <omni/ui/ColorSet.h>

namespace omni {
    namespace ui {
        ColorSet::ColorSet(QWidget *_parent) :
            parent_(_parent) {
        }

        QColor ColorSet::color(QPalette::ColorRole _role) const
        {
            return parent_->palette().color(colorGroup(), _role);
        }

        QPalette::ColorGroup ColorSet::colorGroup() const
        {
            return parent_->isEnabled() ?
                   (parent_->hasFocus() ? QPalette::Active : QPalette::Inactive) :
                   QPalette::Disabled;
        }

#define COLORSET_COLOR_GETTER(name, colorrole) \
    QColor ColorSet::name() const              \
    {                                          \
        return this->color(colorrole);         \
    }

        COLORSET_COLOR_GETTER(window,          QPalette::Window)
        COLORSET_COLOR_GETTER(windowText,      QPalette::WindowText)
        COLORSET_COLOR_GETTER(button,          QPalette::Button)
        COLORSET_COLOR_GETTER(toolTipBase,     QPalette::ToolTipBase)
        COLORSET_COLOR_GETTER(toolTipText,     QPalette::ToolTipText)
        COLORSET_COLOR_GETTER(text,            QPalette::Text)
        COLORSET_COLOR_GETTER(buttonText,      QPalette::ButtonText)
        COLORSET_COLOR_GETTER(brightText,      QPalette::BrightText)
        COLORSET_COLOR_GETTER(light,           QPalette::Light)
        COLORSET_COLOR_GETTER(midlight,        QPalette::Midlight)
        COLORSET_COLOR_GETTER(dark,            QPalette::Dark)
        COLORSET_COLOR_GETTER(shadow,          QPalette::Shadow)
        COLORSET_COLOR_GETTER(highlight,       QPalette::Highlight)
        COLORSET_COLOR_GETTER(highlightedText, QPalette::HighlightedText)
        COLORSET_COLOR_GETTER(link,            QPalette::Link)
        COLORSET_COLOR_GETTER(linkVisited,     QPalette::LinkVisited)

        QColor ColorSet::base(bool _alternate) const
        {
            return _alternate ?
                   color(QPalette::AlternateBase) :
                   color(QPalette::Base);
        }
    }
}
