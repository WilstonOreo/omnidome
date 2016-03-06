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

#ifndef OMNI_UI_AFFINETRANSFORM_H_
#define OMNI_UI_AFFINETRANSFORM_H_

#include <memory>
#include <QWidget>
#include <omni/AffineTransform.h>
#include <omni/ui/RangedFloat.h>
#include <omni/ui/mixin/Locked.h>
#include <omni/ui/mixin/Scale.h>

namespace omni {
    namespace ui {
        namespace Ui {
            class AffineTransform;
        }

        /// A widget for editing an affine transformation
        class AffineTransform :
            public QWidget,
            private mixin::Locked,
            private mixin::Scale<RangedFloat> {
            Q_OBJECT
        public:
            AffineTransform(QWidget* = nullptr);
            ~AffineTransform();

            omni::AffineTransform const* transform() const;
            void setTransform(omni::AffineTransform*);

        public slots:
            void updateTransform();
            
        private:


            std::unique_ptr<Ui::AffineTransform> ui_;
            omni::AffineTransform* transform_ = nullptr;
        };
    }
}

#endif /* OMNI_UI_AFFINETRANSFORM_H_ */
