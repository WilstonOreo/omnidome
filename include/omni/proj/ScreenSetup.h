#ifndef OMNI_PROJ_SCREENSETUP_H_
#define OMNI_PROJ_SCREENSETUP_H_

#include "Screen.h"

namespace omni
{
  namespace proj
  {
    /// Singleton class which manages the current screen setup
    class ScreenSetup
    {
    public:
      typedef std::vector<Screen> screenlist_type;

      ScreenSetup();

      static ScreenSetup current();
      static ScreenSetup debug();

      static Screen standardScreen();

      screenlist_type const& screens() const;

      bool operator==(const ScreenSetup&) const;
    
    private:
      screenlist_type screens_;
    };
  }

  using proj::ScreenSetup;
}

#endif /* OMNI_PROJ_SCREENSETUP_H_ */
