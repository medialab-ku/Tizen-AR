#include "CameraFrameActor.h"
#include <bullet/BulletCollision/NarrowPhaseCollision/btRaycastCallback.h>
#include <cmath>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>
#include <iostream>

CameraFrameActor::CameraFrameActor(Dali::Stage &stage, Dali::CameraActor &actor)
    : FrameActor(stage, actor),
      mCameraActor(actor)
{
    
}

bool 
CameraFrameActor::Raycast(Dali::Vector2 touch, btDynamicsWorld *dynamicsWorld, RaycastHit &hit)
{
    // btDiscreteDynamicsWorld has rayTest function
    // if we can get screen to world ray,
    // than everything is ok

    Dali::Vector3 pos = mCameraActor.GetCurrentPosition();
    Dali::Vector4 rot = mCameraActor.GetCurrentOrientation().AsVector();
    float fov = mCameraActor.GetFieldOfView();
    float aspect = mCameraActor.GetAspectRatio();
    float far = mCameraActor.GetFarClippingPlane();
    float near = mCameraActor.GetNearClippingPlane();
    
    // Make Projection Matrix
    float e00 = 1.0f / tan(fov/2.0f) / aspect;
    float e11 = 1.0f / tan(fov/2.0f);
    float e22 = far / (far - near);
    float e23 = (near * far) / (far - near);
    float e32 = -1;
    Eigen::Matrix4f projMat;
    projMat << e00, 0, 0, 0,
            0, e11, 0, 0,
            0, 0, e22, e23,
            0, 0, e32, 0;

    Eigen::Matrix4f projMatInverse = projMat.inverse();

    // Make View Matrix
    Eigen::Quaternionf quat(rot.w, rot.x, rot.y, rot.z);
    Eigen::Matrix3f rotMat = quat.matrix();
    Eigen::Vector3f u = -rotMat.row(0);
    Eigen::Vector3f v = rotMat.row(1);
    Eigen::Vector3f n = -rotMat.row(2);
    Eigen::Vector3f eye = Eigen::Vector3f(pos.x, pos.y, pos.z);
    float eye_u = eye.dot(u);
    float eye_v = eye.dot(v);
    float eye_n = eye.dot(n);
    Eigen::Matrix4f viewMat;
    viewMat << u.x(), u.y(), u.z(), -eye_u,
            v.x(), v.y(), v.z(), -eye_v,
            n.x(), n.y(), n.z(), -eye_n,
            0, 0, 0, 1;

    Eigen::Matrix4f viewMatInverse = viewMat.inverse();

    // Get screen position (We are in Projection Space currently)
    Dali::Vector2 screenSize = mStage.GetSize();
    float x = 2.0 * touch.x / screenSize.x - 1;
    float y = 2.0 * touch.y / screenSize.y - 1;
    std::cout << "touch [" << x << ", " << y << "]" << std::endl;

    Eigen::Vector4f start_proj = Eigen::Vector4f(x, -y, 0, 1);
    // (end - start) will be ray direction
    Eigen::Vector4f end_proj = Eigen::Vector4f(x, -y, -1, 1);
    start_proj *= near;
    end_proj *= far;

    // Go Back to View Frustum
    Eigen::Vector4f start_view = projMatInverse * start_proj;
    Eigen::Vector4f end_view = projMatInverse * end_proj;

    // Go Back to World
    Eigen::Vector4f start_world = viewMatInverse * start_view;
    Eigen::Vector4f end_world = viewMatInverse * end_view;

    // Note that we are currently in the Dali coordinates.
    Dali::Vector3 from_dali(start_world.x(), start_world.y(), start_world.z());
    Dali::Vector3 to_dali(end_world.x(), end_world.y(), end_world.z());

    // Now we try raycast
    btVector3 from = wVector3(from_dali).ToBullet();
    btVector3 to = wVector3(to_dali).ToBullet();
    btCollisionWorld::AllHitsRayResultCallback result(from, to);
    result.m_flags |= btTriangleRaycastCallback::kF_KeepUnflippedNormal;
    result.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;

    std::cout << "Raycast from [" << from.getX() << ", " 
            << from.getY() << ", " << from.getZ() << "]"
            << " to [" << to.getX() << ", " 
            << to.getY() << ", " << to.getZ() << "]" 
            << std::endl;
    dynamicsWorld->rayTest(from, to, result);

    if (result.m_collisionObjects.size() > 0)
    {
        hit.rigidbody = (btRigidBody*)result.m_collisionObjects[0]->getUserPointer();
        hit.point = wVector3(result.m_hitPointWorld[0]);
        return true;
    }
    else
    {
        return false;
    }
}

ScreenToWorldResult 
CameraFrameActor::ScreenToWorld(Dali::Vector2 screen)
{
    // same algorithm

    Dali::Vector3 pos = mCameraActor.GetCurrentPosition();
    Dali::Vector4 rot = mCameraActor.GetCurrentOrientation().AsVector();
    float fov = mCameraActor.GetFieldOfView();
    float aspect = mCameraActor.GetAspectRatio();
    float far = mCameraActor.GetFarClippingPlane();
    float near = mCameraActor.GetNearClippingPlane();
    
    // Make Projection Matrix
    float e00 = 1.0f / tan(fov/2.0f) / aspect;
    float e11 = 1.0f / tan(fov/2.0f);
    float e22 = far / (far - near);
    float e23 = (near * far) / (far - near);
    float e32 = -1;
    Eigen::Matrix4f projMat;
    projMat << e00, 0, 0, 0,
            0, e11, 0, 0,
            0, 0, e22, e23,
            0, 0, e32, 0;

    Eigen::Matrix4f projMatInverse = projMat.inverse();

    // Make View Matrix
    Eigen::Quaternionf quat(rot.w, rot.x, rot.y, rot.z);
    Eigen::Matrix3f rotMat = quat.matrix();
    Eigen::Vector3f u = -rotMat.row(0);
    Eigen::Vector3f v = rotMat.row(1);
    Eigen::Vector3f n = -rotMat.row(2);
    Eigen::Vector3f eye = Eigen::Vector3f(pos.x, pos.y, pos.z);
    float eye_u = eye.dot(u);
    float eye_v = eye.dot(v);
    float eye_n = eye.dot(n);
    Eigen::Matrix4f viewMat;
    viewMat << u.x(), u.y(), u.z(), -eye_u,
            v.x(), v.y(), v.z(), -eye_v,
            n.x(), n.y(), n.z(), -eye_n,
            0, 0, 0, 1;

    Eigen::Matrix4f viewMatInverse = viewMat.inverse();

    // Get screen position (We are in Projection Space currently)
    Dali::Vector2 screenSize = mStage.GetSize();
    float x = 2.0 * screen.x / screenSize.x - 1;
    float y = -(2.0 * screen.y / screenSize.y - 1); // I don't know why it need to be negated
    std::cout << "screen [" << x << ", " << y << "]" << std::endl;

    Eigen::Vector4f start_proj = Eigen::Vector4f(x, -y, 0, 1);
    // (end - start) will be ray direction
    Eigen::Vector4f end_proj = Eigen::Vector4f(x, -y, -1, 1);
    start_proj *= near;
    end_proj *= far;

    // Go Back to View Frustum
    Eigen::Vector4f start_view = projMatInverse * start_proj;
    Eigen::Vector4f end_view = projMatInverse * end_proj;

    // Go Back to World
    Eigen::Vector4f start_world = viewMatInverse * start_view;
    Eigen::Vector4f end_world = viewMatInverse * end_view;

    // Note that we are currently in the Dali coordinates.
    Dali::Vector3 from_dali(start_world.x(), start_world.y(), start_world.z());
    Dali::Vector3 to_dali(end_world.x(), end_world.y(), end_world.z());
    Dali::Vector3 dir_dali = (to_dali - from_dali);
    dir_dali.Normalize();

    btVector3 from = wVector3(from_dali).ToBullet();
    btVector3 to = wVector3(to_dali).ToBullet();
    btVector3 dir = wVector3(dir_dali).ToBullet();

    ScreenToWorldResult result;
    result.worldPos = wVector3(from);
    result.direction = wVector3(dir);

    return result;
}