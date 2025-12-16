#include "Precompiled.h"
#include "OutlineEffect.h"
#include "VertexTypes.h"
#include "Camera.h"
#include "RenderObject.h"

using namespace SumEngine;
using namespace SumEngine::Graphics;

void OutlineEffect::Initialize()
{
    std::filesystem::path shaderFile = L"../../Assets/Shaders/Outline.fx";
    mVertexShader.Initialize<Vertex>(shaderFile);
    mPixelShader.Initialize(shaderFile);

    mTransformBuffer.Initialize();
    mSettingsBuffer.Initialize();
    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void OutlineEffect::Terminate()
{
    mSampler.Terminate();
    mSettingsBuffer.Terminate();
    mTransformBuffer.Terminate();
    mPixelShader.Terminate();
    mVertexShader.Terminate();
}

void OutlineEffect::Begin()
{
    mVertexShader.Bind();
    mPixelShader.Bind();

    mTransformBuffer.BindVS(0);
    mTransformBuffer.BindPS(0);
    mSettingsBuffer.BindVS(1);
    mSettingsBuffer.BindPS(1);

    mSampler.BindVS(0);
    mSampler.BindPS(0);
}

void OutlineEffect::End()
{
    // Optional: Add any cleanup if needed
}

void OutlineEffect::Render(const RenderObject& renderObject)
{
    ASSERT(mCamera != nullptr, "Outline Effect: must have a camera");

    const Math::Matrix4 matWorld = renderObject.transform.GetMatrix4();
    const Math::Matrix4 matView = mCamera->GetViewMatrix();
    const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();
    const Math::Matrix4 matFinal = matWorld * matView * matProj;

    TransformData transformData;
    transformData.wvp = Transpose(matFinal);
    transformData.world = Transpose(matWorld);
    transformData.viewPosition = mCamera->GetPosition();
    mTransformBuffer.Update(transformData);

    SettingsData settings = mSettingsData;
    mSettingsBuffer.Update(settings);

    renderObject.meshBuffer.Render();
}

void OutlineEffect::Render(const RenderGroup& renderGroup)
{
    ASSERT(mCamera != nullptr, "Outline Effect: must have a camera");

    const Math::Matrix4 matWorld = renderGroup.transform.GetMatrix4();
    const Math::Matrix4 matView = mCamera->GetViewMatrix();
    const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();
    const Math::Matrix4 matFinal = matWorld * matView * matProj;

    TransformData transformData;
    transformData.wvp = Transpose(matFinal);
    transformData.world = Transpose(matWorld);
    transformData.viewPosition = mCamera->GetPosition();
    mTransformBuffer.Update(transformData);

    SettingsData settings = mSettingsData;
    mSettingsBuffer.Update(settings);
    for (const auto& renderObject : renderGroup.renderObjects)
	{
		renderObject.meshBuffer.Render();
	}
}

void OutlineEffect::SetCamera(const Camera& camera)
{
    mCamera = &camera;
}