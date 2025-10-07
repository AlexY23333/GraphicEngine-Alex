#pragma once

#include <SumEngine/Inc/SumEngine.h>

class GameState : public SumEngine::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Render() override;
	void DebugUI() override;
	void Update(float deltaTime) override;

protected:
	void UpdateCamera(float deltaTime);

	SumEngine::Graphics::Camera mCamera;
	SumEngine::Graphics::RenderObject mRenderObject;
	SumEngine::Graphics::StandardEffect mStandardEffect;

};
