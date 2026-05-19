#pragma once

// Built-in includes
#include <cmath>

// Third-party includes

// Local includes






namespace Vector {
    struct Vector2 {
        float x;
        float y;

        Vector2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
    };

    class Vector3 {
    public:
        float x;
        float y;
        float z;

        Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}

        static const Vector3 Up, Down, Left, Right, Forward, Backward, Zero, One;

        // static float[] ToArray(const Vector3& v) {
        //     return { v.x, v.y, v.z };
        // };

        static Vector3 Cross(const Vector3& a, const Vector3& b) {
            return Vector3(
                a.y * b.z - a.z * b.y,
                a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x
            );
        }

        static float Dot(const Vector3& a, const Vector3& b) {
            return a.x * b.x + a.y * b.y + a.z * b.z;
        }

        static float Length(const Vector3& v) {
            return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
        }

        static Vector3 Normalize(const Vector3& v) {
            float length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
            if (length > 0) {
                return Vector3(v.x / length, v.y / length, v.z / length);
            }
            return Vector3(0, 0, 0); // Return zero vector if length is zero
        }

        Vector3 Normalize() const {
            return Normalize(*this);
        }

        static Vector3 Scale(const Vector3& v, float scalar) {
            return Vector3(v.x * scalar, v.y * scalar, v.z * scalar);
        }

        // Addition Operators
        Vector3 operator+(const Vector3& other) const {
            return Vector3(x + other.x, y + other.y, z + other.z);
        }

        Vector3 operator+=(const Vector3& other) {
            *this = *this + other;
            return *this;
        }

        // Subtraction Operators
        Vector3 operator-(const Vector3& other) const {
            return Vector3(x - other.x, y - other.y, z - other.z);
        }
        Vector3 operator-=(const Vector3& other) {
            *this = *this - other;
            return *this;
        }


        // Multiplication Operators
        Vector3 operator*(float scalar) const {
            return Vector3(x * scalar, y * scalar, z * scalar);
        }

        Vector3 operator*(const Vector3& other) const {
            return Vector3(x * other.x, y * other.y, z * other.z);
        }

        Vector3 operator*=(float scalar) {
            *this = *this * scalar;
            return *this;
        }

        Vector3 operator*=(const Vector3& other) {
            *this = *this * other;
            return *this;
        }

        // Division Operators
        Vector3 operator/(float scalar) const {
            return Vector3(x / scalar, y / scalar, z / scalar);
        }

    };

    class Vector3i {
    public:
        int x;
        int y;
        int z;

        Vector3i(int x = 0, int y = 0, int z = 0) : x(x), y(y), z(z) {}
    };

    struct Vector4 {
        float x;
        float y;
        float z;
        float w;

        Vector4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f) : x(x), y(y), z(z), w(w) {}
    };
}