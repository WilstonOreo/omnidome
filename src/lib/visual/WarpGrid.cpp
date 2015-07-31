#include <omni/visual/WarpGrid.h>

#include <QPointF>
#include <QRectF>
#include <QColor>
#include <omni/visual/util.h>

namespace omni
{
  namespace visual
  {
    WarpGrid::WarpGrid(omni::WarpGrid const& _warpGrid) :
      warpGrid_(_warpGrid)
    {
      update();
    }

    WarpGrid::~WarpGrid()
    {
    }

    void WarpGrid::draw() const
    {
        for (int y = 0; y < warpGrid_.vertical()-1; ++y)
        {
          glBegin(GL_QUAD_STRIP);
          for (int x = 0; x < warpGrid_.horizontal(); ++x)
          {
            auto _p0 = warpGrid_.getPoint(x,y);
            auto _py = warpGrid_.getPoint(x,y+1);
            auto _texCoord0 = warpGrid_.getTexCoord(x,y);
            auto _texCoordY = warpGrid_.getTexCoord(x,y+1);

            glTexCoord2f(_texCoord0.x(), 1.0 - _texCoord0.y());
            glVertex2f(_p0->x(),_p0->y());
            glTexCoord2f(_texCoordY.x(), 1.0 - _texCoordY.y());
            glVertex2f(_py->x(),_py->y());
          }
          glEnd();
        }
    }

    void WarpGrid::drawLines()
    {
      visual::with_current_context([&](QOpenGLFunctions& _)
      {
        _.glEnable(GL_BLEND);
        glPolygonMode(GL_FRONT,GL_LINE);
        glPolygonMode(GL_BACK,GL_LINE);
        glColor4f(1.0,1.0,1.0,0.2);
        glLineWidth(2.0);
        draw();
        glPolygonMode(GL_FRONT,GL_FILL);
        glPolygonMode(GL_BACK,GL_FILL);
      });
    };

    void WarpGrid::drawHandles(QColor const& _color, QRectF const& _rect)
    {
      constexpr size_t _numVertices = 16;
      const float _radius = 0.025;
      std::array<QPointF,_numVertices> _circlePoints;
      util::for_each_circle_point(_numVertices,_radius,[&](size_t i, const QPointF& _p)
      {
        _circlePoints[i] = QPointF(_p.x() * _rect.width(),_p.y() * _rect.height());
      });

      visual::with_current_context([&](QOpenGLFunctions& _)
      {
        _.glEnable(GL_BLEND);
        for (auto& _point : warpGrid_.points())
        {
          glBegin(GL_LINE_LOOP);
          glColor4f(1.0,1.0,1.0,0.5);
          for (auto& _p : _circlePoints)
            glVertex2f(_p.x() + _point.x(),_p.y() + _point.y());
          glEnd();
          if (_point.selected())
          {
            glBegin(GL_TRIANGLE_FAN);
            glColor4f(_color.redF(),_color.greenF(),_color.blueF(),0.5);
            glVertex2f(_point.x(),_point.y());
            for (auto& _p : _circlePoints)
            {
              glVertex2f(_p.x() + _point.x(),_p.y() + _point.y());
            }
            auto& _p = _circlePoints[0];
            glVertex2f(_p.x() + _point.x(),_p.y() + _point.y());
            glEnd();
          }
        }
      });
    }

    void WarpGrid::update()
    {
    }
  }
}
