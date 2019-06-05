#ifndef __DEBUG_SCENE_H__
#define __DEBUG_SCENE_H__

#include "Scene.h"

class DebugScene : public Scene
{
    public:
        DebugScene(Dali::Stage &stage, Dali::CameraActor &camera) : Scene(stage, camera)
        {

        }
};

#endif