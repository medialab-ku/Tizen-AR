#ifndef __MODEL_H__
#define __MODEL_H__

#include <dali-toolkit/dali-toolkit.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <string>
#include <vector>
#include "Assets.h"

struct TextureInfo
{
	int index;
	std::string name;
};

/*
 * Container of Graphics and Physics data
 */
class Model
{
public:
	Model(Dali::Shader &shader);
	Dali::Shader& GetShader() { return mShader; }
	Dali::Geometry& GetGeometry() { return mGeometry; }
	std::vector<TextureInfo> GetTextures() { return mTextures; }
	float GetMass() const { return mMass; }
	float GetFriction() const { return mFriction; }
	float GetRollingFriction() const { return mRollingFriction; }
	float GetSpinningFriction() const { return mRollingFriction; }
	float GetRestitution() const { return mRestitution; }
	void AddTexture(int index, const std::string name);
	void SetShader(const Dali::Shader &shader) { mShader = shader; }
	void SetGeometry(const Dali::Geometry geometry) { mGeometry = geometry; }
	void SetMass(const float mass) { mMass = mass; }
	void SetFriction(const float friction) { mFriction = friction; }
	void SetRollingFriction(const float rollingFriction) { mRollingFriction = rollingFriction; }
	void SetSpinningFriction(const float spinningFriction) { mSpinningFriction = spinningFriction; }
	void SetRestitution(const float restitution) { mRestitution = restitution; }

    /*
     * Create collision shape for btRigidBody.
     */
	virtual btCollisionShape* CreateCollisionShape();

private:
	std::vector<TextureInfo> mTextures;
	Dali::Shader mShader;
	Dali::Geometry mGeometry;
	float mMass;
	float mFriction;
	float mRollingFriction;
	float mSpinningFriction;
	float mRestitution;
};

#endif