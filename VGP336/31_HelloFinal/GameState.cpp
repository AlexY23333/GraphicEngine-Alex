#include "GameState.h"

using namespace SumEngine;
using namespace SumEngine::Math;
using namespace SumEngine::Graphics;
using namespace SumEngine::Core;
using namespace SumEngine::Input;
using namespace SumEngine::Audio;

namespace
{
	constexpr float PlayerRadius = 0.45f;
	constexpr float MoveSpeed = 8.0f;
	constexpr float JumpSpeed = 9.5f;
	constexpr float Gravity = -24.0f;
	constexpr float FallResetHeight = -10.0f;
	constexpr float GoalRadius = 1.2f;
	constexpr float GroundSnapTolerance = 0.25f;

	const std::filesystem::path PlayerTemplate = L"../../Assets/Templates/platformer_player.json";
	const std::filesystem::path PlatformTemplate = L"../../Assets/Templates/platformer_platform.json";
	const std::filesystem::path GoalTemplate = L"../../Assets/Templates/platformer_goal.json";
	const std::filesystem::path CameraTemplate = L"../../Assets/Templates/fps_camera.json";
}

void GameState::Initialize()
{
	mGameWorld.AddService<CameraService>();
	mGameWorld.AddService<RenderService>();
	mGameWorld.Initialize(32);

	CreatePlatform("Start", { 0.0f, 0.0f, 0.0f }, { 5.0f, 0.5f, 5.0f });
	CreatePlatform("Step01", { 5.0f, 1.1f, 0.0f }, { 3.0f, 0.5f, 3.0f });
	CreatePlatform("Step02", { 9.0f, 2.0f, 1.8f }, { 3.0f, 0.5f, 3.0f });
	CreatePlatform("Step03", { 13.5f, 2.7f, -1.2f }, { 3.2f, 0.5f, 3.2f });
	CreatePlatform("FinishPlatform", { 18.0f, 2.8f, 0.0f }, { 4.0f, 0.5f, 4.0f });

	GameObject* goal = mGameWorld.CreateGameObject("Goal", GoalTemplate);
	goal->GetComponent<TransformComponent>()->position = mGoalPosition;
	goal->Initialize();

	mPlayer = mGameWorld.CreateGameObject("Player", PlayerTemplate);
	mPlayerTransform = mPlayer->GetComponent<TransformComponent>();
	mPlayer->Initialize();
	ResetPlayer();

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
	UpdateCamera();
	mGameWorld.Update(deltaTime);
}

void GameState::Render()
{
	mGameWorld.Render();
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("31_HelloFinal");
	ImGui::Text("Move: WASD");
	ImGui::Text("Jump: Space");
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
		ResetPlayer();
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

void GameState::ResetPlayer()
{
	mPlayerTransform->position = mSpawnPosition;
	mPlayerVelocity = Vector3::Zero;
	mIsGrounded = false;
	mHasWon = false;
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
		ResetPlayer();
	}

	if (Distance(mPlayerTransform->position, mGoalPosition) <= GoalRadius)
	{
		mHasWon = true;
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
		const bool wasAbove = previousPosition.y - PlayerRadius >= top - GroundSnapTolerance;
		const bool isAtOrBelowTop = playerPosition.y - PlayerRadius <= top + GroundSnapTolerance;
		const bool insideX = Abs(playerPosition.x - platform.center.x) <= platform.halfExtents.x + PlayerRadius;
		const bool insideZ = Abs(playerPosition.z - platform.center.z) <= platform.halfExtents.z + PlayerRadius;

		if (wasAbove && isAtOrBelowTop && insideX && insideZ)
		{
			mPlayerTransform->position.y = top + PlayerRadius;
			return true;
		}
	}

	return false;
}

bool GameState::HasPlatformSupport() const
{
	const Vector3& playerPosition = mPlayerTransform->position;
	for (const Platform& platform : mPlatforms)
	{
		const float top = platform.center.y + platform.halfExtents.y;
		const bool closeToTop = Abs((playerPosition.y - PlayerRadius) - top) <= GroundSnapTolerance;
		const bool insideX = Abs(playerPosition.x - platform.center.x) <= platform.halfExtents.x + PlayerRadius;
		const bool insideZ = Abs(playerPosition.z - platform.center.z) <= platform.halfExtents.z + PlayerRadius;

		if (closeToTop && insideX && insideZ)
		{
			mPlayerTransform->position.y = top + PlayerRadius;
			return true;
		}
	}

	return false;
}
