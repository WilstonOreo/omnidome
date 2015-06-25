#ifndef OMNI_VISUAL_PROJECTOR
#define OMNI_VISUAL_PROJECTOR

#include <QColor>
#include <QVector3D>
#include <omni/visual/Interface.h>
#include <omni/proj/Projector.h>

namespace omni
{
  namespace visual
  {
    class Projector : public Interface
    {
    public:
      Projector(const proj::Projector&);
      ~Projector() {}

      void update();

      qreal size() const;
      void setSize(qreal);

      QColor color() const;
      void setColor(QColor);

      void draw() const;
      void drawHalo() const;

    private:

      proj::Projector const& proj_;

      float size_ = 2.0;

      QColor color_;
      QVector3D eye_, topLeft_, topRight_, bottomLeft_, bottomRight_;
    };
  }
}

#endif /* OMNI_VISUAL_PROJECTOR */
