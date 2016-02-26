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

#ifndef OMNI_UI_TUNINGWIDGET_H_
#define OMNI_UI_TUNINGWIDGET_H_

#include <omni/ui/mixin/Locked.h>
#include <omni/proj/Tuning.h>

namespace omni
{
        namespace ui
        {
                namespace mixin
                {
                        /**@brief Helper class with an interface for getting parameter from tuning
                        **/
                        class TuningWidget : protected Locked
                        {
                        public:
                                inline omni::proj::Tuning const* tuning() const
                                {
                                        return tuning_;
                                }

                                inline omni::proj::Tuning* tuning()
                                {
                                        return tuning_;
                                }

                                inline virtual void setTuning(omni::proj::Tuning* _tuning)
                                {
                                        this->locked([&]()
                                        {
                                                tuning_ = _tuning;
                                                if (tuning_)
                                                        tuningParameters();
                                        });
                                }

                            protected:
                                /// Virtual method for retrieving parameters from tuning
                                virtual void tuningParameters() = 0;

                            private:
                                omni::proj::Tuning* tuning_ = nullptr;
                        };
                }
        }
}

#endif /* OMNI_UI_TUNINGWIDGET_H_ */
