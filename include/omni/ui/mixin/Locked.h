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
#ifndef OMNI_UI_MIXIN_LOCKED_H_
#define OMNI_UI_MIXIN_LOCKED_H_

namespace omni
{
        namespace ui
        {
                namespace mixin
                {
                        /// Mixin with a function to lock the widget temporarily
                        class Locked {
                        public:
                                template<typename F>
                                inline void locked(F f)
                                {
                                        bool _oldLocked = locked_;
                                        locked_ = true;
                                        f();
                                        locked_ = _oldLocked;
                                }

                                inline bool isLocked() const
                                {
                                        return locked_;
                                }
                        private:
                                /// Flag which sets if widgets are currently locked
                                bool locked_ = false;
                        };
                }
        }
}

#endif /* OMNI_UI_MIXIN_LOCKED_H_ */
