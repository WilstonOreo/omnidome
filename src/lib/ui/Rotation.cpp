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

#include <omni/ui/Rotation.h>

#include <QHBoxLayout>

namespace omni {
    namespace ui {
        Rotation::Rotation(QWidget *_parent) :
            QWidget(_parent)
        {
            init(0.0, 0.0, 0.0);
        }

        Rotation::Rotation(double _x, double _y, double _z, QWidget *_parent) :
            QWidget(_parent)
        {
            init(_x, _y, _z);
        }

        Rotation::~Rotation()
        {}

        void Rotation::init(double _x, double _y, double _z)
        {
            QLayout *_layout = new QHBoxLayout();

            auto setupDial = [&](double _value, QChar _letter) -> Dial *
                             {
                                 Dial *_d = new Dial(_value, 0.0, 360.0);

                                 _d->setRange(0.0, 360.0);
                                 _d->setSingleStep(5.0);
                                 _d->setPageStep(45.0);
                                 _d->setLetter(_letter);
                                 _d->setSuffix("°");
                                 _layout->addWidget(_d);
                                 return _d;
                             };

            x_ = setupDial(_x, 'X');
            connect(x_, SIGNAL(valueChanged()), this, SIGNAL(xChanged()));

            y_ = setupDial(_y, 'Y');
            connect(y_, SIGNAL(valueChanged()), this, SIGNAL(yChanged()));

            z_ = setupDial(_z, 'Z');
            connect(z_, SIGNAL(valueChanged()), this, SIGNAL(zChanged()));

            setLayout(_layout);
        }

        double Rotation::x() const
        {
            return x_->value();
        }

        double Rotation::y() const
        {
            return y_->value();
        }

        double Rotation::z() const
        {
            return z_->value();
        }

        void Rotation::setX(double _x)
        {
            x_->setValue(_x);
            emit xChanged();
        }

        void Rotation::setY(double _y)
        {
            y_->setValue(_y);
            emit yChanged();
        }

        void Rotation::setZ(double _z)
        {
            z_->setValue(_z);
            emit zChanged();
        }
    }
}
