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

#ifndef OMNI_UI_AFFINETRANSFORM_H_
#define OMNI_UI_AFFINETRANSFORM_H_

#include <memory>
#include <QWidget>
#include <omni/geometry/AffineTransform.h>
#include <omni/ui/RangedFloat.h>
#include <omni/ui/mixin/Scale.h>
#include <omni/ui/mixin/DataModel.h>

namespace omni {
  namespace ui {
    namespace Ui {
      class AffineTransform;
    }

    /// A widget for editing an affine transformation
    class OMNIWIDGET_EXPORT AffineTransform :
      public QWidget,
      public mixin::UnsharedDataModel<AffineTransform,omni::AffineTransform>,
      private mixin::Scale<RangedFloat>{
        Q_OBJECT
        OMNI_UI_UNSHARED_DATAMODEL(AffineTransform,omni::AffineTransform)

        using mixin_scale_type = mixin::Scale<RangedFloat>;
      public:
        AffineTransform(QWidget * = nullptr);
        ~AffineTransform() override;

        /// Return true if translation sliders are visible
        bool  isTranslationVisible() const;

        /// Return true if rotation widget is visible
        bool  isRotationVisible() const;

        /// Return true if scale sliders are visible
        bool  isScaleVisible() const;

        /// Return value of scale range (5.0 by default)
        float scaleRange() const;

      public slots:
        /// Set visibility of translation sliders
        void  setTranslationVisible(bool);

        /// Set visibility of rotation widget
        void  setRotationVisible(bool);

        /// Set visibility of scale sliders
        void  setScaleVisible(bool);

        /// Set new value for scale range (5.0 by default)
        void  setScaleRange(float);

        /// Set unit suffix for offset
        void setOffsetUnit(QString const& _unit);

        /// Set scale of ranges for offsets
        void setOffsetRangeScale(float);

        /// Set value if offset values are rescaled
        void setRescaleOffsetValues(bool);

      signals:
        void  dataModelChanged();

      private:
        /// Setup widget
        void  setup();

        /// Update sliders from current transform
        void  dataToFrontend() override;

        /// Assign slider values to current transform
        bool  frontendToData() override;

        float scaleRange_ = 1.0;
        std::unique_ptr<Ui::AffineTransform> ui_;
    };
  }
}

#endif /* OMNI_UI_AFFINETRANSFORM_H_ */
