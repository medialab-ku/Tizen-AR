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
        void OnUpdate(double deltaTime, Vec3 planeNormal, Vec3 planeOrigin);
        void OnKeyEvent(const Dali::KeyEvent &event);
        void OnTouch(Dali::Actor actor, const Dali::TouchData &touch);

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
};

#endif