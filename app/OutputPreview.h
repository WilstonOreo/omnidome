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

#include <QWidget>
#include <QColor>
#include <QRect>
#include <QPoint>
#include <omni/Session.h>

#include "mixin/DataModel.h"
#include "mixin/TransformedRect.h"

namespace omni {
  namespace ui {
    /// Widget to draw rendered output of a session
    class OutputPreview :
      public QWidget,
      public mixin::SharedDataModel<OutputPreview,Session>,
      public mixin::TransformedRect<OutputPreview>{
        Q_OBJECT
        OMNI_UI_SHARED_DATAMODEL(OutputPreview,Session)

      public:
        OutputPreview(QWidget * = nullptr);
        ~OutputPreview() override;

        /// Return desktop rectangle
        QRect desktopRect() const override;

      public slots:
        void  triggerUpdate();

      signals:
        void  dataModelChanged();

      protected:
        void  resizeEvent(QResizeEvent *) override;
        void  paintEvent(QPaintEvent *) override;
        void  timerEvent(QTimerEvent *) override;

      private:
        void  drawBorder(QColor _color,
                         QRect const& _rect,
                         QPoint& _capPos,
                         QString const& _caption);

        void  drawTuning(QPainter&,
                         proj::Tuning const *);
        void  drawScreen(QPainter&,
                         QScreen const *);
        int   verticalMultiplier() const;

        /// Return non transformed rectangle for tuning
        QRect getRectForTuning(proj::Tuning const *);

        void  dataToFrontend() override;
        inline bool frontendToData() override {
          return false;
        }

        int timerId_ = 0;
        bool needsUpdate_ = true;
        QImage image_;
    };
  }
}
