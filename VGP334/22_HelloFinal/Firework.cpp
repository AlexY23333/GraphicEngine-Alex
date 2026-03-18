#include "Firework.h"

using namespace SumEngine;
using namespace SumEngine::Graphics;
using namespace SumEngine::Physics;
using namespace SumEngine::Audio;
using namespace SumEngine::Math;

void Firework::Initialize()
{
    mParticleSystemEffect.Initialize();

    ParticleSystemInfo fireworkInfo;
    fireworkInfo.textureId = TextureCache::Get()->LoadTexture("explosion.png");
    fireworkInfo.lifeTime = 2.0f;
    fireworkInfo.delay = 0.5f;
    fireworkInfo.particlesPerEmit = { 7, 10 };
    fireworkInfo.timeBetweenEmit = { 0.05f, 0.1f };
    fireworkInfo.spawnAngle = { -20.0f, 20.0f };
    fireworkInfo.spawnLifeTime = { 1.0f, 2.0f };
    fireworkInfo.spawnDirection = Vector3::YAxis;
    fireworkInfo.startColor = { Colors::DarkGray, Colors::Gray };
    fireworkInfo.endColor = { Colors::LightGray, Colors::White };
    fireworkInfo.startScale = { Vector3::One * 0.25f, Vector3::One * 0.5f };
    fireworkInfo.endScale = { Vector3::Zero, Vector3::Zero };
    mFireworkParticles.Initialize(fireworkInfo);

    ParticleSystemInfo explosionInfo;
    explosionInfo.textureId = TextureCache::Get()->LoadTexture("explosion.png");
    explosionInfo.lifeTime = 0.0f;
    explosionInfo.particlesPerEmit = { 300, 300 };
    explosionInfo.timeBetweenEmit = { 0.05f, 0.1f };
    explosionInfo.spawnAngle = { -180.0f, 180.0f };
    explosionInfo.spawnLifeTime = { 1.0f, 2.0f };
    explosionInfo.spawnSpeed = { 6.0f, 10.0f };
    explosionInfo.spawnDirection = Vector3::YAxis;
    explosionInfo.startColor = { Colors::OrangeRed, Colors::Red };
    explosionInfo.endColor = { Colors::PaleVioletRed, Colors::Orange };
    explosionInfo.startScale = { Vector3::One * 0.75f, Vector3::One };
    explosionInfo.endScale = { Vector3::Zero, Vector3::Zero };
    mExplosionParticles.Initialize(explosionInfo);

    mFireworkSoundId = SoundEffectManager::Get()->Load("photongun1.wav");
    mExplosionSoundId = SoundEffectManager::Get()->Load("explosion.wav");

    mFireworkAnimation = AnimationBuilder()
        .AddPositionKey(Vector3::Zero, 0.0f)
        .AddPositionKey({ 0.0f, 10.0f, 0.0f }, 2.0f, EaseType::EaseIn)
      //  .AddEventKey([this]() { PlaySound(mFireworkSoundId); }, 0.40f)
      //  .AddEventKey([this]() { PlaySound(mExplosionSoundId); }, 1.8f)
        .AddEventKey([this]() { mExplosionParticles.SpawnParticles(); }, 2.0f)
        .Build();
}

void Firework::Terminate()
{
    mFireworkParticles.Terminate();
    mExplosionParticles.Terminate();
    mParticleSystemEffect.Terminate();
}

void Firework::Start(const Vector3& position)
{
    mIsActive = true;
    mAnimationTimer = 0.0f;
    mBasePosition = position;
}

void Firework::Update(float deltaTime)
{
    if (!mIsActive)
    {
        return;
    }

    float prevTime = mAnimationTimer;
    mAnimationTimer += deltaTime;

    mFireworkAnimation.PlayEvents(prevTime, mAnimationTimer);

    if (mAnimationTimer > mFireworkAnimation.GetDuration())
    {
        mIsActive = false;
        mAnimationTimer = mFireworkAnimation.GetDuration();
    }

    Transform transform = mFireworkAnimation.GetTransform(mAnimationTimer);

    Vector3 finalPosition = mBasePosition + transform.position;

    mFireworkParticles.SetPosition(finalPosition);
    mExplosionParticles.SetPosition(finalPosition);

    mFireworkParticles.Update(deltaTime);
    mExplosionParticles.Update(deltaTime);
}

void Firework::Render(const Camera& camera)
{
    mParticleSystemEffect.SetCamera(camera);

    mParticleSystemEffect.Begin();
        mFireworkParticles.Render(mParticleSystemEffect);
        mExplosionParticles.Render(mParticleSystemEffect);
    mParticleSystemEffect.End();
}

void Firework::DebugUI()
{
    if (ImGui::CollapsingHeader("Firework", ImGuiTreeNodeFlags_DefaultOpen))
    {
        mFireworkParticles.DebugUI();
    }
}

void Firework::PlaySound(SoundId sound)
{
    SoundEffectManager::Get()->Play(sound);
}