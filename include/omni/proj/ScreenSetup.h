#ifndef OMNI_PROJ_SCREENSETUP_H_
#define OMNI_PROJ_SCREENSETUP_H_

#include <vector>
#include <QScreen>

namespace omni
{
  namespace proj
  {
    /// Singleton class which manages the current screen setup
    class ScreenSetup
    {
    public:
      ScreenSetup();

      /// Returns screen size that supports triple heads
      static std::vector<QSize> const& screenResolutions();

      /// Return standard screen, where omnidome has to be placed
      static QScreen const* standardScreen();

      /// Returns the number of subscreens for a single screen
      static int subScreenCount(QScreen const*);
      
      /// Returns the rectangle of a subscreen with a certain index
      static QRect subScreenRect(QScreen const*, int _subScreenIndex);
      static int subScreenWidth(QScreen const*);

      static QRect desktopRect(bool _excludeStandardScreen = true);
      static std::vector<QScreen const*> screens(bool _excludeStandardScreen = true);

      bool operator==(const ScreenSetup&) const;
    };
  }

  using proj::ScreenSetup;
}

#endif /* OMNI_PROJ_SCREENSETUP_H_ */
