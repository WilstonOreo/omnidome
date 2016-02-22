/* Copyright (c) 2014-2016 "Omnidome" by cr8tr
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
#ifndef OMNI_RENDEROUTPUT_H_
#define OMNI_RENDEROUTPUT_H_

#include <QScreen>
#include <omni/proj/Tuning.h>
#include <omni/RenderBuffer.h>

namespace omni {

    class RenderOutputElement {
    public:

        proj::Tuning const* tuning() const;
        void setTuning(proj::Tuning const* _tuning);

        QScreen const* screen() const;
        void setScreen(QScreen const*);

    private:
        proj::Tuning const* tuning_ = nullptr;
        QScreen const* screen_ = nullptr;
        RenderBuffer buffer_;
    };

    class RenderOutput : private std::vector<RenderOutputElement> {

    };
}

#endif /* OMNI_RENDEROUTPUT_H_ */
