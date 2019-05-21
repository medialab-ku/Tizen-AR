#include "FrameActor.h"
#include <iostream>

FrameActor::FrameActor(Dali::Stage &stage)
    : mStage(stage),
      mChilds(),
      mParent(nullptr)
{
    mActor = Dali::Actor::New();
    mActor.SetAnchorPoint(Dali::AnchorPoint::CENTER);
    mActor.SetParentOrigin(Dali::ParentOrigin::CENTER);
    // Initial size of Dali Actor is... (0, 0, 0)... really!
    mActor.SetSize(Dali::Vector3(1, 1, 1));
    mStage.Add(mActor);
    FrameActor::SetPosition(0, 0, 0);
    FrameActor::SetRotation(0, 0, 0, 1);
}

FrameActor::FrameActor(Dali::Stage &stage, Dali::Actor &actor)
    : mStage(stage),
      mActor(actor)
{
    
}

FrameActor::~FrameActor()
{
    // Dali::Actor's destructor will be called
    mStage.Remove(mActor);
}

void
FrameActor::SetPosition(float x, float y, float z)
{
    mPosition = wVector3(x, y, z);
    mActor.SetPosition(mPosition.ToDali());
}

void
FrameActor::SetPosition(wVector3 position)
{
    mPosition = position;
    mActor.SetPosition(mPosition.ToDali());
}

void
FrameActor::SetRotation(float x, float y, float z, float w)
{
    mRotation = wQuaternion(x, y, z, w);
    mActor.SetOrientation(mRotation.ToDali());
}

void
FrameActor::SetRotation(wQuaternion rotation)
{
    mRotation = rotation;
    mActor.SetOrientation(mRotation.ToDali());
}

void
FrameActor::SetSize(float x, float y, float z)
{
    mSize = wVector3(x, y, z);
    mActor.SetSize(mSize.ToDali());
}

void
FrameActor::SetSize(wVector3 size)
{
    mSize = size;
    mActor.SetSize(mSize.ToDali());
}

void
FrameActor::RotateBy(wQuaternion rot)
{
    Dali::Quaternion newRot = mRotation.ToDali();
    newRot *= rot.ToDali();
    SetRotation(wQuaternion(newRot));
}

void
FrameActor::AddChild(FrameActor *child)
{
    bool found = (std::find(mChilds.begin(), mChilds.end(), child) != mChilds.end());
    if (!found)
    {
        mChilds.push_back(child);
        mActor.Add(child->GetActor());
        child->mParent = this;
    }
}

void
FrameActor::RemoveChild(FrameActor *child)
{
    bool found = (std::find(mChilds.begin(), mChilds.end(), child) != mChilds.end());
    if (found)
    {
        mChilds.remove(child);
        mActor.Remove(child->GetActor());
        child->mParent = nullptr;
    }
}

void
FrameActor::Unparent()
{
    if (mParent)
    {
        mParent->RemoveChild(this);
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