#pragma once
#include "Precompiled.h"
#include "StandardEffect.h"

#include "Camera.h"
#include "RenderObject.h"
#include "VertexTypes.h"
#include "Texture.h"

using namespace SumEngine;
using namespace SumEngine::Graphics;

void StandardEffect::Initialize(const std::filesystem::path& path)
{
	mTransformBuffer.Initialize();
	
	mLightBuffer.Initialize();

	mMaterialBuffer.Initialize();

	mSettingsBuffer.Initialize();

	mVertexShader.Initialize<Vertex>(path);
	mPixelShader.Initialize(path);
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void StandardEffect::Terminate()
{

	mVertexShader.Terminate();
	mPixelShader.Terminate();
	mSampler.Terminate();
	mTransformBuffer.Terminate();
	mLightBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mSettingsBuffer.Terminate();
}

void StandardEffect::Begin()
{
	mVertexShader.Bind();
	mPixelShader.Bind();

	mTransformBuffer.BindVS(0);
	mLightBuffer.BindPS(1);
	mLightBuffer.BindVS(1);
	mMaterialBuffer.BindPS(2);
	mSettingsBuffer.BindPS(3);
	mSettingsBuffer.BindVS(3);

	mSampler.BindPS(0);
}

void StandardEffect::End()
{
	// null
}

void StandardEffect::Render(const RenderObject& renderObject)
{
	const Math::Matrix4 matMatrix = renderObject.transform.GetMatrix4();
	const Math::Matrix4 matView = mCamera->GetViewMatrix();
	const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();
	const Math::Matrix4 matFinal = matMatrix * matView * matProj;

	TransformData data;
	data.wvp = Math::Transpose(matFinal);
	data.world = Math::Transpose(matMatrix);
	data.viewPosition = mCamera->GetPosition();
	mTransformBuffer.Update(data);
	mLightBuffer.Update(*mDirectionalLight);

	mMaterialBuffer.Update(renderObject.material);

	SettingsData settingsData;
	settingsData.useDiffuseMap = (mSettingsData.useDiffuseMap > 0 && renderObject.diffuseId > 0) ? 1 : 0;
	settingsData.useNormalMap = (mSettingsData.useNormalMap > 0 && renderObject.normalId > 0) ? 1 : 0;
	settingsData.useSpecMap = (mSettingsData.useSpecMap > 0 && renderObject.specularId > 0) ? 1 : 0;
	settingsData.useBumpMap = (mSettingsData.useBumpMap > 0 && renderObject.bumpId > 0) ? 1 : 0;
	settingsData.bumpWeight = mSettingsData.bumpWeight;
	mSettingsBuffer.Update(settingsData);

	TextureCache* tc = TextureCache::Get();
	tc->BindPS(renderObject.diffuseId, 0);
	tc->BindPS(renderObject.normalId, 1);
	tc->BindPS(renderObject.specularId, 2);
	tc->BindVS(renderObject.bumpId, 3);

	renderObject.meshBuffer.Render();
	
}

void StandardEffect::Render(const RenderGroup& renderGroup)
{
	const Math::Matrix4 matMatrix = renderGroup.transform.GetMatrix4();
	const Math::Matrix4 matView = mCamera->GetViewMatrix();
	const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();
	const Math::Matrix4 matFinal = matMatrix * matView * matProj;

	TransformData data;
	data.wvp = Math::Transpose(matFinal);
	data.world = Math::Transpose(matMatrix);
	data.viewPosition = mCamera->GetPosition();
	mTransformBuffer.Update(data);
	mLightBuffer.Update(*mDirectionalLight);
	
	TextureCache* tc = TextureCache::Get();
	SettingsData settingsData;

	for (const RenderObject& renderObject : renderGroup.renderObjects) 
	{
		SettingsData settingsData;
		settingsData.useDiffuseMap = (mSettingsData.useDiffuseMap > 0 && renderObject.diffuseId > 0) ? 1 : 0;
		settingsData.useNormalMap = (mSettingsData.useNormalMap > 0 && renderObject.normalId > 0) ? 1 : 0;
		settingsData.useSpecMap = (mSettingsData.useSpecMap > 0 && renderObject.specularId > 0) ? 1 : 0;
		settingsData.useBumpMap = (mSettingsData.useBumpMap > 0 && renderObject.bumpId > 0) ? 1 : 0;
		settingsData.bumpWeight = mSettingsData.bumpWeight;
		mSettingsBuffer.Update(settingsData);
		mMaterialBuffer.Update(renderObject.material);

		tc->BindPS(renderObject.diffuseId, 0);
		tc->BindPS(renderObject.normalId, 1);
		tc->BindPS(renderObject.specularId, 2);
		tc->BindVS(renderObject.bumpId, 3);

		renderObject.meshBuffer.Render();
	}
}

void StandardEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;

}

void StandardEffect::SetDirectionalLight(const DirectionalLight& directionalLight)
{
	mDirectionalLight = &directionalLight;
}

void StandardEffect::DebugUI()
{
	if (ImGui::CollapsingHeader("StandardEffect", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool useDiffuseMap = mSettingsData.useDiffuseMap > 0;
		if (ImGui::Checkbox("UseDiffuseMap", &useDiffuseMap))
		{
			mSettingsData.useDiffuseMap = (useDiffuseMap) ? 1 : 0;
		}
		bool useNormalMap = mSettingsData.useNormalMap > 0;
		if (ImGui::Checkbox("UseNormalMap", &useNormalMap))
		{
			mSettingsData.useNormalMap = (useNormalMap) ? 1 : 0;
		}
		bool useSpecMap = mSettingsData.useSpecMap > 0;
		if (ImGui::Checkbox("UseSpecMap", &useSpecMap))
		{
			mSettingsData.useSpecMap = (useSpecMap) ? 1 : 0;
		}
		bool useBumpMap = mSettingsData.useBumpMap > 0;
		if (ImGui::Checkbox("UseBumpMap", &useBumpMap))
		{
			mSettingsData.useBumpMap = (useBumpMap) ? 1 : 0;
		}
		ImGui::DragFloat("BumpWeight", &mSettingsData.bumpWeight, 0.01f, 0.0f, 10.0f);

	}
}
