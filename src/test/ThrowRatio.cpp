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

#include <array>

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

        struct EncodeFloatInByteArray {
          EncodeFloatInByteArray() {
            test();
          }

          void test() {

            size_t _r = 123;
            float _d = 1.43241;
            float _s = _d;

            for (size_t i = 0; i < _r; ++i) {
              array_type _array;
              encode(_s,_array);

              float _result = 0.0;
              decode(_array,_result);

              qDebug() << _s << _result << (_s - _result);
              _s *= _d;
            }
          }

          typedef std::array<uint8_t,8> array_type;

          static void encode(const float& _t, array_type& _array) {
            for (int i = 0; i < _array.size(); ++i) {
              float _n = _t / std::pow(256.0,float(i-int(_array.size()/2)));
              float _intpart;
              float _fractpart = std::modf (_n , &_intpart);
              _array[i] = uint8_t(256.0*_fractpart);
            }
          }

          static void decode(const array_type& _array, float& _t) {
            float _sum = 0.0;
            for (int i = 0; i < _array.size(); ++i) {
              _sum += (_array[i] / 256.0) * std::pow(256.0,float(i-int(_array.size()/2)));
            }
            _t = _sum;
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

    test::EncodeFloatInByteArray();
}
