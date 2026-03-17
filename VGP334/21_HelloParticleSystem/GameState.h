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
	void SpawnExplosion();
	void PlaySound(SumEngine::Audio::SoundId sound);

	SumEngine::Graphics::Camera mCamera;

	SumEngine::Physics::ParticleSystem mFireworkParticles;
	SumEngine::Physics::ParticleSystem mExplosionParticles;

	SumEngine::Graphics::ParticleSystemEffect mParticleSystemEffect;

	SumEngine::Graphics::Animation mFireworkAnimation;
	float mAnimationTimer = 0.0f;

	SumEngine::Audio::SoundId mExplosionSoundId;
	SumEngine::Audio::SoundId mFireworkSoundId;
	bool mAnimationCompleted = false;
};
