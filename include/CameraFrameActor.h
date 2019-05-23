#ifndef __CAMERA_FRAMEACTOR_H__
#define __CAMERA_FRAMEACTOR_H__

#include <cmath>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>
#include <iostream>
#include "FrameActor.h"

class CameraFrameActor : public FrameActor
{
    public:
        CameraFrameActor(Dali::Stage &stage, Dali::CameraActor &actor);

    private:
        Dali::CameraActor _cameraActor;
};

#endif