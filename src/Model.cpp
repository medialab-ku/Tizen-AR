#include "Model.h"

Model::Model(Dali::Shader &shader)
    : mShader(shader),
      mTextures(),
      // See btRigidbody.h if you want to check default values
      mMass(0),
      mFriction(0.5),
      mRollingFriction(0),
      mSpinningFriction(0),
      mRestitution(0)
{

}

void
Model::AddTexture(int index, const std::string name)
{
    TextureInfo info;
    info.index = index;
    info.name = name;
    mTextures.push_back(info);
}

btCollisionShape*
Model::CreateCollisionShape()
{
    // Default is box shape
    btCollisionShape *shape = new btBoxShape(btVector3(1, 1, 1));
    return shape;
}