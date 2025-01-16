#pragma once

#include <math.h>

#define PI   3.14159265359f
#define PI_2 1.57079632679f
#define TAU  6.28318530718f

#define CLAMP(x, min, max) ((x) < (min) ? (min) : (x) > (max) ? (max) : (x))
#define MIN(a, b)          ((a) < (b) ? (a) : (b))
#define MAX(a, b)          ((a) > (b) ? (a) : (b))
#define ABS(x)             ((x) < 0 ? -(x) : (x))
#define SIGN(x)            ((x) < 0 ? -1 : 1)
#define RAD(deg)           ((deg) * PI / 180.0f)
#define DEG(rad)           ((rad) * 180.0f / PI)
#define LERP(a, b, t)      ((a) + (t) * ((b) - (a)))
#define NLERP(a, b, t)     LERP(a, b, t)
#define SLERP(a, b, t) ((a) + (t) * ((b) - (a))
