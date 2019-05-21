#ifndef __CAMERA_FRAMEACTOR_H__
#define __CAMERA_FRAMEACTOR_H__

#include "FrameActor.h"

class RaycastHit
{
    public:
        btRigidBody* rigidbody;
        wVector3 point;
};

class ScreenToWorldResult
{
    public:
        wVector3 worldPos;
        wVector3 direction;
};

class CameraFrameActor : public FrameActor
{
    public:
        CameraFrameActor(Dali::Stage &stage, Dali::CameraActor &actor);

        /**
         * @brief
         * Check if a rigidbody hits.
         * 
         * @param hit
         * Raycast result output.
         */
        bool Raycast(Dali::Vector2 touch, btDynamicsWorld *dynamicsWorld, RaycastHit &hit);

        /**
         * Convert screen position to world position
         */
        ScreenToWorldResult ScreenToWorld(Dali::Vector2 screen);

    private:
        Dali::CameraActor mCameraActor;
};

#endif