#include <omni/visual/WarpGrid.h>

namespace omni
{
  namespace visual
  {
    WarpGrid::WarpGrid(omni::WarpGrid const* _warpGrid) :
      warpGrid_(_warpGrid)
    {
      update();
    }

    WarpGrid::~WarpGrid()
    {
    }

    void WarpGrid::draw() const
    {
    }
    
    void WarpGrid::update()
    {

    }
      
    omni::WarpGrid const* WarpGrid::warpGrid() const
    {
      return warpGrid_;
    }
      
    void WarpGrid::setWarpGrid(omni::WarpGrid const* _warpGrid)
    {
      warpGrid_ = _warpGrid;
      update();
    }

    void WarpGrid::visualPatch() const
    {
      for (int y = 0; y < warpGrid_->vertical()-1; ++y)
      {
        glBegin(GL_QUAD_STRIP);
        for (int x = 0; x < warpGrid_->horizontal(); ++x)
        {
          auto& _p0 = *(warpGrid_->getPoint(x,y));
          auto& _py = *(warpGrid_->getPoint(x,y+1));
          auto _texCoord0 = warpGrid_->getTexCoord(x,y);
          auto _texCoordY = warpGrid_->getTexCoord(x,y+1);
          glTexCoord2f(_texCoord0.x(), _texCoord0.y());
          glVertex2f(_p0.x(),_p0.y());
          glTexCoord2f(_texCoordY.x(), _texCoordY.y());
          glVertex2f(_py.x(),_py.y());
        }
        glEnd();
      }
    }
  }
}
