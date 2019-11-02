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

#ifndef OMNI_UI_COLORSET_H_
#define OMNI_UI_COLORSET_H_

#include <QWidget>
#include <QPalette>
#include <omni/ui/global.h>

namespace omni {
  namespace ui {
    /// Simplies color access from a palette for a widget
    struct OMNIWIDGET_EXPORT ColorSet
    {
      /**@brief Default constructor
         @param _parent Pointer to parent widget
       **/
      ColorSet(QWidget *_parent);

      QColor               color(QPalette::ColorRole) const;
      QPalette::ColorGroup colorGroup() const;

      /// Window aka Background color
      QColor window() const;

      /// Window text aka Foreground color
      QColor windowText() const;

      /**@brief Base color
       * @param _alternate Returns alternate color (used for alternted rows)
       **/
      QColor base(bool _alternate = false) const;

      /// Button color (ColorRole = QPalette::Button)
      QColor button() const;

      /// Tooltip color (ColorRole = QPalette::ToolTipBase)
      QColor toolTipBase() const;

      /// Tooltip text color (ColorRole = QPalette::ToolTipText)
      QColor toolTipText() const;

      /// Text color (ColorRole = QPalette::Text)
      QColor text() const;

      /// Button text color (ColorRole = QPalette::ButtonText)
      QColor buttonText() const;

      /// Bright text color (ColorRole = QPalette::BrightText)
      QColor brightText() const;

      /// Light color (ColorRole = QPalette::Light)
      QColor light() const;

      /// Mid light color (ColorRole = QPalette::Midlight)
      QColor midlight() const;

      /// Dark color (ColorRole = QPalette::Dark)
      QColor dark() const;

      /// Shadow color (ColorRole = QPalette::Shadow)
      QColor shadow() const;

      /// Highlight color (ColorRole = QPalette::Highlight)
      QColor highlight() const;

      /// HighlightText color (ColorRole = QPalette::HighlightText)
      QColor highlightedText() const;

      /// Link color (ColorRole = QPalette::Link)
      QColor link() const;

      /// Visited link color (ColorRole = QPalette::LinkVisited)
      QColor linkVisited() const;

      private:
        /// Pointer to parent widget
        QWidget *parent_;
    };
  }
}

#endif /* OMNI_UI_COLORSET_H_ */
