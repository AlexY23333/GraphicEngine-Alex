#include "GameState.h"
using namespace std;
using namespace SumEngine;
using namespace SumEngine::Math;
using namespace SumEngine::Graphics;
using namespace SumEngine::Core;
using namespace SumEngine::Input;

const char* gDrawTypeNames[] =
{
	"Sun",
	"Mercury",
	"Venus",
	"Earth",
	"Mars",
	"Jupiter",
	"Saturn",
	"Uranus",
	"Neptune",
	"Pluto",
	"Galaxy",
	"Moon"
};

const char* cTextureLocations[] =
{
	"planets/sun.jpg",
	"planets/mercury.jpg",
	"planets/venus.jpg",
	"planets/earth.jpg",
	"planets/mars.jpg",
	"planets/jupiter.jpg",
	"planets/saturn.jpg",
	"planets/neptune.jpg",
	"planets/uranus.jpg",
	"planets/pluto.jpg",
	"skysphere/space.jpg",
	"planets/moon.jpg"
	
};

void GameState::Initialize()
{
	// Set Cameras
	mCamera.SetPosition({ 0.0f, 150.0f, -600.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });
	mRenderTargetCamera.SetPosition({ 0.0f, 0.0f, -300.0f });
	mRenderTargetCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });
	mRenderTargetCamera.SetAspectRatio(1.0f);

	// Create Meshes
	mObjects[(int)SolarSystem::Sun].mMeshBuffer.Initialize<MeshPX>(MeshBuilder::CreateSpherePX(100, 100, 100.0f));
	mObjects[(int)SolarSystem::Mercury].mMeshBuffer.Initialize<MeshPX>(MeshBuilder::CreateSpherePX(100, 100, 3.8f));//size * 10
	mObjects[(int)SolarSystem::Venus].mMeshBuffer.Initialize<MeshPX>(MeshBuilder::CreateSpherePX(100, 100, 4.75f));//size * 5
	mObjects[(int)SolarSystem::Earth].mMeshBuffer.Initialize<MeshPX>(MeshBuilder::CreateSpherePX(100, 100, 5.0f));//size * 5
	mObjects[(int)SolarSystem::Mars].mMeshBuffer.Initialize<MeshPX>(MeshBuilder::CreateSpherePX(100, 100, 5.3f));//size * 10
	mObjects[(int)SolarSystem::Jupiter].mMeshBuffer.Initialize<MeshPX>(MeshBuilder::CreateSpherePX(100, 100, 10.97f));
	mObjects[(int)SolarSystem::Saturn].mMeshBuffer.Initialize<MeshPX>(MeshBuilder::CreateSpherePX(100, 100, 9.14f));
	mObjects[(int)SolarSystem::Uranus].mMeshBuffer.Initialize<MeshPX>(MeshBuilder::CreateSpherePX(100, 100, 3.98f));
	mObjects[(int)SolarSystem::Neptune].mMeshBuffer.Initialize<MeshPX>(MeshBuilder::CreateSpherePX(100, 100, 3.86f));
	mObjects[(int)SolarSystem::Pluto].mMeshBuffer.Initialize<MeshPX>(MeshBuilder::CreateSpherePX(100, 100, 3.6f));//size * 20
	mObjects[(int)SolarSystem::Moon].mMeshBuffer.Initialize<MeshPX>(MeshBuilder::CreateSpherePX(100, 100, 2.7f));// size * 10
	mObjects[(int)SolarSystem::Galaxy].mMeshBuffer.Initialize<MeshPX>(MeshBuilder::CreateSkySpherePX(100, 100, 1000.0f));


	mConstantBuffer.Initialize(sizeof(Matrix4));

	std::filesystem::path shaderFile = L"../../Assets/Shaders/DoTexture.fx";
	mVertexShader.Initialize<VertexPX>(shaderFile);

	mPixelShader.Initialize(shaderFile);

	// Create Textures
	for (int i = 0; i < (int)SolarSystem::End; i++)
	{
		mObjects[i].mDiffuseTexture.Initialize("../../Assets/Images/" + (std::string)cTextureLocations[i]);
	}

	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

	constexpr uint32_t size = 512;
	mRenderTarget.Initialize(size, size, Texture::Format::RGBA_U32);

	// Set Positions
	mObjects[(int)SolarSystem::Sun].distanceFromSun = 0;
	mObjects[(int)SolarSystem::Mercury].distanceFromSun = 160;
	mObjects[(int)SolarSystem::Venus].distanceFromSun = 200;
	mObjects[(int)SolarSystem::Earth].distanceFromSun = 350;
	mObjects[(int)SolarSystem::Mars].distanceFromSun = 410;
	mObjects[(int)SolarSystem::Jupiter].distanceFromSun = 590;
	mObjects[(int)SolarSystem::Saturn].distanceFromSun = 700;
	mObjects[(int)SolarSystem::Uranus].distanceFromSun = 780;
	mObjects[(int)SolarSystem::Neptune].distanceFromSun = 850;
	mObjects[(int)SolarSystem::Pluto].distanceFromSun = 950;
	mObjects[(int)SolarSystem::Galaxy].distanceFromSun = 0;
	
	//set moon position to earth
	mObjects[(int)SolarSystem::Moon].distanceFromSun = 0;
	mObjects[(int)SolarSystem::Moon].distanceFromEarth = 15.0f;


	// Set Revolutions
	mObjects[(int)SolarSystem::Sun].orbitSpeed = 0;
	mObjects[(int)SolarSystem::Mercury].orbitSpeed = 20;
	mObjects[(int)SolarSystem::Venus].orbitSpeed = 25;
	mObjects[(int)SolarSystem::Earth].orbitSpeed = 30;
	mObjects[(int)SolarSystem::Mars].orbitSpeed = 35;
	mObjects[(int)SolarSystem::Jupiter].orbitSpeed = 40;
	mObjects[(int)SolarSystem::Saturn].orbitSpeed = 45;
	mObjects[(int)SolarSystem::Uranus].orbitSpeed = 50;
	mObjects[(int)SolarSystem::Neptune].orbitSpeed = 55;
	mObjects[(int)SolarSystem::Pluto].orbitSpeed = 60;
	mObjects[(int)SolarSystem::Galaxy].orbitSpeed = 0;
	mObjects[(int)SolarSystem::Moon].orbitSpeed = 50.0f;

	// Set Rotations
	mObjects[(int)SolarSystem::Sun].rotationSpeed = 1;
	mObjects[(int)SolarSystem::Mercury].rotationSpeed = 20;
	mObjects[(int)SolarSystem::Venus].rotationSpeed = 25;
	mObjects[(int)SolarSystem::Earth].rotationSpeed = 30;
	mObjects[(int)SolarSystem::Mars].rotationSpeed = 35;
	mObjects[(int)SolarSystem::Jupiter].rotationSpeed = 40;
	mObjects[(int)SolarSystem::Saturn].rotationSpeed = 45;
	mObjects[(int)SolarSystem::Uranus].rotationSpeed = 50;
	mObjects[(int)SolarSystem::Neptune].rotationSpeed = 55;
	mObjects[(int)SolarSystem::Pluto].rotationSpeed = 60;
	mObjects[(int)SolarSystem::Galaxy].rotationSpeed = 0.3;
	mObjects[(int)SolarSystem::Moon].rotationSpeed = 50.0f;

	// Set Render Target Distances
	mObjects[(int)SolarSystem::Sun].renderTargetDistance = -300.0f;
	mObjects[(int)SolarSystem::Mercury].renderTargetDistance = -15.0f;
	mObjects[(int)SolarSystem::Venus].renderTargetDistance = -20.0f;
	mObjects[(int)SolarSystem::Earth].renderTargetDistance = -25.0f;
	mObjects[(int)SolarSystem::Mars].renderTargetDistance = -25.0f;
	mObjects[(int)SolarSystem::Jupiter].renderTargetDistance = -50.0f;
	mObjects[(int)SolarSystem::Saturn].renderTargetDistance = -45.0f;
	mObjects[(int)SolarSystem::Uranus].renderTargetDistance = -20.0f;
	mObjects[(int)SolarSystem::Neptune].renderTargetDistance = -20.0f;
	mObjects[(int)SolarSystem::Pluto].renderTargetDistance = -18.0f;
	mObjects[(int)SolarSystem::Galaxy].renderTargetDistance = -1200.0f;
	mObjects[(int)SolarSystem::Moon].renderTargetDistance = -12.0f;


}

void GameState::Terminate()
{
	mRenderTarget.Terminate();
	mSampler.Terminate();

	for (int i = (int)SolarSystem::End - 1; i >= 0; i--)
	{
		mObjects[i].mDiffuseTexture.Terminate();
	}

	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mConstantBuffer.Terminate();

	for (int i = (int)SolarSystem::End - 1; i >= 0; i--)
	{
		mObjects[i].mMeshBuffer.Terminate();
	}
}

void GameState::UpdateCamera(float deltaTime)
{
	auto input = InputSystem::Get();
	const float moveSpeed = (input->IsKeyDown(KeyCode::LSHIFT) ? 100.0f : 1.0f) * deltaTime;
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
	// 渲染轨道环
	if (ringsToggle)
	{
		// 渲染行星绕太阳的轨道环
		for (int i = 1; i < (int)SolarSystem::Galaxy; i++)
		{
			SimpleDraw::AddGroundCircle(100, mObjects[i].distanceFromSun, { 0,0,0 }, Colors::Gray);
		}

		// 土星环特殊处理
		{
			Matrix4 ringWorldPosition = Matrix4::RotationY(mObjects[(int)SolarSystem::Saturn].rotationSpeed * totalTime) *
				Matrix4::Translation(Vector3::ZAxis * mObjects[(int)SolarSystem::Saturn].distanceFromSun) *
				Matrix4::RotationY(mObjects[(int)SolarSystem::Saturn].orbitSpeed * totalTime / 10.0f);
			Vector3 ringPosition = { ringWorldPosition._41, ringWorldPosition._42, ringWorldPosition._43 };
			SimpleDraw::AddGroundCircle(100, 25, ringPosition, Colors::White);
		}

		// 计算地球位置
		Matrix4 earthMatWorld = Matrix4::RotationY(mObjects[(int)SolarSystem::Earth].rotationSpeed * totalTime) *
			Matrix4::Translation(Vector3::ZAxis * mObjects[(int)SolarSystem::Earth].distanceFromSun) *
			Matrix4::RotationY(mObjects[(int)SolarSystem::Earth].orbitSpeed * totalTime / 10.0f);
		Vector3 earthPosition = { earthMatWorld._41, earthMatWorld._42, earthMatWorld._43 };

		// 渲染月球绕地球的轨道环
		SimpleDraw::AddGroundCircle(100, mObjects[(int)SolarSystem::Moon].distanceFromEarth,
			earthPosition, Colors::LightGray);

		SimpleDraw::Render(mCamera);
	}

	// 渲染太阳系天体
	for (int i = 0; i < (int)SolarSystem::End; i++)
	{
		// 跳过月球，单独处理
		if (i == (int)SolarSystem::Moon) continue;

		mVertexShader.Bind();
		mPixelShader.Bind();
		mObjects[i].mDiffuseTexture.BindPS(0);
		mSampler.BindPS(0);

		Matrix4 matWorld = Matrix4::RotationY(mObjects[i].rotationSpeed * totalTime) *
			Matrix4::Translation(Vector3::ZAxis * mObjects[i].distanceFromSun) *
			Matrix4::RotationY(mObjects[i].orbitSpeed * totalTime / 10.0f);

		Matrix4 matView = mCamera.GetViewMatrix();
		Matrix4 matProj = mCamera.GetProjectionMatrix();
		Matrix4 matFinal = matWorld * matView * matProj;
		Matrix4 wvp = Transpose(matFinal);

		mConstantBuffer.Update(&wvp);
		mConstantBuffer.BindVS(0);
		mObjects[i].mMeshBuffer.Render();
	}

	// 单独渲染月球
	{
		mVertexShader.Bind();
		mPixelShader.Bind();
		mObjects[(int)SolarSystem::Moon].mDiffuseTexture.BindPS(0);
		mSampler.BindPS(0);

		// 先计算地球的世界矩阵
		Matrix4 earthMatWorld = Matrix4::RotationY(mObjects[(int)SolarSystem::Earth].rotationSpeed * totalTime) *
			Matrix4::Translation(Vector3::ZAxis * mObjects[(int)SolarSystem::Earth].distanceFromSun) *
			Matrix4::RotationY(mObjects[(int)SolarSystem::Earth].orbitSpeed * totalTime / 10.0f);

		// 月球的世界矩阵基于地球位置
		Matrix4 moonMatWorld = Matrix4::RotationY(mObjects[(int)SolarSystem::Moon].rotationSpeed * totalTime) *
			Matrix4::Translation(Vector3::ZAxis * mObjects[(int)SolarSystem::Moon].distanceFromEarth) *
			Matrix4::RotationY(mObjects[(int)SolarSystem::Moon].orbitSpeed * totalTime) *
			earthMatWorld;

		Matrix4 matView = mCamera.GetViewMatrix();
		Matrix4 matProj = mCamera.GetProjectionMatrix();
		Matrix4 matFinal = moonMatWorld * matView * matProj;
		Matrix4 wvp = Transpose(matFinal);

		mConstantBuffer.Update(&wvp);
		mConstantBuffer.BindVS(0);
		mObjects[(int)SolarSystem::Moon].mMeshBuffer.Render();
	}

	// 渲染目标特写视图
	{
		mVertexShader.Bind();
		mPixelShader.Bind();
		mObjects[currentRenderTarget].mDiffuseTexture.BindPS(0);
		mSampler.BindPS(0);

		Matrix4 matWorld;

		// 特殊处理月球的渲染目标
		if (currentRenderTarget == (int)SolarSystem::Moon)
		{
			matWorld = Matrix4::RotationY(mObjects[currentRenderTarget].rotationSpeed * totalTime);
			mRenderTargetCamera.SetPosition({ 0.0f, 0.0f, mObjects[currentRenderTarget].renderTargetDistance });
		}
		else
		{
			matWorld = Matrix4::RotationY(mObjects[currentRenderTarget].rotationSpeed * totalTime);
			mRenderTargetCamera.SetPosition({ 0.0f, 0.0f, mObjects[currentRenderTarget].renderTargetDistance });
		}

		Matrix4 matView = mRenderTargetCamera.GetViewMatrix();
		Matrix4 matProj = mRenderTargetCamera.GetProjectionMatrix();
		Matrix4 matFinal = matWorld * matView * matProj;
		Matrix4 wvp = Transpose(matFinal);

		mConstantBuffer.Update(&wvp);
		mConstantBuffer.BindVS(0);

		mRenderTarget.BeginRender();
		mObjects[currentRenderTarget].mMeshBuffer.Render();
		mRenderTarget.EndRender();
	}
}

bool buttonValue = false;

void GameState::DebugUI()
{
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Checkbox("OrbitRings", &ringsToggle);

	int currentDrawType = static_cast<int>(mCurrentTarget);
	if (ImGui::Combo("Objects", &currentRenderTarget, gDrawTypeNames, static_cast<int>(std::size(gDrawTypeNames))))
	{
		mCurrentTarget = (SolarSystem)currentRenderTarget;
	}

	ImGui::DragFloat("RenderTargetDistance", &mObjects[currentDrawType].renderTargetDistance, 1000.0f, -1000.0f, 0.0f);

	ImGui::DragFloat("OrbitSpeed", &mObjects[currentDrawType].orbitSpeed);
	ImGui::DragFloat("RotationSpeed", &mObjects[currentDrawType].rotationSpeed);
	ImGui::Image(
		mRenderTarget.GetRawData(),
		{ 512, 512 },
		{ 0, 0 },	//uv0
		{ 1, 1 },	//uv1
		{ 1, 1, 1, 1 },
		{ 1, 1, 1, 1 });

	ImGui::End();
}