#include "GameState.h"

using namespace SumEngine;
using namespace SumEngine::Math;
using namespace SumEngine::Graphics;
using namespace SumEngine::Core;
using namespace SumEngine::Input;
using namespace SumEngine::Audio;

namespace
{
	constexpr float PlayerCollisionRadius = 0.45f;
	constexpr float PlayerGroundOffset = 0.0f;
	constexpr float MoveSpeed = 8.0f;
	constexpr float JumpSpeed = 9.5f;
	constexpr float Gravity = -24.0f;
	constexpr float FallResetHeight = -10.0f;
	constexpr float GoalRadius = 1.2f;
	constexpr float CoinCollectRadius = 1.0f;
	constexpr int CoinScoreValue = 100;
	constexpr float GroundSnapTolerance = 0.25f;

	const std::filesystem::path PlayerTemplate = L"../../Assets/Templates/platformer_player.json";
	const std::filesystem::path PlatformTemplate = L"../../Assets/Templates/platformer_platform.json";
	const std::filesystem::path CoinTemplate = L"../../Assets/Templates/platformer_coin.json";
	const std::filesystem::path GoalTemplate = L"../../Assets/Templates/platformer_goal.json";
	const std::filesystem::path CameraTemplate = L"../../Assets/Templates/fps_camera.json";
}

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor({ 0.45f, 0.68f, 0.92f, 1.0f });

	mGameWorld.AddService<CameraService>();
	mGameWorld.AddService<RenderService>();
	mGameWorld.Initialize(48);

	CreatePlatform("Start", { 0.0f, 0.0f, 0.0f }, { 6.0f, 0.5f, 5.0f });
	CreatePlatform("WarmupStep", { 4.7f, 0.8f, 1.8f }, { 2.8f, 0.5f, 2.8f });
	CreatePlatform("LeftFork", { 8.0f, 1.45f, -1.5f }, { 2.6f, 0.5f, 2.6f });
	CreatePlatform("RightFork", { 8.3f, 1.55f, 2.7f }, { 2.4f, 0.5f, 2.4f });
	CreatePlatform("NarrowBridge", { 11.8f, 2.05f, 0.4f }, { 4.2f, 0.45f, 1.1f });
	CreatePlatform("LandingDeck", { 15.4f, 2.55f, 0.4f }, { 3.4f, 0.5f, 3.0f });
	CreatePlatform("HighStep01", { 18.5f, 3.25f, -2.0f }, { 2.4f, 0.5f, 2.4f });
	CreatePlatform("HighStep02", { 21.8f, 3.85f, 1.8f }, { 2.6f, 0.5f, 2.6f });
	CreatePlatform("SkyRunway", { 25.3f, 4.45f, 1.8f }, { 4.0f, 0.5f, 1.2f });
	CreatePlatform("LastJump", { 27.6f, 4.85f, -1.4f }, { 2.2f, 0.5f, 2.2f });
	CreatePlatform("FinishPlatform", { 29.5f, 5.10f, 0.0f }, { 4.5f, 0.5f, 4.0f });

	CreateCoin("CoinStart", { 1.4f, 0.95f, 1.0f });
	CreateCoin("CoinWarmup", { 4.7f, 1.75f, 1.8f });
	CreateCoin("CoinLeftFork", { 8.0f, 2.40f, -1.5f });
	CreateCoin("CoinRightFork", { 8.3f, 2.50f, 2.7f });
	CreateCoin("CoinBridge", { 11.8f, 2.95f, 0.4f });
	CreateCoin("CoinLanding", { 15.4f, 3.50f, 0.4f });
	CreateCoin("CoinHigh01", { 18.5f, 4.20f, -2.0f });
	CreateCoin("CoinHigh02", { 21.8f, 4.80f, 1.8f });
	CreateCoin("CoinRunway", { 25.3f, 5.40f, 1.8f });
	CreateCoin("CoinFinish", { 29.5f, 6.05f, 0.0f });

	GameObject* goal = mGameWorld.CreateGameObject("Goal", GoalTemplate);
	goal->GetComponent<TransformComponent>()->position = mGoalPosition;
	goal->Initialize();

	mPlayer = mGameWorld.CreateGameObject("Player", PlayerTemplate);
	mPlayerTransform = mPlayer->GetComponent<TransformComponent>();
	mPlayer->Initialize();
	ResetPlayer(true);

	GameObject* cameraGO = mGameWorld.CreateGameObject("Camera", CameraTemplate);
	mCameraComponent = cameraGO->GetComponent<CameraComponent>();
	cameraGO->Initialize();
	UpdateCamera();
}

void GameState::Terminate()
{
	mGameWorld.Terminate();
}

void GameState::Update(float deltaTime)
{
	UpdatePlayer(deltaTime);
	UpdateCoins();
	UpdateCamera();
	mGameWorld.Update(deltaTime);
}

void GameState::Render()
{
	mGameWorld.Render();
}

void GameState::DebugUI()
{
	ImGui::SetNextWindowPos({ 20.0f, 20.0f }, ImGuiCond_Always);
	ImGui::SetNextWindowBgAlpha(0.75f);
	ImGui::Begin("HUD", nullptr,
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings);
	ImGui::Text("Score: %d", mScore);
	ImGui::Text("Coins: %d / %d", GetCollectedCoinCount(), static_cast<int>(mCoins.size()));
	ImGui::Text("Coin Count: %d", GetCollectedCoinCount());
	if (mHasWon)
	{
		ImGui::Text("Goal reached!");
	}
	ImGui::End();

	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("31_HelloFinal");
	ImGui::Text("Move: WASD");
	ImGui::Text("Jump: Space");
	ImGui::Separator();
	ImGui::Text("Score: %d", mScore);
	ImGui::Text("Coins: %d / %d", GetCollectedCoinCount(), static_cast<int>(mCoins.size()));
	ImGui::Separator();
	ImGui::Text("Player Position: %.2f, %.2f, %.2f",
		mPlayerTransform->position.x,
		mPlayerTransform->position.y,
		mPlayerTransform->position.z);
	ImGui::Text("Velocity: %.2f, %.2f, %.2f",
		mPlayerVelocity.x,
		mPlayerVelocity.y,
		mPlayerVelocity.z);
	ImGui::Text("Grounded: %s", mIsGrounded ? "Yes" : "No");
	ImGui::Text("Goal: %s", mHasWon ? "Reached" : "Find the gold cube");
	if (ImGui::Button("Reset"))
	{
		ResetPlayer(true);
	}
	ImGui::Separator();
	mGameWorld.DebugUI();
	ImGui::End();
}

void GameState::CreatePlatform(const char* name, const Vector3& center, const Vector3& scale)
{
	GameObject* platform = mGameWorld.CreateGameObject(name, PlatformTemplate);
	TransformComponent* transform = platform->GetComponent<TransformComponent>();
	transform->position = center;
	transform->scale = scale;
	platform->Initialize();

	Platform& platformData = mPlatforms.emplace_back();
	platformData.center = center;
	platformData.halfExtents = scale * 0.5f;
}

void GameState::CreateCoin(const char* name, const Vector3& position)
{
	GameObject* coinObject = mGameWorld.CreateGameObject(name, CoinTemplate);
	TransformComponent* transform = coinObject->GetComponent<TransformComponent>();
	transform->position = position;
	coinObject->Initialize();

	Coin& coin = mCoins.emplace_back();
	coin.position = position;
	coin.transform = transform;
}

void GameState::ResetPlayer(bool resetCoins)
{
	mPlayerTransform->position = mSpawnPosition;
	mPlayerVelocity = Vector3::Zero;
	mIsGrounded = false;
	mHasWon = false;

	if (resetCoins)
	{
		mScore = 0;

		for (Coin& coin : mCoins)
		{
			coin.collected = false;
			if (coin.transform != nullptr)
			{
				coin.transform->position = coin.position;
			}
		}
	}
}

void GameState::UpdatePlayer(float deltaTime)
{
	if (mHasWon)
	{
		mPlayerVelocity = Vector3::Zero;
		return;
	}

	InputSystem* input = InputSystem::Get();
	Vector3 moveDirection = Vector3::Zero;
	if (input->IsKeyDown(KeyCode::D))
	{
		moveDirection.x += 1.0f;
	}
	if (input->IsKeyDown(KeyCode::A))
	{
		moveDirection.x -= 1.0f;
	}
	if (input->IsKeyDown(KeyCode::W))
	{
		moveDirection.z += 1.0f;
	}
	if (input->IsKeyDown(KeyCode::S))
	{
		moveDirection.z -= 1.0f;
	}

	if (MagnitudeSqr(moveDirection) > 0.0f)
	{
		moveDirection = Normalize(moveDirection);
	}

	mPlayerVelocity.x = moveDirection.x * MoveSpeed;
	mPlayerVelocity.z = moveDirection.z * MoveSpeed;

	const bool wantsJump = mIsGrounded && input->IsKeyPressed(KeyCode::SPACE);
	if (wantsJump)
	{
		mPlayerVelocity.y = JumpSpeed;
		mIsGrounded = false;
	}

	const Vector3 previousPosition = mPlayerTransform->position;
	mPlayerVelocity.y += Gravity * deltaTime;
	mPlayerTransform->position += mPlayerVelocity * deltaTime;

	if (mPlayerVelocity.y <= 0.0f && CheckPlatformLanding(previousPosition))
	{
		mPlayerVelocity.y = 0.0f;
		mIsGrounded = true;
	}
	else if (!wantsJump && mPlayerVelocity.y <= 0.0f && HasPlatformSupport())
	{
		mPlayerVelocity.y = 0.0f;
		mIsGrounded = true;
	}
	else
	{
		mIsGrounded = false;
	}

	if (mPlayerTransform->position.y < FallResetHeight)
	{
		ResetPlayer(false);
	}

	if (Distance(mPlayerTransform->position, mGoalPosition) <= GoalRadius)
	{
		mHasWon = true;
	}
}

void GameState::UpdateCoins()
{
	for (Coin& coin : mCoins)
	{
		if (coin.collected)
		{
			continue;
		}

		if (Distance(mPlayerTransform->position, coin.position) <= CoinCollectRadius)
		{
			coin.collected = true;
			mScore += CoinScoreValue;
			coin.transform->position = { coin.position.x, -100.0f, coin.position.z };
		}
	}
}

void GameState::UpdateCamera()
{
	if (mCameraComponent == nullptr || mPlayerTransform == nullptr)
	{
		return;
	}

	Camera& camera = mCameraComponent->GetCamera();
	const Vector3 target = mPlayerTransform->position;
	const Vector3 cameraPosition = target + Vector3{ -8.0f, 6.0f, -10.0f };
	camera.SetPosition(cameraPosition);
	camera.SetLookAt(target + Vector3{ 0.0f, 1.0f, 0.0f });
}

bool GameState::CheckPlatformLanding(const Vector3& previousPosition)
{
	const Vector3& playerPosition = mPlayerTransform->position;
	for (const Platform& platform : mPlatforms)
	{
		const float top = platform.center.y + platform.halfExtents.y;
		const bool wasAbove = previousPosition.y - PlayerGroundOffset >= top - GroundSnapTolerance;
		const bool isAtOrBelowTop = playerPosition.y - PlayerGroundOffset <= top + GroundSnapTolerance;
		const bool insideX = Abs(playerPosition.x - platform.center.x) <= platform.halfExtents.x + PlayerCollisionRadius;
		const bool insideZ = Abs(playerPosition.z - platform.center.z) <= platform.halfExtents.z + PlayerCollisionRadius;

		if (wasAbove && isAtOrBelowTop && insideX && insideZ)
		{
			mPlayerTransform->position.y = top + PlayerGroundOffset;
			return true;
		}
	}

	return false;
}

int GameState::GetCollectedCoinCount() const
{
	int count = 0;
	for (const Coin& coin : mCoins)
	{
		if (coin.collected)
		{
			++count;
		}
	}
	return count;
}

bool GameState::HasPlatformSupport() const
{
	const Vector3& playerPosition = mPlayerTransform->position;
	for (const Platform& platform : mPlatforms)
	{
		const float top = platform.center.y + platform.halfExtents.y;
		const bool closeToTop = Abs((playerPosition.y - PlayerGroundOffset) - top) <= GroundSnapTolerance;
		const bool insideX = Abs(playerPosition.x - platform.center.x) <= platform.halfExtents.x + PlayerCollisionRadius;
		const bool insideZ = Abs(playerPosition.z - platform.center.z) <= platform.halfExtents.z + PlayerCollisionRadius;

		if (closeToTop && insideX && insideZ)
		{
			mPlayerTransform->position.y = top + PlayerGroundOffset;
			return true;
		}
	}

	return false;
}
