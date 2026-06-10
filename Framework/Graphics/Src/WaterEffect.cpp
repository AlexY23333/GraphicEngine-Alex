#include "Precompiled.h"
#include "WaterEffect.h"

#include "VertexTypes.h"
#include "Camera.h"
#include "RenderObject.h"

using namespace SumEngine;
using namespace SumEngine::Graphics;

void WaterEffect::Initialize()
{
	// water shader will have a consistent shaderFile
	std::filesystem::path shaderFile = L"../../Assets/Shaders/WaterWaves.fx";
	mVertexShader.Initialize<Vertex>(shaderFile);
	mPixelShader.Initialize(shaderFile);

	mTransformBuffer.Initialize();
	mSettingsBuffer.Initialize();
	//mLightBuffer.Initialize();
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void WaterEffect::Terminate()
{
	mSampler.Terminate();
	//mLightBuffer.Terminate();
	mSettingsBuffer.Terminate();
	mTransformBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void WaterEffect::Begin()
{
	mVertexShader.Bind();
	mPixelShader.Bind();

	mTransformBuffer.BindVS(0);
	mSettingsBuffer.BindVS(1);
	mSettingsBuffer.BindPS(1);

	//mLightBuffer.BindVS(2);

	mSampler.BindVS(0);
	mSampler.BindPS(0);
}

void WaterEffect::End()
{

}

void WaterEffect::Update(float deltaTime)
{
	mSettingsData.waveTime += deltaTime;
}

void WaterEffect::Render(const RenderObject& renderObject)
{
	ASSERT(mCamera != nullptr, "Water Effect: must have a camera");

	const Math::Matrix4 matWorld = renderObject.transform.GetMatrix4();
	const Math::Matrix4 matView = mCamera->GetViewMatrix();
	const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();

	const Math::Matrix4 matFinal = matWorld * matView * matProj;

	WaterTransform waterTransform;
	waterTransform.wvp = Transpose(matFinal);
	waterTransform.world = Transpose(matWorld);
	waterTransform.viewPosition = mCamera->GetPosition();
	mTransformBuffer.Update(waterTransform);

	SettingsData settingsData;
	settingsData.amplitude1 = mSettingsData.amplitude1;
	settingsData.amplitude2 = mSettingsData.amplitude2;
	settingsData.amplitude3 = mSettingsData.amplitude3;
	settingsData.speed = mSettingsData.speed;
	settingsData.waveLength1 = mSettingsData.waveLength1;
	settingsData.waveLength2 = mSettingsData.waveLength2;
	settingsData.waveLength3 = mSettingsData.waveLength3;
	settingsData.waveTime = mSettingsData.waveTime;

	mSettingsBuffer.Update(settingsData);
	//mLightBuffer.Update(*mDirectionalLight);

	TextureCache* tc = TextureCache::Get();
	tc->BindPS(renderObject.diffuseMapId, 0);
	tc->BindPS(renderObject.normalMapId, 1);

	renderObject.meshBuffer.Render();
}

void WaterEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;
}

void WaterEffect::SetLightCamera(const Camera& camera)
{
	mLightCamera = &camera;
}

void WaterEffect::SetDirectionalLight(const DirectionalLight& directionalLight)
{
	mDirectionalLight = &directionalLight;
}

void WaterEffect::DebugUI()
{
	if (ImGui::CollapsingHeader("Waves", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("Amplitude1##Waves", &mSettingsData.amplitude1, 0.001f, 0.0f, 2.0f);
		ImGui::DragFloat("Amplitude2##Waves", &mSettingsData.amplitude2, 0.001f, 0.0f, 2.0f);
		ImGui::DragFloat("Amplitude3##Waves", &mSettingsData.amplitude3, 0.001f, 0.0f, 2.0f);
		ImGui::DragFloat("WaveLength1##Waves", &mSettingsData.waveLength1, 0.001f, 0.0f, 2.0f);
		ImGui::DragFloat("WaveLength2##Waves", &mSettingsData.waveLength2, 0.001f, 0.0f, 2.0f);
		ImGui::DragFloat("WaveLength3##Waves", &mSettingsData.waveLength3, 0.001f, 0.0f, 2.0f);
		ImGui::DragFloat("Speed##Waves", &mSettingsData.speed, 0.01f, 0.0f, 100.0f);
	}
}
