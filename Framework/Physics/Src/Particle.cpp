#include "Precompiled.h"
#include "Particle.h"

using namespace SumEngine;
using namespace SumEngine::Physics;
using namespace SumEngine::Graphics;

void Particle::Initialize()
{
	mLifeTime = 0.0f;
	mCollisionShape.InitializeEmpty();
	mRigidBody.Initialize(mTransform, mCollisionShape, 1.0f, false);
}

void Particle::Terminate()
{
	mRigidBody.Terminate();
	mCollisionShape.Terminate();
}

void Particle::Activate(const ParticleInfo& info, const Math::Vector3& position, const Math::Vector3& velocity)
{
	mInfo = info;
	mLifeTime = info.lifeTime;
	if (mLifeTime > 0.0f)
	{
		mRigidBody.Activate();
	}
	mRigidBody.SetPosition(position);
	mRigidBody.SetVelocity(velocity);
}

void Particle::Update(float deltaTime)
{
	if (IsActive())
	{
		mLifeTime -= deltaTime;
		if (!IsActive())
		{
			mRigidBody.Deactivate();
		}
		else
		{
			const float t = 1.0f - Math::Clamp(mLifeTime / mInfo.lifeTime, 0.0f, 1.0f);
			mTransform.scale = Math::Lerp(mInfo.startScale, mInfo.endScale, t);
		}
	}
}

bool Particle::IsActive() const
{
	return mLifeTime > 0.0f;
}

const Transform& Particle::GetTransform() const
{
	return mTransform;
}

const Color& Particle::GetColor() const
{
	const float t = 1.0f - Math::Clamp(mLifeTime / mInfo.lifeTime, 0.0f, 1.0f);
	return Math::Lerp(mInfo.startColor, mInfo.endColor, t);
}

const Math::Vector3& Particle::GetPosition() const
{
	return mTransform.position;
}
