#ifndef MATH_H_
#define MATH_H_

#include <math.h>
#include <float.h>
#include <limits>

namespace Dash {

////////////////////////////////////////////////////////////////////////////////
// Floats
////////////////////////////////////////////////////////////////////////////////

const static float Inf = std::numeric_limits<float>::infinity();
const static float Zero = 0.0f;
const static float One = 1.0f;
const static float Two = 2.0f;
const static float Half = 0.5;
const static float E = 2.718281828f;
const static float Pi = 3.141592654f;
const static float TwoPi = 6.283185307f;
const static float Pi_2 = 1.5707963267f;
const static float Pi_4 = 0.7853981634f;
const static float Sqrt2 = 1.4142135624f;
const static float Sqrt1_2 = 0.70710678119f;
const static float Epsilon = 1e-5f;
const static float DtoRConst = 0.0174532925f;
const static float RtoDConst = 57.29577951f;

inline float Min(float a, float b) { return ((a < b) ? a : b); }
inline float Max(float a, float b) { return ((a > b) ? a : b); }
inline float Min(float a, float b, float c) { return Min(Min(a, b), c); }
inline float Max(float a, float b, float c) { return Max(Max(a, b), c); }
inline float Abs(float x) { return fabsf(x); }
inline float Sqr(float x) { return x*x; }
inline float Sqrt(float x) { return sqrtf(x); }
inline float Cube(float x) { return x*x*x; }
inline float Cbrt(float x) { return cbrtf(x); }
inline float Exp(float x) { return expf(x); }
inline float Log(float x) { return logf(x); }
inline float Pow(float x, float y) { return powf(x,y); }
inline float Sin(float x) { return sinf(x); }
inline float Cos(float x) { return cosf(x); }
inline float Tan(float x) { return tanf(x); }
inline float Asin(float x) { return asinf(x); }
inline float Acos(float x) { return acosf(x); }
inline float Atan(float x) { return atanf(x); }
inline float Atan2(float y, float x) { return atan2f(y, x); }
inline float Floor(float x) { return floorf(x); }
inline float Ceil(float x) { return ceilf(x); }
inline float Round(float x) { return roundf(x); }
inline float Mod(float x, float y) { return fmodf(x, y); }
inline float Clamp(float x, float a, float b) { return ((x < a) ? (a) : ((x > b) ? b : x)); }
inline float Map(float x, float in_min, float in_max, float out_min, float out_max) { return out_min + (x - in_min)*(out_max - out_min)/(in_max - in_min); }
inline bool FuzzyEquals(float a, float b, float eps=Epsilon) { return Abs(a-b) <= eps; }
inline bool FuzzyZero(float a, float eps=Epsilon) { return Abs(a) <= eps; }
inline float DtoR(float f) { return f*DtoRConst; }
inline float RtoD(float f) { return f*RtoDConst; }
inline float Inv(float x) { return One/x; }
/// Returns 1/x if x is nonzero, otherwise 0
inline float PseudoInv(float x, float eps=Zero) { return (FuzzyZero(x,eps) ? Zero : One/x); }
/// Kronecker delta
inline float Delta(float i, float j) { return ((i==j) ? One : Zero); }


////////////////////////////////////////////////////////////////////////////////
// Ints
////////////////////////////////////////////////////////////////////////////////
const static int IntMin = std::numeric_limits<int>::min();
const static int IntMax = std::numeric_limits<int>::max();

inline int Clamp(int x, int a, int b) { return ((x < a) ? (a) : ((x > b) ? b : x)); }

// Ceiling of Log base 2 (number of bits needed to represent an integer x)
inline int Clog2(int x) { return (int) Ceil(log2f((float) x)); }

inline int Min(int a, int b) { return ((a < b) ? a : b); }
inline int Max(int a, int b) { return ((a > b) ? a : b); }
inline unsigned int Min(unsigned int a, unsigned int b) { return ((a < b) ? a : b); }
inline unsigned int Max(unsigned int a, unsigned int b) { return ((a > b) ? a : b); }


} //namespace Dash

#endif // MATH_H_
