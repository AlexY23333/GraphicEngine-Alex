#pragma once

#include <SumEngine/Inc/SumEngine.h>
#include "Firework.h"

enum class SceneState
{
	Scene1,
	Scene2,
	Scene3,
	Scene4,
	Scene5,
	Scene6,
	Scene7,
	Scene8,
	Scene9
};

class GameState : public SumEngine::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime);
	void Render() override;
	void DebugUI() override;

private:
	void PlayBGM();
	void StopBGM();

	void PlayBGM2();
	void StopBGM2();

	void UpdateCamera(float deltaTime);

	void UpdateScene1(float deltaTime);
	void UpdateScene2(float deltaTime);
	void UpdateScene3(float deltaTime);
	void UpdateScene4(float deltaTime);
	void UpdateScene5(float deltaTime);
	void UpdateScene6(float deltaTime);
	void UpdateScene7(float deltaTime);

	SumEngine::Graphics::Camera mCamera;
	SumEngine::Graphics::DirectionalLight mDirectionalLight;


	SumEngine::Graphics::StandardEffect mStandardEffect;
	SumEngine::Graphics::RenderObject mGround;

	SumEngine::Graphics::RenderGroup mCharater;
	SumEngine::Graphics::Animator mCharacterAnimator;
	SumEngine::Math::Vector3 mRotation = { 0.0f, 0.0f, 0.0f };
	SumEngine::Graphics::Transform mTempTransform;

	SumEngine::Graphics::RenderGroup mCharacter2;
	SumEngine::Graphics::Animator mCharacterAnimator2;
	SumEngine::Math::Vector3 mRotation2 = { 0.0f, 0.0f, 0.0f };
	SumEngine::Graphics::Transform mTempTransform2;

	SumEngine::Graphics::RenderGroup mCharacter3;
	SumEngine::Graphics::Animator mCharacterAnimator3;
	SumEngine::Math::Vector3 mRotation3 = { 0.0f, 0.0f, 0.0f };
	SumEngine::Graphics::Transform mTempTransform3;

	Firework mFirework;
	Firework mFirework2;
	Firework mFirework3;

	SumEngine::Graphics::Animation mCameraAnimation;

	SumEngine::Graphics::Animation mCameraLastAnimation;

	SceneState mCurrentScene = SceneState::Scene1;
	float mSceneTime = 0.0f;
	float mScene2Timer = 0.0f;
	float mScene3Timer = 0.0f;
	float mScene4Timer = 0.0f;
	float mScene5Timer = 0.0f;
	float mScene6Timer = 0.0f;
	float mScene7Timer = 0.0f;
	float mScene8Timer = 0.0f;
	float mScene9Timer = 0.0f;


	bool mFireworkTriggered = false;
	bool mFireworkTriggered1 = false;

	bool mIsCharacter2Visible = false;
	bool mIsCharacter1Visible = false;

	const float mSceneDuration = 50.0f;
	float mTimer = 0.0f;
	SumEngine::Audio::SoundId mBgmId;
	SumEngine::Audio::SoundId mBgmId2;
	bool mIsPaused = false;

	SumEngine::Math::Vector3 mCamPosition = { 0.0f, 0.0f, 0.0f };
	SumEngine::Math::Vector3 mCamDirection = { 0.0f, 0.0f, 1.0f };
	SumEngine::Math::Vector3 mCamLookAt = { 0.0f, 0.0f, 0.0f };

	SumEngine::Graphics::MeshBuffer mSkyboxMeshBuffer;
	SumEngine::Graphics::VertexShader mSkyboxVS;
	SumEngine::Graphics::PixelShader mSkyboxPS;
	SumEngine::Graphics::Texture mSkyboxTexture;
	SumEngine::Graphics::ConstantBuffer mSkyboxCB;
};