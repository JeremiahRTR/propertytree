#include <Vec3.h>

using namespace std;

namespace Dash {

Vec3::Vec3() {
  SetZero();
}


Vec3::Vec3(const Vec3& v) {
  Set(v);
}


Vec3::Vec3(float _x, float _y, float _z) {
  Set(_x, _y, _z);
}


Vec3::Vec3(float c) {
  Set(c);
}


ostream& operator << (ostream& out, const Vec3& v) {
  out << v.x << " " << v.y << " " << v.z;
  return out;
}

} // namespace Dash
