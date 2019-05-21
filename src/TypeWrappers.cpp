#include "TypeWrappers.h"

wVector3::wVector3()
    : x(0.0f), y(0.0f), z(0.0f)
{
}

wVector3::wVector3(const float x, const float y, const float z)
    : x(x), y(y), z(z)
{
}

wVector3::wVector3(const Eigen::Vector3f cv)
    : x(cv(0)), y(-cv(1)), z(-cv(2))
{    
}

wVector3::wVector3(const Dali::Vector3 dali)
    : x(-dali.x), y(-dali.y), z(-dali.z)
{
}

wVector3::wVector3(const btVector3 bullet)
    : x(bullet.getX()), y(bullet.getY()), z(bullet.getZ())
{
}

Dali::Vector3
wVector3::ToCV() const
{
    return Dali::Vector3(x, -y, -z);
}

Dali::Vector3
wVector3::ToDali() const
{
    return Dali::Vector3(-x, -y, -z);
}

btVector3
wVector3::ToBullet() const
{
    return btVector3(x, y, z);
}

wVector3
wVector3::operator+ (const wVector3& v) const
{
    return wVector3(x + v.x , y + v.y, z + v.z);
}

wVector3
wVector3::operator- (const wVector3& v) const
{
    return wVector3(x - v.x , y - v.y, z - v.z);
}

wVector3
wVector3::operator* (const float k) const
{
    return wVector3(x*k, y*k, z*k);
}

float
wVector3::Length() const
{
    return std::sqrt(x*x + y*y + z*z);
}

wVector3
wVector3::Normalize()
{
    float l = std::sqrt(x*x + y*y + z*z);
    return wVector3(x/l, y/l,  z/l);
}


wQuaternion::wQuaternion()
    : x(0.0f), y(0.0f), z(0.0f), w(0.0f)
{
}

wQuaternion::wQuaternion(const float x, const float y, const float z, const float w)
    : x(x), y(y), z(z), w(w)
{
}

wQuaternion::wQuaternion(const Eigen::Vector4f cv)
    : x(cv(0)), y(-cv(1)), z(-cv(2)), w(cv(3))
{
}

wQuaternion::wQuaternion(const Dali::Quaternion dali)
    // may have bug
    : x(-dali.AsVector().x), y(-dali.AsVector().y), z(-dali.AsVector().z), w(-dali.AsVector().w)
{
}

wQuaternion::wQuaternion(const btQuaternion bullet)
    : x(bullet.getX()), y(bullet.getY()), z(bullet.getZ()), w(bullet.getW())
{
}

Dali::Quaternion
wQuaternion::ToDali() const
{
    // may have bug
    return Dali::Quaternion(-w, -x, -y, -z);
}

btQuaternion
wQuaternion::ToBullet() const
{
    return btQuaternion(x, y, z, w);
}

wQuaternion
wQuaternion::Inverse()
{
    return wQuaternion(-x, -y, -z, -w);
}