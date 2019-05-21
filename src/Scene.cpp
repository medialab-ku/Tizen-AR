#include "Scene.h"
#include <iostream>

Scene::Scene(Dali::Stage &stage, Dali::CameraActor &camera, Dali::Layer &uiLayer,
    btDiscreteDynamicsWorld *dynamicsWorld, FrameActor *plane)
    : mListActor(),
      mStage(stage),
      mUILayer(uiLayer),
      mDynamicsWorld(dynamicsWorld),
      mPlane(plane)
{
    AddActor(plane);
    mCamera = new CameraFrameActor(stage, camera);
    AddActor(mCamera);
}

void
Scene::Update(double deltaTime)
{
    mDynamicsWorld->stepSimulation(deltaTime);
    OnUpdate(deltaTime);
    for(auto itr = mListActor.begin(); itr != mListActor.end(); ++itr)
    {
        (*itr)->OnUpdate(deltaTime);
    }
}

void
Scene::Dispose()
{
    mListActor.clear();
}

void 
Scene::AddActor(FrameActor *actor)
{
    bool found = (std::find(mListActor.begin(), mListActor.end(), actor) != mListActor.end());
    if (!found)
    {
        mListActor.push_back(actor);
    }
}

void 
Scene::RemoveActor(FrameActor *actor)
{
    bool found = (std::find(mListActor.begin(), mListActor.end(), actor) != mListActor.end());
    if (found)
    {
        mListActor.remove(actor);
        delete actor;
    }
}

void
Scene::AddUI(Dali::Actor &ui)
{
    mUILayer.Add(ui);
}

void
Scene::OnStart()
{
    // Put all actors on the plane
    for(auto itr = mListActor.begin(); itr != mListActor.end(); ++itr)
    {
        std::cout << (*itr)->GetPosition().ToDali() << std::endl;
        if ((*itr) == mPlane) continue;

        auto pos = (*itr)->GetPosition();
        auto rot = (*itr)->GetRotation();
        (*itr)->SetPosition(pos + mPlane->GetPosition());
        (*itr)->RotateBy(mPlane->GetRotation());
        std::cout << (*itr)->GetName() << ": " << pos.ToDali() << std::endl;
    }

    for(auto itr = mListActor.begin(); itr != mListActor.end(); ++itr)
    {
        (*itr)->OnStart();
    }
}

void
Scene::OnUpdate(double deltaTime)
{
    mDynamicsWorld->stepSimulation(deltaTime, 10);
    for(auto itr = mListActor.begin(); itr != mListActor.end(); ++itr)
    {
        (*itr)->OnUpdate(deltaTime);
    }
}

void
Scene::OnKeyEvent(const Dali::KeyEvent &event)
{

}

void
Scene::OnTouch(Dali::Actor actor, const Dali::TouchData &touch)
{

}