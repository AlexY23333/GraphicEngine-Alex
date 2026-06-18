#pragma once

#include <SumEngine/Inc/SumEngine.h>


class GameState : public SumEngine::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Render() override;
	void DebugUI() override;
	void Update(float deltaTime);

protected:
	struct Platform
	{
		SumEngine::Math::Vector3 center = SumEngine::Math::Vector3::Zero;
		SumEngine::Math::Vector3 halfExtents = SumEngine::Math::Vector3::One;
	};

	void CreatePlatform(const char* name, const SumEngine::Math::Vector3& center, const SumEngine::Math::Vector3& scale);
	void ResetPlayer();
	void UpdatePlayer(float deltaTime);
	void UpdateCamera();
	bool CheckPlatformLanding(const SumEngine::Math::Vector3& previousPosition);
	bool HasPlatformSupport() const;

	SumEngine::GameWorld mGameWorld;
	std::vector<Platform> mPlatforms;

	SumEngine::GameObject* mPlayer = nullptr;
	SumEngine::TransformComponent* mPlayerTransform = nullptr;
	SumEngine::CameraComponent* mCameraComponent = nullptr;

	SumEngine::Math::Vector3 mPlayerVelocity = SumEngine::Math::Vector3::Zero;
	SumEngine::Math::Vector3 mSpawnPosition = { 0.0f, 2.0f, 0.0f };
	SumEngine::Math::Vector3 mGoalPosition = { 29.5f, 5.85f, 0.0f };

	bool mIsGrounded = false;
	bool mHasWon = false;
};
