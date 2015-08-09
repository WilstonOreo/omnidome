#include <omni/dome/Half.h>
#include <omni/canvas/util.hpp>

#include <omni/gl.hpp>

using namespace omni::dome;
using namespace omni;

Half::Half(Scalar _diameter) :
  diameter_(_diameter)
{
  generate();
}

bool Half::intersect(const Ray& _ray, Point3& _point, Vec3* _normal) const
{
  return canvas::util::sphereIntersection(Point3(0,0,0),radius(),_ray,_point,_normal);
}

omni::Scalar Half::radius() const
{
  return diameter_*0.5005;
}

void Half::update()
{
  generate();
}

std::set<std::string> Half::supportedSetups() const
{
  return {"RingArray","RingArrayOneTop","SingleProjector"};
}

void Half::drawCanvas() const
{
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);

  glTexCoordPointer(2, GL_FLOAT, 0, texCoords_.data());
  glNormalPointer(GL_FLOAT, 0, normals_.data());
  glVertexPointer(3, GL_FLOAT, 0, vertices_.data());

  glDrawElements(GL_TRIANGLES,indices_.size(),GL_UNSIGNED_INT,indices_.data());

  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
}

void Half::drawAux() const
{
  std::vector<float> _radii;
  float _radius = radius();
  for (size_t i = 1; i <= _radius; ++i)
  {
    _radii.push_back(i);
  }
  _radii.push_back(radius());

  glLineWidth(1.0);
  size_t i = 0;
  for (auto& _r : _radii)
  {
    glBegin(GL_LINE_LOOP);
    glColor4f(1.0,1.0,1.0,(i % 2 || _r == _radius) ? 1.0 : 0.5);
    canvas::util::for_each_circle_point(64,_r,[&](size_t i, const Point2& _p)
    {
      glVertex3f(_p.x(),_p.y(),0.0);
    });
    glEnd();
    ++i;
  }
}


Vec2 Half::getTexCoords(const Point3& _point, const Vec3& _normal, MapMode _mapMode) const
{
  return texcoords::get(_mapMode,_normal);
}


std::set<omni::MapMode> Half::supportedMapModes() const
{
  return {MapMode::FISHEYE,MapMode::CUBEMAP,MapMode::SPHERICAL};
}


Half::bounds_type Half::bounds() const
{
  return bounds_;
}

bool Half::load(const ConfigPath& _path, Config const& _config)
{
  bool _updated = Serializer<Half>::load(_path,_config);
  if (_updated) generate();
  return _updated;
}

void Half::additionalParameters(Config& _config) const
{
  using namespace tbd::parameter;
  make(*this,_config,"diameter",
  {
    {"min", "0.5"},
    {"max", "50.0"}
  });
}

void Half::generate()
{
  bounds_ = bounds_type();
  const int _stacks = 64;
  const int _slices = 128;
  indices_.clear();
  vertices_.clear();
  normals_.clear();
  texCoords_.clear();

  std::vector<Vertex> _vertices;

  canvas::util::generateSphereUpper(_stacks,_slices,0,_vertices,indices_);
  for (auto& _vertex : _vertices)
  {
    _vertex.pos() *= radius();
    bounds_.extend(_vertex.pos());
    vertices_.push_back(_vertex.pos());
    normals_.push_back(_vertex.normal());
    texCoords_.push_back(_vertex.texCoord());
  }
}

