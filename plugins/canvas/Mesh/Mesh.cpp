#include "Mesh.h"
#include "MeshWidget.h"

#include <vector>
#include <QOpenGLContext>

#include <OpenMesh/Core/IO/MeshIO.hh>

#include <omni/visual/util.h>
#include <omni/visual/Scene.h>
#include <omni/util.h>

namespace omni {
    namespace canvas {
        Mesh::Mesh()
        {
        }

        void Mesh::draw() const {
          if (!vertexCount_ || !vbo_.id()) return; // Vertex Buffer Object has not generated yet, so skip

            withCurrentContext([&](QOpenGLFunctions& _)
            {
                _.glBindBuffer(GL_ARRAY_BUFFER, vbo_.id());

                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                glEnableClientState(GL_NORMAL_ARRAY);
                glEnableClientState(GL_VERTEX_ARRAY);

                glTexCoordPointer(3, GL_FLOAT, sizeof(Vertex),(void*)Vertex::texCoordOffset());
                glNormalPointer(GL_FLOAT, sizeof(Vertex), (void*)Vertex::normalOffset());
                glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (void*)Vertex::posOffset());
                _.glDrawArrays(GL_TRIANGLES,0,vertexCount_);

                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                glDisableClientState(GL_NORMAL_ARRAY);
                glDisableClientState(GL_VERTEX_ARRAY);

                _.glBindBuffer(GL_ARRAY_BUFFER, 0 );
            });
        }

        void Mesh::update() {
          if (!needsUpdate_ || !QOpenGLContext::currentContext()) return;

          /// Update bounding box first to center the mesh later
          updateBoundingBox();

          std::vector<Vertex> _vertices;
          _vertices.reserve(3 * mesh_.n_faces());

          /// Extract face vertices from mesh and put them into buffer
          std::vector<Vertex> _faceVertices;
          for (auto f_it=mesh_.faces_begin(); f_it!=mesh_.faces_end(); ++f_it) {
            _faceVertices.clear();
            for (auto cfv =  mesh_.cfv_iter(*f_it); cfv.is_valid(); ++cfv ) {

              auto _pos = mesh_.point(*cfv);
              auto _normal = mesh_.normal(*cfv);
              auto _texCoord = mesh_.texcoord2D(*cfv);

              _faceVertices.emplace_back(
                QVector3D(_pos[0],_pos[1],_pos[2]), // Position
                QVector3D(_normal[0],_normal[1],_normal[2]), // normal
                QVector2D(_texCoord[0],_texCoord[1])); // tex coord
            }

            if (_faceVertices.size() != 3) continue;

            for (auto& _faceVertex : _faceVertices) {
              _vertices.push_back(_faceVertex);
            }
          }

          vertexCount_ = _vertices.size();

          float _sceneSize = 1.0f; //scene()->size();

          int _largestExtentAxis = 0;
          if (bounds_.size().y() > bounds_.size().x()) {
            _largestExtentAxis = 1;
          }
          if (bounds_.size().z() > bounds_.size().y()) {
            _largestExtentAxis = 2;
          }
          float _largestExtent = bounds_.size()[_largestExtentAxis];

          float _scaleFactor = 1.0;

          if (scaleToScene_) {
            /// Calculate scene scale factor
            _scaleFactor = _sceneSize / _largestExtent;
            int _mag = int(std::log10(_scaleFactor) - 0.5);
            _scaleFactor = std::pow(10,_mag);
          }

          if (centerToScene_) {
            for (auto& _vertex : _vertices) {
              _vertex.setPos(_scaleFactor * (_vertex.pos() - bounds_.center()));
            }
            updateBoundingBox(_vertices);
            for (auto& _vertex : _vertices) {
              _vertex.setPos(_vertex.pos() - QVector3D(0,0,bounds_.min().z()));
            }
          }
          updateBoundingBox(_vertices);

          primaryContextSwitch([&](QOpenGLFunctions& _) {
            vbo_.freeAndGen();
            vbo_.bufferStaticArray(_vertices);
          });

          qDebug() << "Mesh has" << vertexCount_ << "vertices.";
          qDebug() << bounds_.min() << " " << bounds_.max();

          needsUpdate_ = false;
        }

        Box  Mesh::bounds() const {
          return bounds_;
        }

        QVector3D Mesh::center() const {
            auto _center = bounds_.center();
            return QVector3D(_center.x(),_center.y(),bounds_.min().z());
        }

        void Mesh::load(QString const& _filename) {
          meshFile_.setPath(_filename);
          if (!OpenMesh::IO::read_mesh(mesh_,meshFile_.path().toStdString())) {
            throw QException();
          }
          mesh_.request_vertex_normals();
          mesh_.request_face_normals();
          mesh_.request_vertex_texcoords2D();
          mesh_.update_normals();
          vertexCount_ = 0;
          needsUpdate_ = true;
          update();
        }

        void Mesh::fromPropertyMap(PropertyMap const& _map)
        {
            canvas::Interface::fromPropertyMap(_map);
            _map.get("scaleToScene",scaleToScene_);
            _map.get("centerToScene",centerToScene_);
            _map.get("meshFile",meshFile_);
            if (!meshFile_.path().isEmpty())
              load(meshFile_.path());
            update();
        }

        void Mesh::toPropertyMap(PropertyMap& _map) const
        {
          _map("scaleToScene",scaleToScene_);
          _map("centerToScene",centerToScene_);
          _map("meshFile",meshFile_);
          canvas::Interface::toPropertyMap(_map);
        }

        ui::CanvasParameters* Mesh::widget() {
            return new omni::ui::canvas::Mesh;
        }

        void Mesh::updateBoundingBox() {
          bounds_ = omni::Box();
          for (auto v_it=mesh_.vertices_begin(); v_it!=mesh_.vertices_end(); ++v_it) {
            auto _pos = mesh_.point(*v_it);
            bounds_.extend(QVector3D(_pos[0],_pos[1],_pos[2]));
          }
        }
        void Mesh::updateBoundingBox(std::vector<Vertex> const& _vertices) {
          bounds_ = omni::Box();
          for (auto& _vertex : _vertices) {
            bounds_.extend(_vertex.pos());
          }
        }

        /// Center To Scene
        void  Mesh::setCenterToScene(bool _enabled) {
          centerToScene_ = _enabled;
          needsUpdate_ = true;
          update();
        }

        bool  Mesh::centerToScene() const {
          return centerToScene_;
        }

        /// Scele To Scene
        void  Mesh::setScaleToScene(bool _enabled) {
          scaleToScene_ = _enabled;
          needsUpdate_ = true;
          update();
        }

        bool  Mesh::scaleToScene() const {
          return scaleToScene_;
        }
    }
}
