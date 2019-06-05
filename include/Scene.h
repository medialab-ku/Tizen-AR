#ifndef __SCENE_H__
#define __SCENE_H__

#include <dali-toolkit/dali-toolkit.h>
#include <dali/devel-api/common/stage-devel.h>
#include <list>
#include <iostream>
#include "FrameActor.h"
#include "GraphicsActor.h"
#include "CameraFrameActor.h"
#include "PrimitiveModels.h"

class PlaneActor : public GraphicsActor
{
    public:
        PlaneActor(Dali::Stage &stage, Model &model);
        void OnSpaceUpdated(FrameActor *plane, Vec3 basisX, Vec3 basisY, Vec3 basisZ, Vec3 origin) override;
};

class Scene
{
    protected:
        // Essentials
        // FrameActor is pointer since it can be created and removed at any time
        std::list<FrameActor*> _actorList;
        Dali::Stage _stage;
        FrameActor *_plane;
        CameraFrameActor *_camera;
        Vec3 _basisX, _basisY, _basisZ;
        Vec3 _origin;

    public:
        Scene(Dali::Stage &stage, Dali::CameraActor &camera);
        void OnStart();
        void OnUpdate(double deltaTime, Vec3 planeNormal, Vec3 planeOrigin, Vec3 cameraPos, Quat cameraRot);
        void OnKeyEvent(const Dali::KeyEvent &event);
        void OnTouch(Dali::Actor actor, const Dali::TouchData &touch);

        Vec3 GetBasisX() { return _basisX; }
        Vec3 GetBasisY() { return _basisY; }
        Vec3 GetBasisZ() { return _basisZ; }
        Vec3 GetOrigin() { return _origin; }

    protected:
        virtual void Start();
        virtual void Update(double deltaTime);
        virtual void KeyEvent(const Dali::KeyEvent &event);
        virtual void Touch(Dali::Actor actor, const Dali::TouchData &touch);

    protected:
        void AddActor(FrameActor *actor);
        void RemoveActor(FrameActor *actor);

    private:
        void _UpdatePlane(Vec3 planeNormal, Vec3 planeOrigin);
        void _UpdateCamera(Vec3 cameraPos, Quat cameraRot);
};

#endif