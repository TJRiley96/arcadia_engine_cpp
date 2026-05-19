#pragma once

// Built-in includes
#include <cmath>

// Third-party includes

// Local includes
#include "Vector.h"
using Vector::Vector3;
using Vector::Vector2;

namespace Math {

    const float PI = 3.14159265f;
    const float TWO_PI = 6.28318530f;
    const float HALF_PI = 1.57079632f;
    const float DEG_TO_RAD = PI / 180.0f;
    const float RAD_TO_DEG = 180.0f / PI;



    template<typename T>
    T Max(const T& a, const T& b) {
        return (a > b) ? a : b;
    }

    template<typename T>
    T Min(const T& a, const T& b) {
        return (a < b) ? a : b;
    }

    inline float ToRadians(float degrees) {
        return degrees * DEG_TO_RAD;
    }

    inline float ToDegrees(float radians) {
        return radians * RAD_TO_DEG;
    }

    inline float Clamp(float value, float min, float max) {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }

    inline float Lerp(float a, float b, float t) {
        return a + t * (b - a);
    }

    inline float SmoothStep(float edge0, float edge1, float x) {
        // Scale, bias and saturate x to 0..1 range
        x = Clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
        // Evaluate polynomial
        return x * x * (3 - 2 * x);
    }

    inline float SmootherStep(float edge0, float edge1, float x) {
        // Scale, bias and saturate x to 0..1 range
        x = Clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
        // Evaluate polynomial
        return x * x * x * (x * (x * 6 - 15) + 10);
    }

    inline float cosineInterpolate(float a, float b, float t) {
        float t2 = (1 - cos(t * PI)) / 2;
        return a * (1 - t2) + b * t2;
    }

    inline float cot(float angle) {
        return 1.0f / tan(angle);
    }

    inline bool NearlyEqual(float a, float b, float epsilon = 0.00001f) {
        return fabs(a - b) < epsilon;
    }
    inline bool NearlyZero(float value, float epsilon = 0.00001f) {
        return fabs(value) < epsilon;
    }

    inline bool NearlyEqual(const Vector3& a, const Vector3& b, float epsilon = 0.00001f) {
        return NearlyEqual(a.x, b.x, epsilon) && NearlyEqual(a.y, b.y, epsilon) && NearlyEqual(a.z, b.z, epsilon);
    }

    inline bool NearlyEqual(const Vector2& a, const Vector2& b, float epsilon = 0.00001f) {
        return NearlyEqual(a.x, b.x, epsilon) && NearlyEqual(a.y, b.y, epsilon);
    }

    inline bool NearlyZero(const Vector3& v, float epsilon = 0.00001f) {
        return NearlyZero(v.x, epsilon) && NearlyZero(v.y, epsilon) && NearlyZero(v.z, epsilon);
    }

    inline bool NearlyZero(const Vector2& v, float epsilon = 0.00001f) {
        return NearlyZero(v.x, epsilon) && NearlyZero(v.y, epsilon);
    }

    inline float Sqrt(float value) {
        return sqrt(value);
    }
}