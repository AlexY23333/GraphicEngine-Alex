#include "GameState.h"

using namespace SumEngine;
using namespace SumEngine::Math;
using namespace SumEngine::Graphics;
using namespace SumEngine::Core;
using namespace SumEngine::Input;

void GameState::Initialize()
{

	LOG("CWD=%s", std::filesystem::current_path().u8string().c_str());
	LOG("FX exists=%d", std::filesystem::exists(L"../../Assets/Shaders/Standard.fx") ? 1 : 0);
	LOG("HM exists=%d", std::filesystem::exists(L"../../Assets/Images/terrain/heightmap_512x512.raw") ? 1 : 0);
	LOG("FX abs=%s", std::filesystem::absolute(L"../../Assets/Shaders/Standard.fx").u8string().c_str());


	mCamera.SetPosition({ 10.0f, 2.0f, 40.0f });
	mCamera.SetLookAt({ 0.0f, 1.0f, 0.0f });

	mDirectionalLight.direction = Normalize({ 1.0f, -1.0f, 1.0f });
	mDirectionalLight.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
	mDirectionalLight.diffuse = { 0.7f, 0.7f, 0.7f, 1.0f };
	mDirectionalLight.specular = { 0.9f, 0.9f, 0.9f, 1.0f };

	//mCharacter.Initialize(L"../../Assets/Models/Ortiz/Ortiz.model");

	mTerrain.Initialize(L"../../Assets/Images/terrain/heightmap_512x512.raw", 40.0f, 20.f);

	mGround.meshBuffer.Initialize(mTerrain.GetMesh());
	mGround.diffuseId = TextureCache::Get()->LoadTexture("terrain/dirt_seamless.jpg");
	mGround.normalId = TextureCache::Get()->LoadTexture("terrain/grass_2048.jpg");


	std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
	mStandardEffect.Initialize(shaderFile);
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);


	mTerrainEffect.Initialize();
	mTerrainEffect.SetCamera(mCamera);
	mTerrainEffect.SetDirectionalLight(mDirectionalLight);

}

void GameState::Terminate()
{
	mGround.Terminate();
	//mCharacter.Terminate();
	mTerrainEffect.Terminate();
	mStandardEffect.Terminate();
}


void GameState::Update(float deltaTime)
{
	UpdateCamera(deltaTime);
}

void GameState::UpdateCamera(float deltaTime)
{
	auto input = InputSystem::Get();
	const float moveSpeed = (input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f) * deltaTime;
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

	Vector3 camPos = mCamera.GetPosition();
	float height = mTerrain.GetHeight(camPos);
	camPos.y = height + 1.5f;
	mCamera.SetPosition(camPos);
}

bool checkBox = true;

void GameState::Render()
{
	//mStandardEffect.Begin();
	//	mStandardEffect.Render(mCharacter);
	//mStandardEffect.End();

	mTerrainEffect.Begin();
		mTerrainEffect.Render(mGround);
	mTerrainEffect.End();
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
		if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::DragFloat3("Direction", &mDirectionalLight.direction.x, 0.01f))
			{
				mDirectionalLight.direction = Normalize(mDirectionalLight.direction);
			}

			ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
			ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.r);
			ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);
		}

		ImGui::Separator();
		ImGui::Text("RenderTarget");

		ImGui::Checkbox("CheckBox", &checkBox);
	mStandardEffect.DebugUI();
	mTerrainEffect.DebugUI();
	ImGui::End();
}

