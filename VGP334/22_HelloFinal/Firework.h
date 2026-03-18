#pragma once

#include <SumEngine/Inc/SumEngine.h>

class Firework
{
public:
    void Initialize();
    void Terminate();

    void Update(float deltaTime);
    void Render(const SumEngine::Graphics::Camera& camera);
    void DebugUI();

    void Start(const SumEngine::Math::Vector3& position);

private:
    void PlaySound(SumEngine::Audio::SoundId sound);

    SumEngine::Physics::ParticleSystem mFireworkParticles;
    SumEngine::Physics::ParticleSystem mExplosionParticles;
    SumEngine::Graphics::ParticleSystemEffect mParticleSystemEffect;

    SumEngine::Graphics::Animation mFireworkAnimation;
    float mAnimationTimer = 0.0f;
    bool mIsActive = false;

    SumEngine::Math::Vector3 mBasePosition = SumEngine::Math::Vector3::Zero;

    SumEngine::Audio::SoundId mExplosionSoundId;
    SumEngine::Audio::SoundId mFireworkSoundId;
};