#include "GameState.h"

using namespace SumEngine;
using namespace SumEngine::Math;
using namespace SumEngine::Graphics;
using namespace SumEngine::Core;
using namespace SumEngine::Input;
using namespace SumEngine::Audio;

void GameState::Initialize()
{
	mCamera.SetPosition({ -0.225f, 1.076f, -0.953f });
	mCamera.SetDirection({ 0.004f, -0.057f, 0.998f });

	mDirectionalLight.direction = Normalize({ -0.6, -1.0f, 1.0f });
	mDirectionalLight.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
	mDirectionalLight.diffuse = { 0.7f, 0.7f, 0.7f, 1.0f };
	mDirectionalLight.specular = { 0.9f, 0.9f, 0.9f, 1.0f };

	std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
	mStandardEffect.Initialize(shaderFile);
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);

	mBgmId = SoundEffectManager::Get()->Load("../../Assets/Sounds/LeePace.wav");
	mBgmId2 = SoundEffectManager::Get()->Load("../../Assets/Sounds/Autumn.wav");

	Mesh groundMesh = MeshBuilder::CreatePlane(20, 20, 1.0f);
	mGround.meshBuffer.Initialize(groundMesh);
	mGround.diffuseId = TextureCache::Get()->LoadTexture("misc/concrete.jpg");


	//characters
	mCharater.Initialize("../../Assets/Models/Paladin/Paladin.model", &mCharacterAnimator);
	ModelCache::Get()->AddAnimation(mCharater.modelId, "../../Assets/Models/Paladin/Idle.model");
	ModelCache::Get()->AddAnimation(mCharater.modelId, "../../Assets/Models/Paladin/HHDanceM.model");
	ModelCache::Get()->AddAnimation(mCharater.modelId, "../../Assets/Models/Paladin/Salute.model");
	ModelCache::Get()->AddAnimation(mCharater.modelId, "../../Assets/Models/Paladin/JumpingDown.model");
	ModelCache::Get()->AddAnimation(mCharater.modelId, "../../Assets/Models/Paladin/Idle.model");
	ModelCache::Get()->AddAnimation(mCharater.modelId, "../../Assets/Models/Paladin/HHDanceF.model");
	ModelCache::Get()->AddAnimation(mCharater.modelId, "../../Assets/Models/Paladin/HHDanceF.model");


	mCharacterAnimator.Initialize(mCharater.modelId);



	mCharacter2.Initialize("../../Assets/Models/Vanguard/Vanguard.model", &mCharacterAnimator2);
	ModelCache::Get()->AddAnimation(mCharacter2.modelId, "../../Assets/Models/Vanguard/VGDanceTwerk.model");
	ModelCache::Get()->AddAnimation(mCharacter2.modelId, "../../Assets/Models/Vanguard/VGJumpD.model");
	ModelCache::Get()->AddAnimation(mCharacter2.modelId, "../../Assets/Models/Vanguard/VGJumpD.model");
	ModelCache::Get()->AddAnimation(mCharacter2.modelId, "../../Assets/Models/Vanguard/VGJumpD.model");
	ModelCache::Get()->AddAnimation(mCharacter2.modelId, "../../Assets/Models/Vanguard/VGJumpD.model");
	ModelCache::Get()->AddAnimation(mCharacter2.modelId, "../../Assets/Models/Vanguard/VGJumpD.model");

	mCharacterAnimator2.Initialize(mCharacter2.modelId);
	mCharacter2.transform.position = { -0.300f, 0.000f, 0.500f };
	mCharacter2.transform.rotation = Quaternion::CreateFromYawPitchRoll(0.000f, -5.400f, 0.000f);
	mCharacterAnimator2.PlayAnimation(1, true);


	mCharacter3.Initialize("../../Assets/Models/Medea/Medea.model", &mCharacterAnimator3);
	ModelCache::Get()->AddAnimation(mCharacter3.modelId, "../../Assets/Models/Medea/Idle.model");

	mCharacterAnimator3.Initialize(mCharacter3.modelId);
	mCharacter3.transform.position = { 0.300f, 0.000f, -0.500f };
	mCharacter3.transform.rotation = Quaternion::CreateFromYawPitchRoll(0.000f, -5.400f, 0.000f);
	mCharacterAnimator3.PlayAnimation(0, true);





	mFirework.Initialize();
	mFirework2.Initialize();
	mFirework3.Initialize();

	mCameraAnimation = AnimationBuilder()
		.AddPositionKey({ -2.417f, 2.081f, -5.550f }, 0.0f)
		.AddPositionKey({ -2.417f, 2.081f, -5.550f }, 1.0f)
		.AddPositionKey({ -2.417f, 2.081f, -5.550f }, 3.5f, EaseType::EaseInOut)
		.Build();

	mCharacterAnimator.PlayAnimation(0, true);
	mCurrentScene = SceneState::Scene1;

	mSceneTime = 0.0f;
	mScene2Timer = 0.0f;
	mScene3Timer = 0.0f;
	mScene4Timer = 0.0f;
	mScene5Timer = 0.0f;
	mScene6Timer = 0.0f;
	mScene7Timer = 0.0f;
	mScene8Timer = 0.0f;
	mScene9Timer = 0.0f;

	mFireworkTriggered = false;
	mIsCharacter2Visible = true;
	mIsPaused = false;
}

void GameState::Terminate()
{
	mFirework.Terminate();
	mFirework2.Terminate();
	mFirework3.Terminate();

	mStandardEffect.Terminate();
	mCharater.Terminate();
	mCharacter2.Terminate();
	mCharacter3.Terminate();
	mGround.Terminate();
}

void GameState::Update(float deltaTime)
{
	if (mIsPaused)
	{
		UpdateCamera(deltaTime);
	}
	else
	{
		mCharacterAnimator.Update(deltaTime);
		mCharacterAnimator2.Update(deltaTime);
		mCharacterAnimator3.Update(deltaTime);
		mFirework.Update(deltaTime);
		mFirework2.Update(deltaTime);
		mFirework3.Update(deltaTime);
		mTimer += deltaTime;
		switch (mCurrentScene)
		{
		case SceneState::Scene1:
			UpdateScene1(deltaTime);
			break;
		case SceneState::Scene2:
			UpdateScene2(deltaTime);
			break;
		case SceneState::Scene3:
			UpdateScene3(deltaTime);
			break;
		case SceneState::Scene4:
			UpdateScene4(deltaTime);
			break;
		case SceneState::Scene5:
			UpdateScene5(deltaTime);
			break;
		case SceneState::Scene6:
			UpdateScene6(deltaTime);
			break;
		case SceneState::Scene7:
			UpdateScene7(deltaTime);
			break;
		case SceneState::Scene8:
			UpdateScene8(deltaTime);
			break;
		case SceneState::Scene9:
			UpdateScene9(deltaTime);
			break;
		}
	}

	mCamPosition = mCamera.GetPosition();
	mCamDirection = mCamera.GetDirection();
	if (!mIsPaused)
	{
		mTempTransform.position = mCharater.transform.position;
		mTempTransform2.position = mCharacter2.transform.position;
		mTempTransform3.position = mCharacter3.transform.position;

	}
}

void GameState::UpdateScene1(float deltaTime)
{

	mSceneTime += deltaTime;
	mCamera.SetPosition({ -2.417f, 2.081f, -5.550f });

	if (mSceneTime >= 1.0f && !mFireworkTriggered)
	{
		PlayBGM();
		mFirework.Start({ -5.0f, 0.3f, 8.0f });
		mFirework2.Start({ 0.0f, 0.3f, 10.0f });
		mFirework3.Start({ 5.0f, 0.3f, 8.0f });
		mFireworkTriggered = true;
	}

	Vector3 startDir = Normalize({ 0.550f, -0.321f, 0.771f });
	Vector3 endDir = Normalize({ 0.550f, -0.321f, 0.771f });
	float t = Clamp((mSceneTime - 1.0f) / (mSceneDuration - 1.0f), 0.0f, 1.0f);
	Vector3 currentPos = mCameraAnimation.GetTransform(mSceneTime).position;
	Vector3 currentDir = Normalize(Lerp(startDir, endDir, t));
	mCamera.SetPosition(currentPos);
	mCamera.SetDirection(currentDir);

	if (mSceneTime >= 5.0f)
	{
		mCurrentScene = SceneState::Scene2;
		mScene2Timer = 0.0f;

		mCharater.transform.position = { -1.600f, 0.000f, 0.000f };
		mCharater.transform.rotation = Quaternion::CreateFromYawPitchRoll(0.000f, -1.900f, 0.000f);

		mCharacterAnimator.PlayAnimation(1, true);
		mCharacterAnimator3.PlayAnimation(0, true);

		mIsCharacter2Visible = true;
	}
}

void GameState::UpdateScene2(float deltaTime)
{
	mCamera.SetPosition({ 0.636f, 1.061f, -2.046f });
	mCamera.SetDirection(Normalize({ -0.527f, -0.207f, 0.824f }));

	mScene2Timer += deltaTime;
	mScene3Timer == 0.0;

	if (mScene2Timer >= 0.5f)
	{
		mCharacterAnimator.PlayAnimation(2, true);
	}

	if (mScene2Timer >= 1.5f)
	{
		mCurrentScene = SceneState::Scene3;

		mCharacterAnimator.PlayAnimation(3, false);
		mCharacterAnimator2.PlayAnimation(1, true);
	}
}

void GameState::UpdateScene3(float deltaTime)
{
	mCamera.SetPosition({ -2.329f, 0.925f, -0.665f });
	mCamera.SetDirection({ 0.870f, 0.085f, 0.486f });

	mScene3Timer += deltaTime;
	mScene4Timer == 0.0f;
	if (mScene3Timer >= 1.3f)
	{
		mCurrentScene = SceneState::Scene4;
		mCharacterAnimator.PlayAnimation(4, false);
		mCharacterAnimator2.PlayAnimation(1, false);
	}

}

void GameState::UpdateScene4(float deltaTime)
{
	mCamera.SetPosition({ -0.306, 1.614, 1.724 });
	mCamera.SetDirection({ -0.132, -0.319, -0.938 });

	mScene4Timer += deltaTime;
	mScene5Timer == 0.0f;

	if (mScene4Timer >= 1.3f)
	{
		mCurrentScene = SceneState::Scene5;
		mCharacterAnimator.PlayAnimation(-1, false);
		mCharacterAnimator2.PlayAnimation(2, false);
	}
}

void GameState::UpdateScene5(float deltaTime)
{
	mCamera.SetPosition({ -1.554, 1.808, -0.514 });
	mCamera.SetDirection({ 0.827, -0.476, 0.299 });

	mScene5Timer += deltaTime;
	mScene6Timer == 0.0f;

	if (mScene5Timer >= 1.5f)
	{
		mCurrentScene = SceneState::Scene6;
		mCharacter2.transform.position = { 5.900f, 0.000f, 0.500f };
		mCharacter2.transform.rotation = Quaternion::CreateFromYawPitchRoll(0.00f, 7.600f, 0.000f);
		mCharacterAnimator2.PlayAnimation(3, false);
	}
}

void GameState::UpdateScene6(float deltaTime)
{
	mCamera.SetPosition({ -1.014, 0.954, -1.569 });
	mCamera.SetDirection({ 0.922, -0.264, 0.284 });

	mScene6Timer += deltaTime;
	mScene7Timer == 0.0f;

	if (mScene6Timer >= 2.0f)
	{
		mCurrentScene = SceneState::Scene7;
		mCharacterAnimator2.PlayAnimation(0, false);
		mCharacterAnimator.PlayAnimation(5, false);
	}
}

void GameState::UpdateScene7(float deltaTime)
{
	mCamera.SetPosition({ 0.636f, 1.061f, -2.046f });
	mCamera.SetDirection(Normalize({ -0.527f, -0.207f, 0.824f }));

	mScene7Timer += deltaTime;
	mScene8Timer == 0.0f;

	if (mScene7Timer >= 2.0f)
	{
		mCurrentScene = SceneState::Scene8;

		mCharacterAnimator2.PlayAnimation(5, false);
	}
}

void GameState::UpdateScene8(float deltaTime)
{
	mCamera.SetPosition({ 5.255, 2.399, -1.592 });
	mCamera.SetDirection({ 0.152, -0.657, 0.739 });

	mScene8Timer += deltaTime;
	mScene9Timer == 0.0f;

	if (mScene8Timer >= 3.0f)
	{
		mCurrentScene = SceneState::Scene9;

		mCharacterAnimator2.PlayAnimation(-1, false);
		mCharacterAnimator.PlayAnimation(6, true);
		PlayBGM2();
	}
}

void GameState::UpdateScene9(float deltaTime)
{
	mCamera.SetPosition({ 0.509, 0.958, 0.663 });
	mCamera.SetDirection({ -0.966, -0.128, -0.223 });

	mScene9Timer += deltaTime;
}



void GameState::Render()
{
	mStandardEffect.SetCamera(mCamera);

	SimpleDraw::AddTransform(mTempTransform.GetMatrix4());
	SimpleDraw::AddTransform(mTempTransform2.GetMatrix4());
	SimpleDraw::AddTransform(mTempTransform3.GetMatrix4());
	SimpleDraw::Render(mCamera);

	mFirework.Render(mCamera);
	mFirework2.Render(mCamera);
	mFirework3.Render(mCamera);


	mStandardEffect.Begin();
		mStandardEffect.Render(mCharater);
		mStandardEffect.Render(mCharacter2);		
		mStandardEffect.Render(mCharacter3);
		mStandardEffect.Render(mGround);
	mStandardEffect.End();
}

void GameState::DebugUI()
{
	ImGui::Begin("debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Text("Scene Time: %.2f", mTimer);

	if (ImGui::Button("Restart Scene"))
	{
		mCurrentScene = SceneState::Scene1;
		mSceneTime = 0.0f;
		mScene2Timer = 0.0f;
		mScene3Timer = 0.0f;
		mScene4Timer = 0.0f;
		mScene5Timer = 0.0f;

		mFireworkTriggered = false;
		mIsCharacter2Visible = false;
		mIsCharacter1Visible = false;

		mCharacterAnimator.PlayAnimation(0, true);
		mCharacterAnimator2.PlayAnimation(0, true);
		mCharacterAnimator3.PlayAnimation(0, true);
		mCharater.transform.position = Vector3::Zero;
		mCharater.transform.rotation = Quaternion::Identity;
		mCharacter2.transform.position = { -0.300f, 0.000f, 0.500f };
		mCharacter2.transform.rotation = Quaternion::CreateFromYawPitchRoll(0.000f, -5.400f, 0.000f);
		mCharacter3.transform.position = { 0.300f, 0.000f, -0.500f };
		mCharacter3.transform.rotation = Quaternion::CreateFromYawPitchRoll(0.000f, -5.400f, 0.000f);
	}
	ImGui::SameLine();
	ImGui::Checkbox("Pause Scene", &mIsPaused);

	ImGui::Separator();

	if (ImGui::CollapsingHeader("Audio", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::Button("Play BGM")) { PlayBGM(); }
		ImGui::SameLine();
		if (ImGui::Button("Stop BGM")) { StopBGM(); }
		ImGui::SameLine();
		if (ImGui::Button("Stop BGM2")) { StopBGM2(); }

	}

	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::DragFloat3("Direction##Light", &mDirectionalLight.direction.x, 0.01f))
		{
			mDirectionalLight.direction = Normalize(mDirectionalLight.direction);
		}
		ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
		ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.r);
		ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);
	}

	if (ImGui::CollapsingHeader("Camera Info", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("CamPos: %.3f, %.3f, %.3f", mCamPosition.x, mCamPosition.y, mCamPosition.z);
		ImGui::Text("CamDir: %.3f, %.3f, %.3f", mCamDirection.x, mCamDirection.y, mCamDirection.z);

		if (mIsPaused)
		{
			if (ImGui::DragFloat3("Edit CamPos", &mCamPosition.x, 0.1f))
			{
				mCamera.SetPosition(mCamPosition);
			}
			if (ImGui::DragFloat3("Edit LookAt", &mCamLookAt.x, 0.1f))
			{
				mCamera.SetLookAt(mCamLookAt);
			}

		}
	}

	if (ImGui::CollapsingHeader("Character 1 Info (Paladin)", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("Debug Pos##Paladin", &mTempTransform.position.x, 0.1f);
		if (ImGui::DragFloat3("Debug Rot##Paladin", &mRotation.x, 0.1f))
		{
			mTempTransform.rotation = Math::Quaternion::CreateFromYawPitchRoll(mRotation.y, mRotation.x, mRotation.z);
		}
		if (ImGui::Button("Move Character To Transform##Paladin"))
		{
			mCharater.transform = mTempTransform;
		}
	}
	if (ImGui::CollapsingHeader("Character 2 Info (Vanguard)", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("Debug Pos##Vanguard", &mTempTransform2.position.x, 0.1f);
		if (ImGui::DragFloat3("Debug Rot##Vanguard", &mRotation2.x, 0.1f))
		{
			mTempTransform2.rotation = Math::Quaternion::CreateFromYawPitchRoll(mRotation2.y, mRotation2.x, mRotation2.z);
		}
		if (ImGui::Button("Move Character To Transform##Vanguard"))
		{
			mCharacter2.transform = mTempTransform2;
		}
	}
	if (ImGui::CollapsingHeader("Character 3 Info (Medea)", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("Debug Pos##Medea", &mTempTransform3.position.x, 0.1f);
		if (ImGui::DragFloat3("Debug Rot##Medea", &mRotation3.x, 0.1f))
		{
			mTempTransform3.rotation = Math::Quaternion::CreateFromYawPitchRoll(mRotation3.y, mRotation3.x, mRotation3.z);
		}
		if (ImGui::Button("Move Character To Transform##Medea"))
		{
			mCharacter3.transform = mTempTransform3;
		}
	}

	mStandardEffect.DebugUI();

	ImGui::End();
}

void GameState::UpdateCamera(float deltaTime)
{
	auto& io = ImGui::GetIO();
	if (io.WantCaptureMouse || io.WantCaptureKeyboard)
	{
		return;
	}

	auto input = InputSystem::Get();
	const float moveSpeed = (input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f) * deltaTime;
	const float turnSpeed = 0.1f * deltaTime;
	if (input->IsKeyDown(KeyCode::W)) { mCamera.Walk(moveSpeed); }
	else if (input->IsKeyDown(KeyCode::S)) { mCamera.Walk(-moveSpeed); }
	if (input->IsKeyDown(KeyCode::D)) { mCamera.Strafe(moveSpeed); }
	else if (input->IsKeyDown(KeyCode::A)) { mCamera.Strafe(-moveSpeed); }
	if (input->IsKeyDown(KeyCode::E)) { mCamera.Rise(moveSpeed); }
	else if (input->IsKeyDown(KeyCode::Q)) { mCamera.Rise(-moveSpeed); }

	if (input->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(input->GetMouseMoveX() * turnSpeed);
		mCamera.Pitch(input->GetMouseMoveY() * turnSpeed);
	}
}

void GameState::PlayBGM() { SoundEffectManager::Get()->Play(mBgmId, false); }
void GameState::PlayBGM2() { SoundEffectManager::Get()->Play(mBgmId2, false); }
void GameState::StopBGM() { SoundEffectManager::Get()->Stop(mBgmId); }
void GameState::StopBGM2() { SoundEffectManager::Get()->Stop(mBgmId2); }