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
