#include "GameState.h"
using namespace std;
using namespace SumEngine;
using namespace SumEngine::Math;
using namespace SumEngine::Graphics;
using namespace SumEngine::Core;
using namespace SumEngine::Input;


void GameState::Initialize()
{
	// Set Cameras
	mCamera.SetPosition({ 0.0f, 150.0f, -600.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	Mesh mesh = MeshBuilder::CreateSphere(30, 30, 1.0f);
	mRenderObject.meshBuffer.Initialize(mesh);

	std::filesystem::path shaderFile = 
	mStandardEffect.Initialize(shaderFile);
	mStandardEffect.SetCamera(mCamera);
}

void GameState::Terminate()
{
	mRenderObject.Terminate();
	mStandardEffect.Terminate();
}

void GameState::UpdateCamera(float deltaTime)
{
	auto input = InputSystem::Get();
	const float moveSpeed = (input->IsKeyDown(KeyCode::LSHIFT) ? 500.0f : 100.0f) * deltaTime;
	const float turnSpeed = 0.1f * deltaTime;
	if (input->IsKeyDown(KeyCode::W))
	{
		mCamera.Walk(moveSpeed);
	}
	else if (input->IsKeyDown(KeyCode::S))
	{
		mCamera.Walk(-moveSpeed);
	}
	if (input->IsKeyDown(KeyCode::D))
	{
		mCamera.Strafe(moveSpeed);
	}
	else if (input->IsKeyDown(KeyCode::A))
	{
		mCamera.Strafe(-moveSpeed);
	}
	if (input->IsKeyDown(KeyCode::E))
	{
		mCamera.Rise(moveSpeed);
	}
	else if (input->IsKeyDown(KeyCode::Q))
	{
		mCamera.Rise(-moveSpeed);
	}
	if (input->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(input->GetMouseMoveX() * turnSpeed);
		mCamera.Pitch(input->GetMouseMoveY() * turnSpeed);
	}
}

float totalTime = 0.0f;

void GameState::Update(float deltaTime)
{
	totalTime += deltaTime / 10.0f; 
	UpdateCamera(deltaTime);
}

int currentRenderTarget = 0;
float renderTargetDistance = -300.0f;
bool ringsToggle = true;

void GameState::Render()
{
	SimpleDraw::Render(mCamera);

	mStandardEffect.Begin();
	mStandardEffect.Render(mRenderObject);
	mStandardEffect.End();

	

}

bool buttonValue = false;

void GameState::DebugUI()
{
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::End();
}