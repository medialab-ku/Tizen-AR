#include "PhysicsActor.h"

PhysicsActor::PhysicsActor(Dali::Stage &stage, Model &model, btDynamicsWorld *dynamicsWorld)
    : GraphicsActor(stage, model),
      mDynamicsWorld(dynamicsWorld)
{
    // Collision shape data is provided by Model
    auto shape = model.CreateCollisionShape();
    float mass = model.GetMass();
    btVector3 localInertia(0, 0, 0);
    shape->calculateLocalInertia(mass, localInertia);

    btTransform transform;
    transform.setIdentity();

    btDefaultMotionState *ms = new btDefaultMotionState(transform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, ms, shape, localInertia);
    rbInfo.m_friction = model.GetFriction();
    rbInfo.m_rollingFriction = model.GetRollingFriction();
    rbInfo.m_spinningFriction = model.GetSpinningFriction();
    rbInfo.m_restitution = model.GetRestitution();

    mRigidBody = new btRigidBody(rbInfo);
    mDynamicsWorld->addRigidBody(mRigidBody);
}

PhysicsActor::~PhysicsActor()
{
    if (mRigidBody && mRigidBody->getMotionState())
    {
        delete mRigidBody->getMotionState();
    }

    mDynamicsWorld->removeCollisionObject(mRigidBody);
    delete mRigidBody;
}

void
PhysicsActor::SetPosition(float x, float y, float z)
{
    FrameActor::SetPosition(x, y, z);
    auto transform = mRigidBody->getWorldTransform();
    transform.setOrigin(btVector3(x, y, z));
    mRigidBody->setWorldTransform(transform);
    mRigidBody->getMotionState()->setWorldTransform(transform);
}

void
PhysicsActor::SetPosition(wVector3 pos)
{
    SetPosition(pos.x, pos.y, pos.z);
}

void
PhysicsActor::SetRotation(float x, float y, float z, float w)
{
    FrameActor::SetRotation(x, y, z, w);
    auto transform = mRigidBody->getWorldTransform();
    transform.setRotation(btQuaternion(x, y, z, w));
    mRigidBody->setWorldTransform(transform);
    mRigidBody->getMotionState()->setWorldTransform(transform);
}

void
PhysicsActor::SetRotation(wQuaternion rotation)
{
    SetRotation(rotation.x, rotation.y, rotation.z, rotation.w);
}

void
PhysicsActor::SetSize(float x, float y, float z)
{
    FrameActor::SetSize(x, y, z);
    auto colShape = mRigidBody->getCollisionShape();
    colShape->setLocalScaling(btVector3(x, y, z));
}

void
PhysicsActor::SetSize(wVector3 size)
{
    SetSize(size.x, size.y, size.z);
}

void
PhysicsActor::RotateBy(wQuaternion rot)
{
    FrameActor::RotateBy(rot);
    SetRotation(mRotation);
}

void
PhysicsActor::OnUpdate(double deltaTime)
{
    // Update transform of Dali::Actor
    btTransform trans;
    mRigidBody->getMotionState()->getWorldTransform(trans);
    btVector3 pos = trans.getOrigin();
    btQuaternion rot = trans.getRotation();

    wVector3 wpos(pos);
    wQuaternion wrot(rot);
    FrameActor::SetPosition(wpos);
    FrameActor::SetRotation(wrot);
}

void
PhysicsActor::ApplyForce(wVector3 force)
{
    mRigidBody->applyCentralForce(force.ToBullet());
}