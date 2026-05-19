#pragma once


// Built-in includes
#include <cmath>
#include <string>

// Third-party includes

// Local includes
#include "Vector.h"
#include "Math.h"
#include "Quaternion.h"

using Vector::Vector3;


namespace Matrix {
    class Matrix4 {
    public:
        float data[4][4];

        Matrix4() {
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    data[i][j] = 0.0f;
                }
            }
        }

        static Matrix4 IdentityMatrix() {
            Matrix4 result;
            result.data[0][0] = result.data[1][1] = result.data[2][2] = result.data[3][3] = 1.0f;
            return result;
        }

        const float* GetAsFloatPtr() const {
            return reinterpret_cast<const float*>(&data[0][0]);
        }

        const std::string GetAsString() const {
            std::string result;
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    result += std::to_string(data[i][j]) + " ";
                }
                result += "\n";
            }
            return result;
        }


        static Matrix4 Translation(const Vector3& translation) {
            Matrix4 result = IdentityMatrix();
            result.data[0][3] = translation.x;
            result.data[1][3] = translation.y;
            result.data[2][3] = translation.z;
            return result;
        }

        static Matrix4 Scaling(float scaleX, float scaleY, float scaleZ) {
            Matrix4 result = IdentityMatrix();
            result.data[0][0] = scaleX;
            result.data[1][1] = scaleY;
            result.data[2][2] = scaleZ;
            return result;
        }

        static Matrix4 Scaling(const Vector3& scale) {
            return Scaling(scale.x, scale.y, scale.z);
        }

        static Matrix4 Scaling(float uniformScale) {
            return Scaling(uniformScale, uniformScale, uniformScale);
        }



        static Matrix4 Rotation(float angle, const Vector3& axis) {
            Matrix4 result = IdentityMatrix();
            float c = cos(angle);
            float s = sin(angle);
            float t = 1 - c;

            result.data[0][0] = c + axis.x * axis.x * t;
            result.data[0][1] = axis.x * axis.y * t - axis.z * s;
            result.data[0][2] = axis.x * axis.z * t + axis.y * s;

            result.data[1][0] = axis.y * axis.x * t + axis.z * s;
            result.data[1][1] = c + axis.y * axis.y * t;
            result.data[1][2] = axis.y * axis.z * t - axis.x * s;

            result.data[2][0] = axis.z * axis.x * t - axis.y * s;
            result.data[2][1] = axis.z * axis.y * t + axis.x * s;
            result.data[2][2] = c + axis.z * axis.z * t;

            return result;
        }

        static Matrix4 RotationX(float angle) {
            Matrix4 result = IdentityMatrix();
            float c = cos(angle);
            float s = sin(angle);
            result.data[1][1] = c;
            result.data[1][2] = -s;
            result.data[2][1] = s;
            result.data[2][2] = c;
            return result;
        }

        static Matrix4 RotationY(float angle) {
            Matrix4 result = IdentityMatrix();
            float c = cos(angle);
            float s = sin(angle);
            result.data[0][0] = c;
            result.data[0][2] = s;
            result.data[2][0] = -s;
            result.data[2][2] = c;
            return result;
        }

        static Matrix4 RotationZ(float angle) {
            Matrix4 result = IdentityMatrix();
            float c = cos(angle);
            float s = sin(angle);
            result.data[0][0] = c;
            result.data[0][1] = -s;
            result.data[1][0] = s;
            result.data[1][1] = c;
            return result;
        }

        static Matrix4 CreateFromQuaternion(const Quaternion& q) {
            Matrix4 result = IdentityMatrix();
            float xx = q.x * q.x;
            float yy = q.y * q.y;
            float zz = q.z * q.z;
            float xy = q.x * q.y;
            float xz = q.x * q.z;
            float yz = q.y * q.z;
            float wx = q.w * q.x;
            float wy = q.w * q.y;
            float wz = q.w * q.z;

            result.data[0][0] = 1 - 2 * (yy + zz);
            result.data[0][1] = 2 * (xy - wz);
            result.data[0][2] = 2 * (xz + wy);

            result.data[1][0] = 2 * (xy + wz);
            result.data[1][1] = 1 - 2 * (xx + zz);
            result.data[1][2] = 2 * (yz - wx);

            result.data[2][0] = 2 * (xz - wy);
            result.data[2][1] = 2 * (yz + wx);
            result.data[2][2] = 1 - 2 * (xx + yy);

            return result;
        }

        static Matrix4 CreateOrthographic(float width, float height, float near, float far) {
            Matrix4 result = IdentityMatrix();
            result.data[0][0] = 2.0f / (width);
            result.data[1][1] = 2.0f / (height);
            result.data[2][2] = -2.0f / (far - near);
            result.data[3][2] = near / (near - far);
            return result;
        }

        static Matrix4 CreateOrthographic(float left, float right, float bottom, float top, float near, float far) {
            Matrix4 result = IdentityMatrix();
            result.data[0][0] = 2.0f / (right - left);
            result.data[1][1] = 2.0f / (top - bottom);
            result.data[2][2] = -2.0f / (far - near);
            result.data[3][0] = -(right + left) / (right - left);
            result.data[3][1] = -(top + bottom) / (top - bottom);
            result.data[3][2] = near / (near - far);
            return result;
        }

        static Matrix4 CreatePerspectiveFOV(float fov, float width, float height, float near, float far) {
            Matrix4 result = IdentityMatrix();
            float yScale = Math::cot(fov / 2.0f);
            float xScale = yScale * height / width;
            result.data[0][0] = xScale;
            result.data[1][1] = yScale;
            // OpenGL RH perspective (NDC z in [-1, 1]).
            result.data[2][2] = -(far + near) / (far - near);
            result.data[2][3] = -(2.0f * far * near) / (far - near);
            result.data[3][2] = -1.0f;
            result.data[3][3] = 0.0f;

            return result;
        }

        static Matrix4 CreateSimpleView(float width, float height) {
            Matrix4 result = IdentityMatrix();
            result.data[0][0] = 2.0f / width;
            result.data[1][1] = 2.0f / height;
            result.data[3][2] = 1.0f;
            return result;
        }

        static Matrix4 CreateLookAt(const Vector3& eye, const Vector3& target, const Vector3& up) {
            Matrix4 result = IdentityMatrix();
            Vector3 zaxis = (eye - target).Normalize();
            Vector3 xaxis = Vector3::Cross(up, zaxis).Normalize();
            Vector3 yaxis = Vector3::Cross(zaxis, xaxis);

            result.data[0][0] = xaxis.x;
            result.data[0][1] = xaxis.y;
            result.data[0][2] = xaxis.z;
            result.data[0][3] = -Vector3::Dot(xaxis, eye);

            result.data[1][0] = yaxis.x;
            result.data[1][1] = yaxis.y;
            result.data[1][2] = yaxis.z;
            result.data[1][3] = -Vector3::Dot(yaxis, eye);

            result.data[2][0] = zaxis.x;
            result.data[2][1] = zaxis.y;
            result.data[2][2] = zaxis.z;
            result.data[2][3] = -Vector3::Dot(zaxis, eye);

            return result;
        }

        // Matrix operations
        Matrix4 operator+(const Matrix4& other) const {
            Matrix4 result;
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    result.data[i][j] = data[i][j] + other.data[i][j];
                }
            }
            return result;
        }

        Matrix4 operator-(const Matrix4& other) const {
            Matrix4 result;
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    result.data[i][j] = data[i][j] - other.data[i][j];
                }
            }
            return result;
        }

        Matrix4 operator*(const Matrix4& other) const {
            Matrix4 result;
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    result.data[i][j] = data[i][0] * other.data[0][j] +
                                     data[i][1] * other.data[1][j] +
                                     data[i][2] * other.data[2][j] +
                                     data[i][3] * other.data[3][j];
                }
            }
            return result;
        }

        Matrix4 operator*(float scalar) const {
            Matrix4 result;
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    result.data[i][j] = data[i][j] * scalar;
                }
            }
            return result;
        }

        Matrix4 operator*(const Vector3& vec) const {
            Matrix4 result;
            result.data[0][0] = data[0][0] * vec.x + data[0][1] * vec.y + data[0][2] * vec.z + data[0][3];
            result.data[1][0] = data[1][0] * vec.x + data[1][1] * vec.y + data[1][2] * vec.z + data[1][3];
            result.data[2][0] = data[2][0] * vec.x + data[2][1] * vec.y + data[2][2] * vec.z + data[2][3];
            result.data[3][0] = data[3][0] * vec.x + data[3][1] * vec.y + data[3][2] * vec.z + data[3][3];
            return result;
        }

        Matrix4 operator*=(const Matrix4& other) {
            *this = *this * other;
            return *this;
        }




       static const Matrix4 Identity;
    };
}