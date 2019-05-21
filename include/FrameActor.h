#ifndef __FRAMEACTOR_H__
#define __FRAMEACTOR_H__

#include <dali/dali.h>
#include <dali-toolkit/dali-toolkit.h>
#include <dali/devel-api/common/stage-devel.h>
#include <string>
#include <list>
#include "TypeWrappers.h"

/*
 * Basic object that managed by the system and can receive events.
 */
class FrameActor
{
public:
    FrameActor(Dali::Stage &stage);
    virtual ~FrameActor();
    Dali::Actor& GetActor() { return mActor; }
    std::string GetName() const { return mActor.GetName(); }
    wVector3 GetPosition() { return mPosition; }
    wQuaternion GetRotation() { return mRotation; }
    wVector3 GetSize() { return mSize; }
    void SetName (const std::string name) { mActor.SetName(name); }
    virtual void SetPosition(float x, float y, float z);
    virtual void SetPosition(wVector3 position);
    virtual void SetRotation(float x, float y, float z, float w);
    virtual void SetRotation(wQuaternion rotation);
    virtual void SetSize(float x, float y, float z);
    virtual void SetSize(wVector3 size);
    virtual void RotateBy(wQuaternion rot);

    // Hierarchy (It's not completed. Don't use this.)
    void AddChild(FrameActor *child);
    void RemoveChild(FrameActor *child);
    void Unparent();

    /*
     * Called at first frame right after the Dali::Actor is initialized.
     * Precisely, it is second frame since the system begun.
     */
    virtual void OnStart();
    /*
     * Being called every timer event
     */
    virtual void OnUpdate(double deltaTime);

protected:
    FrameActor(Dali::Stage &stage, Dali::Actor &actor);
    
protected:
    Dali::Actor mActor;
    Dali::Stage mStage;
    wVector3 mPosition;
    wQuaternion mRotation;
    wVector3 mSize;
    std::list<FrameActor*> mChilds;
    FrameActor *mParent;
};

#endif