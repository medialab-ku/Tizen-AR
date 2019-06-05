#include "CameraFrameActor.h"

CameraFrameActor::CameraFrameActor(Dali::Stage &stage, Dali::CameraActor &actor)
    : FrameActor(stage, actor),
      _cameraActor(actor)
{
    
}

Vec3
CameraFrameActor::GetRealPosition()
{
    return GetPosition();
}

Quat 
CameraFrameActor::GetRealRotation()
{
    return GetRotation();
}