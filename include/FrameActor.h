#ifndef __FRAMEACTOR_H__
#define __FRAMEACTOR_H__

#include <dali/dali.h>
#include <dali-toolkit/dali-toolkit.h>
#include <dali/devel-api/common/stage-devel.h>
#include <string>
#include <list>
#include "Type.h"

/*
 * Basic object that managed by the system and can receive events.
 */
class FrameActor
{
    protected:
        Dali::Actor _actor;
        Dali::Stage _stage;
        Vec3 _position;
        Quat _rotation;
        Vec3 _size;
        Vec3 _spaceBasisX, _spaceBasisY, _spaceBasisZ;
        Vec3 _spaceOrigin;
        //std::list<FrameActor*> _childs;
        FrameActor *_parent;
        FrameActor *_plane;

    protected:
        FrameActor(Dali::Stage &stage, Dali::Actor &actor);

    public:
        FrameActor(Dali::Stage &stage);
        virtual ~FrameActor();
        Dali::Actor& GetActor() { return _actor; }
        std::string GetName() const { return _actor.GetName(); }
        Vec3 GetPosition() { return _position; }
        Quat GetRotation() { return _rotation; }
        Vec3 GetSize() { return _size; }
        void SetName (const std::string name) { _actor.SetName(name); }
        virtual void SetPosition(float x, float y, float z);
        virtual void SetPosition(Vec3 position);
        virtual void SetRotation(float x, float y, float z, float w);
        virtual void SetRotation(Quat rotation);
        virtual void SetSize(float x, float y, float z);
        virtual void SetSize(Vec3 size);
        virtual void RotateBy(Quat rot);

        // Hierarchy (It's not completed. Don't use this.)
        void AddChild(FrameActor *child);
        void RemoveChild(FrameActor *child);
        void Unparent();

        virtual void OnSpaceUpdated(FrameActor *plane, Vec3 basisX, Vec3 basisY, Vec3 basisZ, Vec3 origin);

        /*
        * Called at first frame right after the Dali::Actor is initialized.
        * Precisely, it is second frame since the system begun.
        */
        virtual void OnStart();
        /*
        * Being called every timer event
        */
        virtual void OnUpdate(double deltaTime);
};

#endif