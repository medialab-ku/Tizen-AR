#include "Type.h"

Vec3 Vec3::zero(0, 0, 0);
Vec3 Vec3::one(1, 1, 1);
Vec3 Vec3::right(1, 0, 0);
Vec3 Vec3::left(-1, 0, 0);
Vec3 Vec3::up(0, 1, 0);
Vec3 Vec3::down(0, -1, 0);
Vec3 Vec3::forward(0, 0, 1);
Vec3 Vec3::back(0, 0, -1);

Vec3::Vec3()
    : x(0.0f), y(0.0f), z(0.0f)
{
}

Vec3::Vec3(const float x, const float y, const float z)
    : x(x), y(y), z(z)
{
}

Vec3::Vec3(const Eigen::Vector3f cv)
    : x(cv(0)), y(-cv(1)), z(-cv(2))
{    
}

Vec3::Vec3(const Dali::Vector3 dali)
    : x(-dali.x), y(-dali.y), z(-dali.z)
{
}

Vec3::Vec3(const btVector3 bullet)
    : x(bullet.getX()), y(bullet.getY()), z(bullet.getZ())
{
}

Dali::Vector3
Vec3::ToCV() const
{
    return Dali::Vector3(x, -y, -z);
}

Dali::Vector3
Vec3::ToDali() const
{
    return Dali::Vector3(-x, -y, -z);
}

btVector3
Vec3::ToBullet() const
{
    return btVector3(x, y, z);
}

Vec3
Vec3::operator+ (const Vec3& v) const
{
    return Vec3(x + v.x , y + v.y, z + v.z);
}

Vec3
Vec3::operator- (const Vec3& v) const
{
    return Vec3(x - v.x , y - v.y, z - v.z);
}

Vec3
Vec3::operator* (const float k) const
{
    return Vec3(x*k, y*k, z*k);
}

Vec3
Vec3::operator- () const
{
    return Vec3(-x, -y, -z);
}

float
Vec3::Length() const
{
    return std::sqrt(x*x + y*y + z*z);
}

Vec3
Vec3::Normalize()
{
    float l = std::sqrt(x*x + y*y + z*z);
    return Vec3(x/l, y/l,  z/l);
}


Quat::Quat()
    : x(0.0f), y(0.0f), z(0.0f), w(0.0f)
{
}

Quat::Quat(const float x, const float y, const float z, const float w)
    : x(x), y(y), z(z), w(w)
{
}

Quat::Quat(const Eigen::Vector4f cv)
    : x(cv(0)), y(-cv(1)), z(-cv(2)), w(cv(3))
{
}

Quat::Quat(const Dali::Quaternion dali)
    // may have bug
    : x(-dali.AsVector().x), y(-dali.AsVector().y), z(-dali.AsVector().z), w(-dali.AsVector().w)
{
}

Quat::Quat(const btQuaternion bullet)
    : x(bullet.getX()), y(bullet.getY()), z(bullet.getZ()), w(bullet.getW())
{
}

Dali::Quaternion
Quat::ToDali() const
{
    // may have bug
    return Dali::Quaternion(-w, -x, -y, -z);
}

btQuaternion
Quat::ToBullet() const
{
    return btQuaternion(x, y, z, w);
}

Quat
Quat::Inverse()
{
    return Quat(-x, -y, -z, -w);
}