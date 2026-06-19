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

	struct Coin
	{
		SumEngine::Math::Vector3 position = SumEngine::Math::Vector3::Zero;
		SumEngine::TransformComponent* transform = nullptr;
		bool collected = false;
	};

	void CreatePlatform(const char* name, const SumEngine::Math::Vector3& center, const SumEngine::Math::Vector3& scale);
	void CreateCoin(const char* name, const SumEngine::Math::Vector3& position);
	void ResetPlayer(bool resetCoins);
	void UpdatePlayer(float deltaTime);
	void UpdateCoins();
	void UpdateCamera();
	bool CheckPlatformLanding(const SumEngine::Math::Vector3& previousPosition);
	bool HasPlatformSupport() const;
	int GetCollectedCoinCount() const;

	SumEngine::GameWorld mGameWorld;
	std::vector<Platform> mPlatforms;
	std::vector<Coin> mCoins;

	SumEngine::GameObject* mPlayer = nullptr;
	SumEngine::TransformComponent* mPlayerTransform = nullptr;
	SumEngine::CameraComponent* mCameraComponent = nullptr;

	SumEngine::Math::Vector3 mPlayerVelocity = SumEngine::Math::Vector3::Zero;
	SumEngine::Math::Vector3 mSpawnPosition = { 0.0f, 2.0f, 0.0f };
	SumEngine::Math::Vector3 mGoalPosition = { 29.5f, 5.85f, 0.0f };

	bool mIsGrounded = false;
	bool mHasWon = false;
	int mScore = 0;
};
