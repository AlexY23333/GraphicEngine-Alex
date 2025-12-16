#pragma once

#include <SumEngine/Inc/SumEngine.h>

class GameState : public SumEngine::AppState
{
public:
    void Initialize() override;
    void Terminate() override;

    void Update(float deltaTime) override;
    void Render() override;

    void DebugUI() override;

protected:
    void UpdateCamera(float deltaTime);

    SumEngine::Graphics::Camera mCamera;
    SumEngine::Graphics::DirectionalLight mDirectionalLight;

    SumEngine::Graphics::StandardEffect mStandardEffect;
    SumEngine::Graphics::ShadowEffect mShadowEffect;
    SumEngine::Graphics::OutlineEffect mOutlineEffect;

    SumEngine::Graphics::RenderObject mGround;

    SumEngine::Graphics::RenderObject mSphere;
    SumEngine::Graphics::RenderObject mSphereOutline;

    SumEngine::Graphics::RenderObject mCube;
    SumEngine::Graphics::RenderObject mCubeOutline;

    SumEngine::Graphics::RenderGroup mCharater; 
    SumEngine::Graphics::RenderGroup mCharaterOutLine; 
};