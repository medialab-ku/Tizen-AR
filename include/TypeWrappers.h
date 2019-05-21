#ifndef __Type_WRAPPERS_H__
#define __Type_WRAPPERS_H__

#include <dali-toolkit/dali-toolkit.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <Eigen/Core>

/*
 * Integrates each data structures into one class.
 * Integrates coordinates system into bullet's one.
 * Provides conversion methods.
 */

class wVector3
{
public:
    wVector3();
    wVector3(const float x, const float y, const float z);
    wVector3(const Eigen::Vector3f cv);
    wVector3(const Dali::Vector3 dali);
    wVector3(const btVector3 bullet);
    Dali::Vector3 ToCV() const;
    Dali::Vector3 ToDali() const;
    btVector3 ToBullet() const;

    wVector3 operator+(const wVector3&) const;
    wVector3 operator-(const wVector3&) const;
    wVector3 operator*(const float) const;
    float Length() const;
    wVector3 Normalize();

public:
    float x, y, z;
};

class wQuaternion
{
public:
    wQuaternion();
    wQuaternion(const float x, const float y, const float z, const float w);
    wQuaternion(const Eigen::Vector4f cv); // x, y, z, w
    wQuaternion(const Dali::Quaternion dali);
    wQuaternion(const btQuaternion bullet);
    Dali::Quaternion ToCV() const;
    Dali::Quaternion ToDali() const;
    btQuaternion ToBullet() const;

    // Operations not implemented yet
    wQuaternion Inverse();

public:
    float x, y, z, w;
};

#endif