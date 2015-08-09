#include <omni/dome/Inflatable.h>

#include <omni/gl.hpp>

using namespace omni;
using namespace omni::dome;

Inflatable::Inflatable(Scalar _diameter) :
  diameter_(_diameter),
  distance_to_pole_(5.0),
  distance_to_equator_(5.0)
{
  generate();
}

Scalar Inflatable::zRadius() const
{
  return distance_to_pole_ - zOffset();
}

void Inflatable::update()
{
  generate();
}

Scalar Inflatable::zOffset() const
{
  return sqrt(abs(distance_to_equator()*distance_to_equator() - radius()*radius()));
}

Scalar Inflatable::baseDiameter() const
{
  return radius()*1.5;
}

bool Inflatable::intersect(const Ray& _ray, Point3& _point, Vec3* _normal) const
{
  Scalar _radius = radius();
  Scalar a = _ray.dir().dot(_ray.dir());
  Scalar b = 2.0 * _ray.dir().dot(_ray.org());
  Scalar c = _ray.org().dot(_ray.org()) - _radius*_radius;
  Scalar t = base::solveQuadraticEquation(a,b,c);
  if (t < _radius * 0.001) return false;

  _point = _ray.org() + t * _ray.dir();
  if (_normal)
  {
    *_normal = _point.normalized();
  }
  return true;
}

Vec2 Inflatable::getTexCoords(const Point3& _point, const Vec3& _normal, MapMode _mapMode) const
{
  return texcoords::get(_mapMode,_normal);
}


Scalar Inflatable::radius() const
{
  return diameter_*0.5005;
}

Inflatable::bounds_type Inflatable::bounds() const
{
  return bounds_;
}

void Inflatable::drawCanvas() const
{
  glColor4f(1.0,1.0,1.0,0.5);
  glBegin(GL_TRIANGLES);
  size_t _n = indices_.size() / 3 * 3;
  for (size_t i = 0; i < _n; i += 3)
  {
    auto& _v0 = vertices_[indices_[i]];
    auto& _v1 = vertices_[indices_[i+1]];
    auto& _v2 = vertices_[indices_[i+2]];
    glNormal3f(_v0.normal().x(),_v0.normal().y(),_v0.normal().z());
    glVertex3f(_v0.pos().x(),_v0.pos().y(),_v0.pos().z());
    glNormal3f(_v1.normal().x(),_v1.normal().y(),_v1.normal().z());
    glVertex3f(_v1.pos().x(),_v1.pos().y(),_v1.pos().z());
    glNormal3f(_v2.normal().x(),_v2.normal().y(),_v2.normal().z());
    glVertex3f(_v2.pos().x(),_v2.pos().y(),_v2.pos().z());
  }
  glEnd();
}

void Inflatable::drawAux() const
{
  std::vector<float> _radii;
  float _radius = baseDiameter() * 0.5;
  for (size_t i = 1; i <= _radius; ++i)
  {
    _radii.push_back(i);
  }
  _radii.push_back(_radius);

  glLineWidth(1.0);
  size_t i = 0;
  for (auto& _r : _radii)
  {
    glBegin(GL_LINE_LOOP);
    glColor4f(1.0,1.0,1.0,(i % 2 || _r == _radius) ? 1.0 : 0.5);
    util::for_each_circle_point(64,_r,[&](size_t i, const Point2& _p)
    {
      glVertex3f(_p.x(),_p.y(),0.0);
    });
    glEnd();
    ++i;
  }

}

std::string Inflatable::intersectionShaderCode() const
{
  std::string _s;
  _s += "float a = dot(ray.dir,ray.dir);";
  _s += "float b = 2.0 * dot(ray.dir,ray.org);";
  _s += "float c = dot(ray.org,ray.org) - sqr(diameter*0.5);";
  _s += "float t; float disc = b*b - 4.0 * a * c;";
  _s += "if (disc < 0.0) return -1.0; float distSqrt = sqrt(disc);";
  _s += "float q; if (b < 0.0) { q = (-b - distSqrt)*0.5; }";
  _s += "else { q = (-b + distSqrt)*0.5; }";
  _s += "if (q == 0.0) return -1.0; float t = c / q;";
  _s += "if (t < 0.0) t = q / a;";
  _s += "if (t < radius * 0.001) return -1.0;";
  _s += "iPoint = ray.org + t * ray.dir; return 1.0;";
  return _s;
}


bool Inflatable::load(const ConfigPath& _path, Config const& _config)
{
  bool _updated = Serializer<Inflatable>::load(_path,_config);
  if (_updated) generate();
  return _updated;
}

std::set<MapMode> Inflatable::supportedMapModes() const
{
  return {MapMode::CUBEMAP,MapMode::SPHERICAL};
}

void Inflatable::generate()
{
  bounds_ = bounds_type();
  const int _stacks = 32;
  const int _slices = 64;
  indices_.clear();
  vertices_.clear();
  util::generateSphereUpper(_stacks,_slices,-0.3,vertices_,indices_);
  /// Top cone
  auto stackRadius = [&_stacks](size_t index)
  {
    return sin(M_PI * float(index) / _stacks);
  };
  auto stackPos = [&_stacks](size_t index)
  {
    return cos(M_PI * float(index) / _stacks);
  };
  util::generateCone(_slices,1.0,stackPos(1),stackRadius(1),vertices_,indices_);

  for (size_t i = 1; i < _stacks-1; ++i)
  {
    if (stackRadius(i+1) < radius() / baseDiameter() * 2.0)
    {
      continue;
    }

    float _stackPos = stackPos(i+1);
    util::generateSphereStack(_slices,
                              stackPos(i),stackPos(i+1),
                              stackRadius(i),stackRadius(i+1),
                              vertices_,indices_);
  }

  for (auto& _vertex : vertices_)
  {
    auto& _p = _vertex.pos();
    _p = Point3(_p.x()*radius(),_p.y()*radius(),_p.z()*zRadius());
    _p.z() += zOffset();
    bounds_.extend(_vertex.pos());
  }
}

