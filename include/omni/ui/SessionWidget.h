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

#ifndef OMNI_UI_SESSIONWIDGET_H_
#define OMNI_UI_SESSIONWIDGET_H_

#include <omni/Session.h>

namespace omni
{
  namespace ui
  {
    /**@brief Helper class with an interface for getting parameter from session
     **/
    class SessionWidget
    {
    public:
      inline Session const* session() const
      {
        return session_;
      }

      inline Session* session()
      {
        return session_;
      }

      inline void setSession(Session* _session)
      {
        this->locked([&]()
        {
          session_ = _session;
          if (session_)
            sessionParameters();
        });
      }

    protected:
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

      /// Virtual method for retrieving parameters from session
      virtual void sessionParameters() = 0;

    private:
      bool locked_ = false;
      Session* session_ = nullptr;
    };

  }
}

#endif /* OMNI_UI_SESSIONWIDGET_H_ */
