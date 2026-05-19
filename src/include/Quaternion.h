#pragma once

// Built-in includes
#include <cmath>

// Third-party includes

// Local includes
#include "Math.h"
#include "Vector.h"

using Vector::Vector3;


class Quaternion {
public:
    float x, y, z, w;

    Quaternion() : x(0), y(0), z(0), w(1) {}
    Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    Quaternion(const Quaternion& other) : x(other.x), y(other.y), z(other.z), w(other.w) {}

    static const Quaternion Identity() {
        return Quaternion(0, 0, 0, 1);
    }

    explicit Quaternion(const Vector3& axis, float angle) {
        float halfAngle = angle * 0.5f;
        float s = sin(halfAngle);
        x = axis.x * s;
        y = axis.y * s;
        z = axis.z * s;
        w = cos(halfAngle);
    }

    void Set(float x, float y, float z, float w) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    static Quaternion FromAxisAngle(float angle, float axisX, float axisY, float axisZ) {
        float halfAngle = angle * 0.5f;
        float s = sin(halfAngle);
        return Quaternion(axisX * s, axisY * s, axisZ * s, cos(halfAngle));
    }

    void Normalize() {
        float length = Length();
        if (length > 0) {
            float invLength = 1.0f / length;
            x *= invLength;
            y *= invLength;
            z *= invLength;
            w *= invLength;
        }
    }

    void Conjugate() {
        x = -x;
        y = -y;
        z = -z;
    }

    float Length() const {
        return sqrt(x*x + y*y + z*z + w*w);
    }

    static float Dot(const Quaternion& a, const Quaternion& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }

    static Quaternion Lerp(const Quaternion& a, const Quaternion& b, float t) {
        return Quaternion(
            Math::Lerp(a.x, b.x, t),
            Math::Lerp(a.y, b.y, t),
            Math::Lerp(a.z, b.z, t),
            Math::Lerp(a.w, b.w, t)
        );
    }

    static Quaternion Slerp(const Quaternion& a, const Quaternion& b, float t) {
        float dot = Dot(a, b);
        const float DOT_THRESHOLD = 0.9995f;
        if (dot > DOT_THRESHOLD) {
            return Lerp(a, b, t);
        }

        dot = fmax(fmin(dot, 1.0f), -1.0f);
        float theta_0 = acos(dot);
        float theta = theta_0 * t;
        float sin_theta = sin(theta);
        float sin_theta_0 = sin(theta_0);

        float s0 = cos(theta) - dot * sin_theta / sin_theta_0;
        float s1 = sin_theta / sin_theta_0;

        return Quaternion(
            (a.x * s0) + (b.x * s1),
            (a.y * s0) + (b.y * s1),
            (a.z * s0) + (b.z * s1),
            (a.w * s0) + (b.w * s1)
        );
    }

    static Quaternion FromEulerAngles(float pitch, float yaw, float roll) {
        float cy = cos(yaw * 0.5f);
        float sy = sin(yaw * 0.5f);
        float cp = cos(pitch * 0.5f);
        float sp = sin(pitch * 0.5f);
        float cr = cos(roll * 0.5f);
        float sr = sin(roll * 0.5f);

        return Quaternion(
            sr * cp * cy - cr * sp * sy,
            cr * sp * cy + sr * cp * sy,
            cr * cp * sy - sr * sp * cy,
            cr * cp * cy + sr * sp * sy
        );
    }

    static void ToEulerAngles(const Quaternion& q, float& pitch, float& yaw, float& roll) {
        // Convert quaternion to Euler angles (in radians)
        float sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
        float cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
        roll = atan2(sinr_cosp, cosr_cosp);

        float sinp = 2 * (q.w * q.y - q.z * q.x);
        if (fabs(sinp) >= 1)
            pitch = copysign(Math::HALF_PI, sinp); // use 90 degrees if out of range
        else
            pitch = asin(sinp);

        float siny_cosp = 2 * (q.w * q.z + q.x * q.y);
        float cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
        yaw = atan2(siny_cosp, cosy_cosp);
    }

    static Quaternion Concatenate(const Quaternion& q, const Quaternion& p)
	{
		Quaternion retVal;

		// Vector component is:
		// ps * qv + qs * pv + pv x qv
		Vector3 qv(q.x, q.y, q.z);
		Vector3 pv(p.x, p.y, p.z);
		Vector3 newVec = qv * p.w + pv * q.w + Vector3::Cross(pv, qv);
		retVal.x = newVec.x;
		retVal.y = newVec.y;
		retVal.z = newVec.z;

		// Scalar component is:
		// ps * qs - pv . qv
		retVal.w = p.w * q.w - Vector3::Dot(pv, qv);

		return retVal;
	}

    Quaternion operator*(const Quaternion& other) const {
        return Quaternion(
            w * other.x + x * other.w + y * other.z - z * other.y,
            w * other.y - x * other.z + y * other.w + z * other.x,
            w * other.z + x * other.y - y * other.x + z * other.w,
            w * other.w - x * other.x - y * other.y - z * other.z
        );
    }

    Quaternion operator*(float scalar) const {
        return Quaternion(x * scalar, y * scalar, z * scalar, w * scalar);
    }
};