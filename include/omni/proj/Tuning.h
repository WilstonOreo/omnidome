#ifndef OMNI_PROJ_TUNING_H_
#define OMNI_PROJ_TUNING_H_

#include <memory>
#include <QColor>
#include <omni/Id.h>
#include <omni/WarpGrid.h>
#include <omni/BlendMask.h>
#include <omni/proj/Projector.h>
#include <omni/proj/Setup.h>
#include <omni/proj/Screen.h>

namespace omni
{
  namespace proj
  {
    /**@brief A projector tuning holds adjustment and distorsion data for a single projector and screen
     **/
    class Tuning
    {
    public:
      /// Constructor
      Tuning();

      /// Constructor with a color
      Tuning(QColor const&);

      void setScreen(Screen const* _screen);
      Screen const* screen() const;

      Projector const& projector() const;

      /**@brief Make a new projector with a certain id and delete old one
       * @return Pointer to new projector setup
       **/
      ProjectorSetup* setupProjector(Id const& _setupId);

      /// Update projector matrix by current setup
      void setupProjector();

      /// Returns projector setup
      ProjectorSetup* projectorSetup();

      /// Returns projector setup
      ProjectorSetup const* projectorSetup() const;

      /// Returns reference to warp grid
      WarpGrid& warpGrid();

      /// Returns reference to warp grid (const version)
      WarpGrid const& warpGrid() const;

      /// Returns reference to blend mask
      BlendMask& blendMask();

      /// Returns reference to blend mask (const version)
      BlendMask const& blendMask() const;

      /// Returns color for mapping
      QColor color() const;

      /// Sets color for mapping
      void setColor(QColor const&);

      /// Return if a screen is associated with mapping
      bool hasScreen() const;

      /// Returns true if screen in argument has a different resolution than given one
      bool resolutionToBeChanged(proj::Screen const* _screen) const;

      /// Returns width of screen
      int width() const;

      /// Returns height of screen
      int height() const;

      friend bool operator==(Tuning const&,Tuning const&);

    private:
      QColor color_;
      Projector projector_;
      std::unique_ptr<ProjectorSetup> projectorSetup_;

      WarpGrid warpGrid_;
      BlendMask blendMask_;
    };
  }
}

QDataStream& operator>>(QDataStream&, omni::proj::Tuning&);
QDataStream& operator<<(QDataStream&, omni::proj::Tuning const&);

#endif /* OMNI_MAPPING_H_ */
