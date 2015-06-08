#ifndef OMNI_VISUAL_WARPGRID_H_
#define OMNI_VISUAL_WARPGRID_H_ 

#include <omni/WarpGrid.h>
#include <omni/visual/Interface.h>
#include <omni/visual/Circle.h>

namespace omni
{
  namespace visual
  {
    /// Draw Handler for WarpGrid
    class WarpGrid : public Interface
    {
    public:
      WarpGrid(omni::WarpGrid const* = nullptr);
      ~WarpGrid();

      /// Draw warp grid
      void draw() const;
      
      /// Generate geometry
      void update();

      omni::WarpGrid const* warpGrid() const;
      
      void setWarpGrid(omni::WarpGrid const*);

    private:
      void visualPatch() const;

      omni::WarpGrid const* warpGrid_ = nullptr;
    };
  }
}

#endif /* OMNI_VISUAL_WARPGRID_H_ */
