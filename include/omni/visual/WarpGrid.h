#ifndef OMNI_VISUAL_WARPGRID_H_
#define OMNI_VISUAL_WARPGRID_H_ 

#include <omni/WarpGrid.h>
#include <omni/visual/Interface.h>
#include <omni/visual/Circle.h>
#include <omni/visual/VBO.h>

namespace omni
{
  namespace visual
  {
    /// Draw Handler for WarpGrid
    class WarpGrid : public Interface
    {
    public:
      WarpGrid(omni::WarpGrid const& _warpGrid);
      ~WarpGrid();

      /// Draw warp grid
      void draw() const;
      
      /// Draw lines for warp grid
      void drawLines();

      /// Draw handles for warp grid
      void drawHandles(QColor const& _color, QRectF const& _rect);

      /// Generate geometry
      void update();

    private:

      omni::WarpGrid const& warpGrid_;
    };
  }
}

#endif /* OMNI_VISUAL_WARPGRID_H_ */
