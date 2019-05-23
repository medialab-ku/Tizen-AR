#include "FrameActor.h"
#include <iostream>

FrameActor::FrameActor(Dali::Stage &stage)
    : _stage(stage),
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
    _actor.SetPosition(_position.ToDali());
}

void
FrameActor::SetPosition(Vec3 position)
{
    _position = position;
    _actor.SetPosition(_position.ToDali());
}

void
FrameActor::SetRotation(float x, float y, float z, float w)
{
    _rotation = Quat(x, y, z, w);
    _actor.SetOrientation(_rotation.ToDali());
}

void
FrameActor::SetRotation(Quat rotation)
{
    _rotation = rotation;
    _actor.SetOrientation(_rotation.ToDali());
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
    Dali::Quaternion newRot = _rotation.ToDali();
    newRot *= rot.ToDali();
    SetRotation(Quat(newRot));
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