#include "Precompiled.h"
#include "GaussianBlurEffect.h"

#include "Camera.h"
#include "GraphicsSystem.h"
#include "RenderObject.h"
#include "VertexTypes.h"

using namespace SumEngine;
using namespace SumEngine::Graphics;

void GaussianBlurEffect::Initialize()
{
	GraphicsSystem* gs = GraphicsSystem::Get();
	const uint32_t screenWidth = gs->GetBackBufferWidth();
	const uint32_t screenHeight = gs->GetBackBufferHeight();
	mHorizontalBlurRenderTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);
	mVerticalBlurRenderTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);
	
	mSettingsBuffer.Initialize();
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

	std::filesystem::path shaderFile = L"../../Assets/Shaders/GaussianBlur.fx";
	mVertexShader.Initialize<VertexPX>(shaderFile);
	mHorizontalBlurPixelShader.Initialize(shaderFile, "HorizontalBlurPS");
	mVerticalBlurPixelShader.Initialize(shaderFile, "VerticalBlurPS");
}

void GaussianBlurEffect::Terminate()
{
	mVerticalBlurPixelShader.Terminate();
	mHorizontalBlurPixelShader.Terminate();
	mVertexShader.Terminate();
	mSampler.Terminate();
	mSettingsBuffer.Terminate();
	mVerticalBlurRenderTarget.Terminate();
	mHorizontalBlurRenderTarget.Terminate();
}

void GaussianBlurEffect::Begin()
{
	GraphicsSystem* gs = GraphicsSystem::Get();
	SettingsData data;
	data.screenWidth = static_cast<float>(gs->GetBackBufferWidth());
	data.screenHeight = static_cast<float>(gs->GetBackBufferHeight());
	data.multiplier = mBlurSaturation;

	mVertexShader.Bind();
	mSettingsBuffer.Update(data);
	mSettingsBuffer.BindPS(0);
	mSampler.BindPS(0);
}

void GaussianBlurEffect::End()
{
	GraphicsSystem* gs = GraphicsSystem::Get();
	gs->ResetRenderTarget();
	gs->ResetViewport();

	Texture::UnbindPS(0);
}

void GaussianBlurEffect::Render(const RenderObject& renderObject)
{
	ASSERT(mSourceTexture != nullptr, "GaussianBlurEffect: SourceTexture is null!");
	GraphicsSystem* gs = GraphicsSystem::Get();
	mHorizontalBlurRenderTarget.BeginRender();
		mSourceTexture->BindPS(0);
		mHorizontalBlurPixelShader.Bind();
		renderObject.meshBuffer.Render();
	mHorizontalBlurRenderTarget.EndRender();

	for (uint32_t i = 1; i < mBlurIterations; ++i)
	{
		mVerticalBlurRenderTarget.BeginRender();
			mHorizontalBlurRenderTarget.BindPS(0);
			mVerticalBlurPixelShader.Bind();
			renderObject.meshBuffer.Render();
		mVerticalBlurRenderTarget.EndRender();

		mHorizontalBlurRenderTarget.BeginRender();
			mVerticalBlurRenderTarget.BindPS(0);
			mHorizontalBlurPixelShader.Bind();
			renderObject.meshBuffer.Render();
		mHorizontalBlurRenderTarget.EndRender();
	}

	mVerticalBlurRenderTarget.BeginRender();
		mHorizontalBlurRenderTarget.BindPS(0);
		mVerticalBlurPixelShader.Bind();
		renderObject.meshBuffer.Render();
	mVerticalBlurRenderTarget.EndRender();
}

void GaussianBlurEffect::DebugUI()
{
	if (ImGui::CollapsingHeader("GuassianBlur", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragInt("BlurIterations", &mBlurIterations, 1, 1, 100);
		ImGui::DragFloat("BlurSaturation", &mBlurSaturation, 0.001f, 1.0f, 100.0f);
	}
}

void GaussianBlurEffect::SetSourceTexture(const Texture& texture)
{
	mSourceTexture = &texture;
}

const Texture& GaussianBlurEffect::GetHorizontalBlurTexture() const
{
	return mHorizontalBlurRenderTarget;
}

const Texture& GaussianBlurEffect::GetVerticalBlurTexture() const
{
	return mVerticalBlurRenderTarget;
}

const Texture& GaussianBlurEffect::GetResultTexture() const
{
	// The last texture that gets updated
	return mVerticalBlurRenderTarget;
}
