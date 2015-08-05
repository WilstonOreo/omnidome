#ifndef OMNI_PROJ_SCREEN_H_
#define OMNI_PROJ_SCREEN_H_

#include <QScreen>

namespace omni
{
  namespace proj
  {
    /// Screen class 
    struct Screen
    {
      /// Screen takes a QScreen and an index as input
      Screen(QRect const&, int _index = 0);

      /// Returns true if number of subscreens is larger than 1
      bool hasSubScreens() const;

      /// Aspect ratio  
      qreal aspectRatio() const;
      
      /// Screen width
      int width() const;

      /// Screen height
      int height() const;

      /// Calculate number of subscreens, for triple head detection
      int subScreenCount() const;

      /// Calculate number of subscreens, for triple head detection from a given rectangle
      static int subScreenCount(QRect const&);

      /// Screen rectangle (absolute positioning)
      QRect rect() const;

    private:
      QRect rect_;
      int index_;
    };
  }

  using proj::Screen;
}

#endif /* OMNI_PROJ_SCREEN_H_ */
