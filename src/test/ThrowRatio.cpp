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

#include <omni/Angle.h>
#include <omni/proj/Projector.h>

namespace omni {
    namespace test {
        /// Throw Ratio test
        struct ThrowRatio {
            ThrowRatio(Angle _angle) {
                proj::Projector _proj;
                _proj.setFov(_angle);
                qDebug() << "FOV: " << _angle.degrees() << "\t ThrowRatio: " << _proj.throwRatio();

            }
        };

        /// Field of view test
        struct FOV {
            FOV(qreal _throwRatio) {
                proj::Projector _proj;
                _proj.setThrowRatio(_throwRatio);
                qDebug() << "FOV: " << _proj.fov().degrees() << "\t ThrowRatio: " << _throwRatio;
            }

        };
    }
}

int main(int ac, char* av[]) {

    using namespace omni;
    test::ThrowRatio(90.0);
    test::ThrowRatio(60.0);
    test::ThrowRatio(45.0);
    test::ThrowRatio(36.0);

    test::FOV(1.5);
    test::FOV(1.0);
    test::FOV(0.8);
    test::FOV(0.5);
}
