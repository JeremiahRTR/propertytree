#ifndef VEC3_H_
#define VEC3_H_

#include <iostream>
#include <tuple>
#include <Math.h>

namespace Dash {

class Vec3 {
public:
  Vec3();
  Vec3(const Vec3& v);
  Vec3(float _x, float _y, float _z);
  Vec3(float _x);

  inline bool operator == (const Vec3& v) const;
  inline bool operator != (const Vec3& v) const;
  inline bool operator < (const Vec3& v) const;
  inline const Vec3& operator = (const Vec3& v);
  inline void operator += (const Vec3& v);
  inline void operator -= (const Vec3& v);
  inline void operator *= (float c);
  inline void operator /= (float c);
  inline float& operator [] (int i);
  inline const float& operator [] (int i) const;
  inline bool IsEqualDir(const Vec3& v, float eps=Epsilon) const;
  inline bool IsEqual(const Vec3& v, float eps=Epsilon) const;
  inline bool IsZero(float eps) const;
  inline float Dot(const Vec3& v) const;
  inline Vec3 Cross(const Vec3& v) const;
  inline float Distance(const Vec3& v) const;
  inline float Length() const;
  inline void Set(const Vec3& v);
  inline void Set(float _x, float _y, float _z);
  inline void Set(float c);
  inline void SetZero();
  inline void InplaceNormalize();
  inline Vec3 EntrywiseMin(const Vec3& v);
  inline Vec3 EntrywiseMax(const Vec3& v);
  inline Vec3 Normal() const;
  inline Vec3 Abs() const;
  inline float MinElement(int& index) const;
  inline float MaxElement(int& index) const;

  union {
    float data[3];
    struct { float x, y, z; };
  };
};

inline Vec3 operator + (const Vec3& a, const Vec3& b);
inline Vec3 operator - (const Vec3& a, const Vec3& b);
inline Vec3 operator * (const Vec3& v, float c);
inline Vec3 operator * (float c, const Vec3& v);
inline Vec3 operator / (const Vec3& v, float c);
std::ostream& operator << (std::ostream& out, const Vec3& v);

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

/* Inline methods */
inline bool Vec3::operator == (const Vec3& v) const { return v.x == x && v.y == y && v.z == z; }
inline bool Vec3::operator != (const Vec3& v) const { return v.x != x || v.y != y || v.z != z; }
inline bool Vec3::operator < (const Vec3& v) const { return std::tie(x, y, z) < std::tie(v.x, v.y, v.z); }
inline const Vec3& Vec3::operator = (const Vec3& v) { Set(v); return *this; }
inline void Vec3::operator += (const Vec3& v) { x += v.x; y += v.y; z += v.z; }
inline void Vec3::operator -= (const Vec3& v) { x -= v.x; y -= v.y; z -= v.z; }
inline void Vec3::operator *= (float c) { x *= c; y *= c; z *=c; }
inline void Vec3::operator /= (float c) { x /= c; y /= c; z /=c; }
inline float& Vec3::operator [] (int i) { return data[i]; }
inline const float& Vec3::operator [] (int i) const  { return data[i]; }
inline bool Vec3::IsEqualDir(const Vec3& v, float eps) const { return FuzzyEquals(Normal().Dot(v.Normal()), 1.0, eps);  }
inline bool Vec3::IsEqual(const Vec3& v, float eps) const { return FuzzyEquals(x, v.x, eps) && FuzzyEquals(y, v.y, eps) && FuzzyEquals(z, v.z, eps); }
inline bool Vec3::IsZero(float eps) const { return FuzzyZero(x, eps) && FuzzyZero(y, eps) && FuzzyZero(z, eps); }
inline float Vec3::Dot(const Vec3& v) const { return x*v.x + y*v.y + z*v.z; }
inline Vec3 Vec3::Cross(const Vec3& v) const { return Vec3(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x); }
inline float Vec3::Distance(const Vec3& v) const { return Sqrt(Sqr(x - v.x) + Sqr(y - v.y) + Sqr(z - v.z)); }
inline float Vec3::Length() const { return Sqrt(Sqr(x) + Sqr(y) + Sqr(z)); }
inline void Vec3::Set(const Vec3& v) { x = v.x; y = v.y; z = v.z; }
inline void Vec3::Set(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }
inline void Vec3::Set(float c) { x = c; y = c; z = c; }
inline void Vec3::SetZero() { x = y = z = 0.0; }
inline void Vec3::InplaceNormalize() { float n = Length(); x /= n; y /= n; z /= n; }
inline Vec3 Vec3::EntrywiseMin(const Vec3& v) { return Vec3(Min(x, v.x), Min(y, v.y), Min(z, v.z)); }
inline Vec3 Vec3::EntrywiseMax(const Vec3& v) { return Vec3(Max(x, v.x), Max(y, v.y), Max(z, v.z)); }
inline Vec3 Vec3::Normal() const { Vec3 temp(*this); temp.InplaceNormalize(); return temp; }
inline Vec3 Vec3::Abs() const { Vec3(fabsf(x), fabsf(y), fabsf(z)); } // TODO: maybe fix
inline float Vec3::MinElement(int& index) const {
  float min_val = x;
  index = 0;
  if (y < min_val) { min_val = y; index = 1; }
  if (z < min_val) { min_val = z; index = 2; }
  return min_val;
}
inline float Vec3::MaxElement(int& index) const {
  float max_val = x;
  index = 0;
  if (y > max_val) { max_val = y; index = 1; }
  if (z > max_val) { max_val = z; index = 2; }
  return max_val;
}

inline float Dot(const Vec3& a, const Vec3& b) { return a.Dot(b); }
inline Vec3 Cross(const Vec3& a, const Vec3& b) { return a.Cross(b); }
inline Vec3 operator + (const Vec3& a, const Vec3& b) { return Vec3(a.x + b.x, a.y + b.y, a.z + b.z); }
inline Vec3 operator - (const Vec3& a, const Vec3& b) { return Vec3(a.x - b.x, a.y - b.y, a.z - b.z); }
inline Vec3 operator * (const Vec3& v, float c) { return Vec3(v.x*c, v.y*c, v.z*c); }
inline Vec3 operator * (float c, const Vec3& v) { return Vec3(v.x*c, v.y*c, v.z*c); }
inline Vec3 operator / (const Vec3& v, float c) { return Vec3(v.x/c, v.y/c, v.z/c); }

} // namespace Dash

#endif // VEC3_H_
