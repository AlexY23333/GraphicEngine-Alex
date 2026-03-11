#pragma once

#include <SumEngine/Inc/SumEngine.h>

class GameState : public SumEngine::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime);
	void Render() override;

	void DebugUI() override;

protected:
	void UpdateCamera(float deltaTime);

	SumEngine::Graphics::Camera mCamera;

	SumEngine::Graphics::DirectionalLight mDirectionalLight;

	SumEngine::Graphics::RenderObject mBall;
	SumEngine::Graphics::RenderObject mGround;

	SumEngine::Physics::CollisionShape mBallShape;
	SumEngine::Physics::RigidBody mBallRB;

	SumEngine::Physics::CollisionShape mGroundShape;
	SumEngine::Physics::RigidBody mGroundRB;


	SumEngine::Graphics::StandardEffect mStandardEffect;
};
