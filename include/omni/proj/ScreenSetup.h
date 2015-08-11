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

      static QScreen const* standardScreen();
      static int subScreenCount(QScreen const*);
      static QRect subScreenRect(QScreen const*, int _subScreenIndex);
      static int subScreenWidth(QScreen const*);

      static QRect desktopRect(bool _excludeStandardScreen = false);
      static std::vector<QScreen const*> screens(bool _excludeStandardScreen = false);

      bool operator==(const ScreenSetup&) const;
    };
  }

  using proj::ScreenSetup;
}

#endif /* OMNI_PROJ_SCREENSETUP_H_ */
