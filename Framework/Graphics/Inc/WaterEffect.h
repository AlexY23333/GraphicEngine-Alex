#pragma once

#include "ConstantBuffer.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "VertexShader.h"
#include "DirectionalLight.h"
#include "Material.h"

namespace SumEngine::Graphics
{
	class Camera;
	class RenderObject;
	class Texture;

	class WaterEffect final
	{
	public:
		void Initialize();
		void Terminate();

		void Begin();
		void End();

		void Update(float deltaTime);

		void Render(const RenderObject& renderObject);

		void SetCamera(const Camera& camera);
		void SetLightCamera(const Camera& camera);
		void SetDirectionalLight(const DirectionalLight& directionalLight);

		void DebugUI();

	private:
		// Transform
		struct WaterTransform
		{
			Math::Matrix4 wvp;
			Math::Matrix4 world;
			Math::Vector3 viewPosition;
			float padding = 0.0f;
		};

		// Light
		// Texture
		// Refract = change the normal value

		struct SettingsData
		{
			// Add all the settings for the water waves here
			float amplitude1 = 0.3f;
			float amplitude2 = 0.1f;
			float amplitude3 = 0.2f;
			float waveLength1 = 0.2f;
			float waveLength2 = 0.3f;
			float waveLength3 = 0.1f;
			float speed = 0.2f;
			float waveTime = 0.0f;
			//float padding[2] = { 0.0f };
		};

		using TransformBuffer = TypedConstantBuffer<WaterTransform>;
		using SettingsBuffer = TypedConstantBuffer<SettingsData>;
		//using LightBuffer = TypedConstantBuffer<DirectionalLight>;

		TransformBuffer mTransformBuffer;
		SettingsBuffer mSettingsBuffer;

		VertexShader mVertexShader;
		PixelShader mPixelShader;
		Sampler mSampler;
		SettingsData mSettingsData;
		//LightBuffer mLightBuffer;


		const Camera* mCamera = nullptr;
		const Camera* mLightCamera = nullptr;
		const DirectionalLight* mDirectionalLight = nullptr;

		// Steps overall
		// Get waves going
		// Add peaks
		// Refraction
		// 

		// Pass the texture created by the water into the standard
		// And check if the pixel being rendered is behind the water
	};
}