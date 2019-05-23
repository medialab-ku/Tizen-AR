#ifndef __SCENE_H__
#define __SCENE_H__

#include <dali-toolkit/dali-toolkit.h>
#include <dali/devel-api/common/stage-devel.h>
#include <list>
#include <iostream>
#include "FrameActor.h"
#include "CameraFrameActor.h"

class Scene
{
    public:
        Scene(Dali::Stage &stage, Dali::CameraActor &camera, Dali::Layer &uiLayer, FrameActor *plane);
        void Update(double deltaTime);

    public:
        virtual void Init() = 0;
        virtual void OnStart();
        virtual void OnUpdate(double deltaTime);
        virtual void Dispose();
        virtual void OnKeyEvent(const Dali::KeyEvent &event);
        virtual void OnTouch(Dali::Actor actor, const Dali::TouchData &touch);

    protected:
        void AddActor(FrameActor *actor);
        void RemoveActor(FrameActor *actor);
        void AddUI(Dali::Actor &ui);    // todo : Wrap with FrameActor

    protected:
        // Essentials
        // FrameActor is pointer since it can be created and removed at any time
        std::list<FrameActor*> _actorList;
        Dali::Stage _stage;
        Dali::Layer _uiLayer;
        FrameActor *_plane;
        CameraFrameActor *_camera;
};

#endif