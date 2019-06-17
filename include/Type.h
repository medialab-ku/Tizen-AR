#ifndef __TWRAPPERS_H__
#define __TWRAPPERS_H__

#include <dali-toolkit/dali-toolkit.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <Eigen/Core>

/**
 * Integrates each data structures into one class.
 * Integrates coordinates system into bullet's one.
 * Provides conversion methods.
 */

class Vec3
{
public:
    Vec3();
    Vec3(const float x, const float y, const float z);
    Vec3(const Eigen::Vector3f cv);
    Vec3(const Dali::Vector3 dali);
    Vec3(const btVector3 bullet);
    Dali::Vector3 ToCV() const;
    Dali::Vector3 ToDali() const;
    btVector3 ToBullet() const;

    Vec3 operator+(const Vec3&) const;
    Vec3 operator-(const Vec3&) const;
    Vec3 operator*(const float) const;
    Vec3 operator-() const;
    float Length() const;
    Vec3 Normalize();

    static Vec3 zero, one;
    static Vec3 right, left, up, down, forward, back;

public:
    float x, y, z;
};

class Quat
{
public:
    Quat();
    Quat(const float x, const float y, const float z, const float w);
    Quat(const Eigen::Vector4f cv); // x, y, z, w
    Quat(const Dali::Quaternion dali);
    Quat(const btQuaternion bullet);
    Dali::Quaternion ToCV() const;
    Dali::Quaternion ToDali() const;
    btQuaternion ToBullet() const;

    // Operations not implemented yet
    Quat Inverse();

public:
    float x, y, z, w;
};

#endif