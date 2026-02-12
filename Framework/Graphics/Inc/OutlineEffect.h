#pragma once

#include "ConstantBuffer.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "VertexShader.h"
#include "RenderObject.h"

namespace SumEngine::Graphics
{
    class Camera;

    class OutlineEffect final
    {
    public:
        void Initialize();
        void Terminate();

        void Begin();
        void End();

        void Render(const RenderObject& renderObject);
        void Render(const RenderGroup& renderGroup);

        void SetCamera(const Camera& camera);

        void SetOutlineWidth(float width) { mSettingsData.outlineWidth = width; }
        void SetOutlineColor(const Math::Vector3& color) { mSettingsData.outlineColor = color; }

    private:
        struct TransformData
        {
            Math::Matrix4 wvp;
            Math::Matrix4 world;
            Math::Vector3 viewPosition;
            float padding = 0.0f;
        };

        struct SettingsData
        {
            float outlineWidth = 0.05f;
            Math::Vector3 outlineColor = { 0.0f, 0.0f, 0.0f };
        };

        using TransformBuffer = TypedConstantBuffer<TransformData>;
        using SettingsBuffer = TypedConstantBuffer<SettingsData>;

        TransformBuffer mTransformBuffer;
        SettingsBuffer mSettingsBuffer;

        VertexShader mVertexShader;
        PixelShader mPixelShader;
        Sampler mSampler;
        SettingsData mSettingsData;

        const Camera* mCamera = nullptr;
    };
}