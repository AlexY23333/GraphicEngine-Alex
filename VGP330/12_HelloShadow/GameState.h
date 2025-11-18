#pragma once

#include <SumEngine/Inc/SumEngine.h>

class GameState : public SumEngine::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime);
	void Render() override;

	void DebugUI() override;

protected:
	void UpdateCamera(float deltaTime);

	SumEngine::Graphics::Camera mCamera;

	SumEngine::Graphics::DirectionalLight mDirectionalLight;

	SumEngine::Graphics::RenderGroup mCharater;

	SumEngine::Graphics::StandardEffect mStandardEffect;

};
