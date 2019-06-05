#include "FrameActor.h"
#include <iostream>

FrameActor::FrameActor(Dali::Stage &stage)
    : _stage(stage),
      _spaceBasisX(Vec3::right),
      _spaceBasisY(Vec3::up),
      _spaceBasisZ(Vec3::forward),
      _spaceOrigin(Vec3::zero),
      _childs(),
      _parent(nullptr)
{
    _actor = Dali::Actor::New();
    _actor.SetAnchorPoint(Dali::AnchorPoint::CENTER);
    _actor.SetParentOrigin(Dali::ParentOrigin::CENTER);
    // Initial size of Dali Actor is... (0, 0, 0)... really!
    _actor.SetSize(Dali::Vector3(1, 1, 1));
    _stage.Add(_actor);
    FrameActor::SetPosition(0, 0, 0);
    FrameActor::SetRotation(0, 0, 0, 1);
}

FrameActor::FrameActor(Dali::Stage &stage, Dali::Actor &actor)
    : _stage(stage),
      _actor(actor)
{
    
}

FrameActor::~FrameActor()
{
    // Dali::Actor's destructor will be called
    _stage.Remove(_actor);
}

void
FrameActor::SetPosition(float x, float y, float z)
{
    _position = Vec3(x, y, z);
    auto real = GetRealPosition();
    _actor.SetPosition(real.ToDali());
}

void
FrameActor::SetPosition(Vec3 position)
{
    SetPosition(position.x, position.y, position.z);
}

void
FrameActor::SetRotation(float x, float y, float z, float w)
{
    _rotation = Quat(x, y, z, w);
    auto real = GetRealRotation();
    _actor.SetOrientation(real.ToDali());
}

void
FrameActor::SetRotation(Quat rotation)
{
    SetRotation(rotation.x, rotation.y, rotation.z, rotation.w);
}

void
FrameActor::SetSize(float x, float y, float z)
{
    _size = Vec3(x, y, z);
    _actor.SetSize(_size.ToDali());
}

void
FrameActor::SetSize(Vec3 size)
{
    _size = size;
    _actor.SetSize(_size.ToDali());
}

void
FrameActor::RotateBy(Quat rot)
{
    auto curRot = GetRealRotation().ToDali();
    auto paramRot = rot.ToDali();
    auto newRot = paramRot * curRot;
    SetRotation(Quat(newRot));
}

Vec3
FrameActor::GetRealPosition()
{
    return (_spaceBasisX * _position.x) 
            + (_spaceBasisY * _position.y) 
            + (_spaceBasisZ * _position.z) 
            + _spaceOrigin;
}

Quat
FrameActor::GetRealRotation()
{
    // Assuming that the basis is Orthonormal
    auto basisRot = Dali::Quaternion(_spaceBasisX.ToDali(), _spaceBasisY.ToDali(), _spaceBasisZ.ToDali());
    auto real = _rotation.ToDali() * basisRot;
    //auto real = basisRot * _rotation.ToDali();
    //auto real = basisRot;
    return Quat(real);
}

void
FrameActor::OnSpaceUpdated(Vec3 basisX, Vec3 basisY, Vec3 basisZ, Vec3 origin)
{
    _spaceBasisX = basisX;
    _spaceBasisY = basisY;
    _spaceBasisZ = basisZ;
    _spaceOrigin = origin;
    SetPosition(GetPosition());
    SetRotation(GetRotation());
}

void
FrameActor::AddChild(FrameActor *child)
{
    bool found = (std::find(_childs.begin(), _childs.end(), child) != _childs.end());
    if (!found)
    {
        _childs.push_back(child);
        _actor.Add(child->GetActor());
        child->_parent = this;
    }
}

void
FrameActor::RemoveChild(FrameActor *child)
{
    bool found = (std::find(_childs.begin(), _childs.end(), child) != _childs.end());
    if (found)
    {
        _childs.remove(child);
        _actor.Remove(child->GetActor());
        _stage.Add(child->GetActor());
        child->_parent = nullptr;
    }
}

void
FrameActor::Unparent()
{
    if (_parent)
    {
        _parent->RemoveChild(this);
    }
}

void
FrameActor::OnStart()
{
}

void
FrameActor::OnUpdate(double deltaTime)
{
}