#include "GameState.h"

using namespace SumEngine;
using namespace SumEngine::Math;
using namespace SumEngine::Graphics;
using namespace SumEngine::Core;
using namespace SumEngine::Input;


void GameState::Initialize()
{

	mCamera.SetPosition({ 0.0f, 1.0f, -5.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	mDirectionalLight.direction = Normalize({ 1.0f, -1.0f, 1.0f });
	mDirectionalLight.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
	mDirectionalLight.diffuse = { 0.7f, 0.7f, 0.7f, 1.0f };
	mDirectionalLight.specular = { 0.9f, 0.9f, 0.9f, 1.0f };

	TextureCache* tc = TextureCache::Get();
	
	//mCharater.Initialize("../../Assets/Models/Ortiz/Ortiz.model");
	mCharater.Initialize("../../Assets/Models/Paladin/Paladin.model");

	MeshPX screenQuad = MeshBuilder::CreateScreenQuad();
	mScreenQuad.meshBuffer.Initialize(screenQuad);

	Mesh ground = MeshBuilder::CreatePlane(10.0f, 10.0f, 1);
	mGround.meshBuffer.Initialize(ground);
	mGround.diffuseId = TextureCache::Get()->LoadTexture("misc/concrete.jpg");


	std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
	mStandardEffect.Initialize(shaderFile);
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);

	shaderFile = L"../../Assets/Shaders/PostProcessing.fx";
	mPostProcessEffect.Initialize(shaderFile);
	mPostProcessEffect.SetTexture(&mRenderTarget);
	mPostProcessEffect.SetTexture(&mCombinedTexture, 1);

	mCombinedTexture.Initialize("../../Assets/Images/water/water_height.jpg");

	GraphicsSystem* gs = GraphicsSystem::Get();
	const uint32_t width = gs->GetBackBufferWidth();
	const uint32_t height = gs->GetBackBufferHeight();
	mRenderTarget.Initialize(width, height, Texture::Format::RGBA_U8);
}

void GameState::Terminate()
{
	mStandardEffect.Terminate();
	mGround.Terminate();
	mCharater.Terminate();
	mRenderTarget.Terminate();
	mPostProcessEffect.Terminate();
	mScreenQuad.Terminate();
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
}

void GameState::Render()
{
	mRenderTarget.BeginRender();
	mStandardEffect.Begin();
		mStandardEffect.Render(mCharater);
		mStandardEffect.Render(mGround);
	mStandardEffect.End();
	mRenderTarget.EndRender();

	mPostProcessEffect.Begin();
		mPostProcessEffect.Render(mScreenQuad);
	mPostProcessEffect.End();
}

void GameState::DebugUI()
{
	ImGui::Begin("debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::DragFloat3("Direction##Light", &mDirectionalLight.direction.x, 0.01f))
		{
			mDirectionalLight.direction = Normalize(mDirectionalLight.direction);
		}

		ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
		ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.r);
		ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);
	}
	ImGui::Text("RenderTarget");
	ImGui::Image(
		mRenderTarget.GetRawData(),
		{ 128,128 },
		{ 0,0 },
		{ 1,1 },
		{ 1, 1, 1,1 },
		{ 1, 1, 1,1 }
	);
	mStandardEffect.DebugUI();
	mPostProcessEffect.DebugUI();
	ImGui::End();

}