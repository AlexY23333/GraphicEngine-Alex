#include "GameState.h"

using namespace SumEngine;
using namespace SumEngine::Math;
using namespace SumEngine::Graphics;
using namespace SumEngine::Core;
using namespace SumEngine::Input;
using namespace SumEngine::Physics;
using namespace SumEngine::Audio;

void GameState::Initialize()
{

	mCamera.SetPosition({ 1.0f, 11.0f, -7.0f });
	mCamera.SetLookAt({ 0.0f, 6.0f, 1.0f });

	mParticleSystemEffect.Initialize();
	mParticleSystemEffect.SetCamera(mCamera);

	// Firework
	ParticleSystemInfo info;
	info.textureId = TextureCache::Get()->LoadTexture("explosion.png");
	int maxParticles = 1000;
	info.lifeTime = 2.0f;
	info.delay = 0.5f;
	info.particlesPerEmit = { 7, 10 };
	info.timeBetweenEmit = { 0.05f, 0.1f };
	info.spawnAngle = { -20.0f, 20.0f };
	info.spawnLifeTime = { 1.0f, 2.0f };
	//info.spawnSpeed = { 0.1f, 0.3f };

	info.spawnDirection = Math::Vector3::YAxis;
	info.startColor = { Colors::DarkGray, Colors::Gray };
	info.endColor = { Colors::LightGray, Colors::White };

	info.startScale = { Math::Vector3::One * 0.25f, Math::Vector3::One * 0.5f };
	info.endScale = { Math::Vector3::Zero, Math::Vector3::Zero };

	mFireworkParticles.Initialize(info);

	// Explosion particle system
	info.lifeTime = 0.0f;
	info.particlesPerEmit = { 300, 300 };
	info.timeBetweenEmit = { 0.05f, 0.1f };
	info.spawnAngle = { -180.0f, 180.0f };
	info.spawnLifeTime = { 1.0f, 2.0f };
	info.spawnSpeed = { 6.0f, 10.0f };

	info.spawnDirection = Math::Vector3::YAxis;
	info.startColor = { Colors::OrangeRed, Colors::Red };
	info.endColor = { Colors::PaleVioletRed, Colors::Orange };

	info.startScale = { Math::Vector3::One * 0.75f, Math::Vector3::One};
	info.endScale = { Math::Vector3::Zero, Math::Vector3::Zero };
	mExplosionParticles.Initialize(info);

	mFireworkSoundId = SoundEffectManager::Get()->Load("photongun1.wav");
	mExplosionSoundId = SoundEffectManager::Get()->Load("explosion.wav");

	mFireworkAnimation = AnimationBuilder()
		.AddPositionKey(Math::Vector3::Zero, 0.0f)
		.AddPositionKey({ 0.0f,10.0f,0.0f }, 2.0f, EaseType::EaseIn)
		.AddEventKey([&]() {PlaySound(mFireworkSoundId); }, 0.50f)
		.AddEventKey([&]() {PlaySound(mExplosionSoundId); }, 1.99f)
		.AddEventKey([&]() {mExplosionParticles.SpawnParticles(); },2.0f)
		.Build();
	mAnimationTimer = 0.0f;
	mAnimationCompleted = false;
}

void GameState::Terminate()
{
	mFireworkParticles.Terminate();
	mExplosionParticles.Terminate();
	mParticleSystemEffect.Terminate();
}

void GameState::Update(float deltaTime)
{
	if (!mAnimationCompleted)
	{
		float prevTime = mAnimationTimer;
		mAnimationTimer += deltaTime;

		mFireworkAnimation.PlayEvents(prevTime, mAnimationTimer);

		if (mAnimationTimer > mFireworkAnimation.GetDuration())
		{
			mAnimationCompleted = true;
			mAnimationTimer = mFireworkAnimation.GetDuration();
		}
	}

	Graphics::Transform transform = mFireworkAnimation.GetTransform(mAnimationTimer);
	mFireworkParticles.SetPosition(transform.position);
	mExplosionParticles.SetPosition(transform.position);


	UpdateCamera(deltaTime);
	mFireworkParticles.Update(deltaTime);
	mExplosionParticles.Update(deltaTime);
}

void GameState::UpdateCamera(float deltaTime)
{
	auto input = InputSystem::Get();
	const float moveSpeed = (input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f) * deltaTime;
	const float turnSpeed = 0.1f * deltaTime;
	if (input->IsKeyDown(KeyCode::W))
	{
		mCamera.Walk(moveSpeed);
	}
	else if (input->IsKeyDown(KeyCode::S))
	{
		mCamera.Walk(-moveSpeed);
	}
	if (input->IsKeyDown(KeyCode::D))
	{
		mCamera.Strafe(moveSpeed);
	}
	else if (input->IsKeyDown(KeyCode::A))
	{
		mCamera.Strafe(-moveSpeed);
	}
	if (input->IsKeyDown(KeyCode::E))
	{
		mCamera.Rise(moveSpeed);
	}
	else if (input->IsKeyDown(KeyCode::Q))
	{
		mCamera.Rise(-moveSpeed);
	}
	if (input->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(input->GetMouseMoveX() * turnSpeed);
		mCamera.Pitch(input->GetMouseMoveY() * turnSpeed);
	}

}

void GameState::PlaySound(SumEngine::Audio::SoundId sound)
{
	SoundEffectManager::Get()->Play(sound);
}

void GameState::Render()
{
	mParticleSystemEffect.Begin();
		mFireworkParticles.Render(mParticleSystemEffect);
		mExplosionParticles.Render(mParticleSystemEffect);
	mParticleSystemEffect.End();
}

void GameState::DebugUI()
{
	ImGui::Begin("debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	mFireworkParticles.DebugUI();

	PhysicsWorld::Get()->DebugUI();
	SimpleDraw::Render(mCamera);

	SimpleDraw::AddGroundPlane(10.0f, Colors::Gray);
	ImGui::End();
}