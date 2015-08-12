#include <omni/visual/Box.h>

#include <omni/visual/util.h>
#include <QDebug>

namespace omni
{
  namespace visual
  {

    Box::Box(omni::Box const& _box, bool _wireframe) :
      box_(_box),
      wireframe_(_wireframe)
    {
      update();
    }

    Box::~Box()
    {
    }
      
    void Box::update() 
    {
      vertexVbo_.freeAndGen();
      indexVbo_.freeAndGen();
      
      vertices_.clear();
      vertices_.reserve(24);
      indices_.clear();
      indices_.reserve(36);

      enum Side { 
        POS_X,
        POS_Y,
        POS_Z,
        NEG_X,
        NEG_Y,
        NEG_Z 
      };

      auto getNormal = [](Side _side)
      {
        switch (_side)
        {
        case POS_X: return QVector3D(1,0,0);
        case POS_Y: return QVector3D(0,1,0);
        case POS_Z: return QVector3D(0,0,1);
        case NEG_X: return QVector3D(-1,0,0);
        case NEG_Y: return QVector3D(0,-1,0);
        case NEG_Z: return QVector3D(0,0,-1);
        default: break;
        }
        return QVector3D(0,0,0);
      };
      
      auto _min = box_.min();
      auto _max = box_.max();

      auto add = [&](QVector3D _pos, QVector3D _normal)
      {
        auto _center = (_max + _min) * 0.5;
/*
        QVector3D _invSize(
            1.0 / (_max.x() - _min.x()),
            1.0 / (_max.y() - _min.y()),
            1.0 / (_max.z() - _min.z()));

        auto _uvw = ((_pos - _center) * _invSize) * 2.0;
        vertices_.emplace_back(_pos,_normal,_uvw); */
        vertices_.emplace_back(_pos,_normal,(_pos - _center).normalized());
      };

      auto addIndices = [&]()
      {
        size_t offset = vertices_.size();
        indices_.push_back(offset+0);
        indices_.push_back(offset+1);
        indices_.push_back(offset+2);
        indices_.push_back(offset+0);
        indices_.push_back(offset+3);
        indices_.push_back(offset+2);
      };

      addIndices();
      add(QVector3D(_max.x(),_min.y(),_min.z()),getNormal(POS_X));
      add(QVector3D(_max.x(),_min.y(),_max.z()),getNormal(POS_X));
      add(QVector3D(_max.x(),_max.y(),_max.z()),getNormal(POS_X));
      add(QVector3D(_max.x(),_max.y(),_min.z()),getNormal(POS_X));

      addIndices();
      add(QVector3D(_min.x(),_min.y(),_min.z()),getNormal(NEG_X));
      add(QVector3D(_min.x(),_min.y(),_max.z()),getNormal(NEG_X));
      add(QVector3D(_min.x(),_max.y(),_max.z()),getNormal(NEG_X));
      add(QVector3D(_min.x(),_max.y(),_min.z()),getNormal(NEG_X));
      
      addIndices();
      add(QVector3D(_min.x(),_max.y(),_min.z()),getNormal(POS_Y));
      add(QVector3D(_min.x(),_max.y(),_max.z()),getNormal(POS_Y));
      add(QVector3D(_max.x(),_max.y(),_max.z()),getNormal(POS_Y));
      add(QVector3D(_max.x(),_max.y(),_min.z()),getNormal(POS_Y));
      
      addIndices();
      add(QVector3D(_min.x(),_min.y(),_min.z()),getNormal(NEG_Y));
      add(QVector3D(_min.x(),_min.y(),_max.z()),getNormal(NEG_Y));
      add(QVector3D(_max.x(),_min.y(),_max.z()),getNormal(NEG_Y));
      add(QVector3D(_max.x(),_min.y(),_min.z()),getNormal(NEG_Y));
      
      addIndices();
      add(QVector3D(_min.x(),_min.y(),_max.z()),getNormal(POS_Z));
      add(QVector3D(_min.x(),_max.y(),_max.z()),getNormal(POS_Z));
      add(QVector3D(_max.x(),_max.y(),_max.z()),getNormal(POS_Z));
      add(QVector3D(_max.x(),_min.y(),_max.z()),getNormal(POS_Z));
      
      addIndices();
      add(QVector3D(_min.x(),_min.y(),_min.z()),getNormal(NEG_Z));
      add(QVector3D(_min.x(),_max.y(),_min.z()),getNormal(NEG_Z));
      add(QVector3D(_max.x(),_max.y(),_min.z()),getNormal(NEG_Z));
      add(QVector3D(_max.x(),_min.y(),_min.z()),getNormal(NEG_Z));
      
      with_current_context([this](QOpenGLFunctions& _)
      {
        // bind VBO in order to use
        _.glBindBuffer(GL_ARRAY_BUFFER, vertexVbo_.id());
        {
          _.glBufferData(GL_ARRAY_BUFFER, vertices_.size()*sizeof(Vertex), vertices_.data(), GL_STATIC_DRAW);
        }
        _.glBindBuffer(GL_ARRAY_BUFFER,0);

        _.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVbo_.id());
        {
          _.glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size()*sizeof(GLuint), indices_.data(), GL_STATIC_DRAW);
        }
        _.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      });

      vertices_.clear();
    }

    void Box::draw(omni::Box const& _box, bool _wireframe)
    {
      float x = _box.min().x(), y = _box.min().y(), z = _box.min().z();
      float xs = _box.max().x(), ys = _box.max().y(), zs = _box.max().z();
      GLuint _visualMode = _wireframe ? GL_LINE_LOOP : GL_QUADS;

      glBegin(_visualMode);
      {
        // top side
        glNormal3f(0.0,1.0,0.0);
        glTexCoord2f(0,0);
        glVertex3f(x , ys, z );
        glTexCoord2f(1,0);
        glVertex3f(xs, ys, z );
        glTexCoord2f(1,1);
        glVertex3f(xs, ys, zs);
        glTexCoord2f(0,1);
        glVertex3f(x , ys, zs);
      }
      glEnd();
      glBegin(_visualMode);
      {
        // bottom side
        glNormal3f(0.0,-1.0,0.0);
        glTexCoord2f(0,0);
        glVertex3f(x , y, z );
        glTexCoord2f(1,0);
        glVertex3f(xs, y, z );
        glTexCoord2f(1,1);
        glVertex3f(xs, y, zs);
        glTexCoord2f(0,1);
        glVertex3f(x , y, zs);
      }
      glEnd();
      glBegin(_visualMode);
      {
        // east side
        glNormal3f(0.0,0.0,-1.0);
        glTexCoord2f(0,0);
        glVertex3f(x , y , z);
        glTexCoord2f(1,0);
        glVertex3f(xs, y , z);
        glTexCoord2f(1,1);
        glVertex3f(xs, ys, z);
        glTexCoord2f(0,1);
        glVertex3f(x , ys, z);
      }
      glEnd();
      glBegin(_visualMode);
      // west side
      {
        glNormal3f(0.0,0.0,1.0);
        glTexCoord2f(0,0);
        glVertex3f(x , y ,zs);
        glTexCoord2f(1,0);
        glVertex3f(xs, y ,zs);
        glTexCoord2f(1,1);
        glVertex3f(xs, ys,zs);
        glTexCoord2f(0,1);
        glVertex3f(x , ys,zs);
      }
      glEnd();
      glBegin(_visualMode);
      {
        // north side
        glNormal3f(-1.0,0.0,0.0);
        glTexCoord2f(0,0);
        glVertex3f(x , y , z );
        glTexCoord2f(1,0);
        glVertex3f(x , y , zs);
        glTexCoord2f(1,1);
        glVertex3f(x , ys, zs);
        glTexCoord2f(0,1);
        glVertex3f(x , ys, z );
      }
      glEnd();
      glBegin(_visualMode);
      {
        // south side
        glNormal3f( 1.0,0.0,0.0);
        glTexCoord2f(0,0);
        glVertex3f( xs, y , z );
        glTexCoord2f(1,0);
        glVertex3f( xs, y , zs);
        glTexCoord2f(1,1);
        glVertex3f( xs, ys, zs);
        glTexCoord2f(0,1);
        glVertex3f( xs, ys, z );
      }
      glEnd();
    }

    void Box::draw() const
    {
      glPushMatrix();

      with_current_context([this](QOpenGLFunctions& _)
      {
        _.glBindBuffer(GL_ARRAY_BUFFER, vertexVbo_.id());
        _.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVbo_.id());

        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_VERTEX_ARRAY);

        glTexCoordPointer(3, GL_FLOAT, sizeof(Vertex),(void*)Vertex::texCoordOffset());
        glNormalPointer(GL_FLOAT, sizeof(Vertex), (void*)Vertex::normalOffset());
        glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (void*)Vertex::posOffset());

        _.glDrawElements(GL_TRIANGLES,indices_.size(),GL_UNSIGNED_INT,0);

        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
      
        _.glBindBuffer(GL_ARRAY_BUFFER, 0 );
        _.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      });
      glPopMatrix();
    }
  }
}

