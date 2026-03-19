#include "GameState.h"

using namespace SumEngine;
using namespace SumEngine::Math;
using namespace SumEngine::Graphics;
using namespace SumEngine::Core;
using namespace SumEngine::Input;
using namespace SumEngine::Audio;

void GameState::Initialize()
{


	mDirectionalLight.direction = Normalize({ 0.999, -0.010f, 0.040f });
	mDirectionalLight.ambient = { 1, 1, 1, 1 };
	mDirectionalLight.diffuse = { 1, 1, 1, 1 };
	mDirectionalLight.specular = { 1, 1, 1, 1 };


	std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
	mStandardEffect.Initialize(shaderFile);
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);

	mBgmId = SoundEffectManager::Get()->Load("../../Assets/Sounds/LeePace.wav");
	mBgmId2 = SoundEffectManager::Get()->Load("../../Assets/Sounds/30K.wav");

	Mesh groundMesh = MeshBuilder::CreatePlane(20, 20, 1.0f);
	mGround.meshBuffer.Initialize(groundMesh);
	mGround.diffuseId = TextureCache::Get()->LoadTexture("misc/concrete.jpg");


	//characters
	mCharater.Initialize("../../Assets/Models/Paladin/Paladin.model", &mCharacterAnimator);
	ModelCache::Get()->AddAnimation(mCharater.modelId, "../../Assets/Models/Paladin/Idle.model");
	ModelCache::Get()->AddAnimation(mCharater.modelId, "../../Assets/Models/Paladin/Walking.model");
	ModelCache::Get()->AddAnimation(mCharater.modelId, "../../Assets/Models/Paladin/Salute.model");
	ModelCache::Get()->AddAnimation(mCharater.modelId, "../../Assets/Models/Paladin/JumpingDown.model");
	ModelCache::Get()->AddAnimation(mCharater.modelId, "../../Assets/Models/Paladin/HHDanceF.model");
	ModelCache::Get()->AddAnimation(mCharater.modelId, "../../Assets/Models/Paladin/WaveDance.model");
	ModelCache::Get()->AddAnimation(mCharater.modelId, "../../Assets/Models/Paladin/HHDanceF.model");


	mCharacterAnimator.Initialize(mCharater.modelId);




	mCharacter2.Initialize("../../Assets/Models/Vanguard/Vanguard.model", &mCharacterAnimator2);
	ModelCache::Get()->AddAnimation(mCharacter2.modelId, "../../Assets/Models/Vanguard/HappyIdle.model");
	ModelCache::Get()->AddAnimation(mCharacter2.modelId, "../../Assets/Models/Vanguard/VGDanceTwerk.model");
	ModelCache::Get()->AddAnimation(mCharacter2.modelId, "../../Assets/Models/Vanguard/WaveDance.model");
	ModelCache::Get()->AddAnimation(mCharacter2.modelId, "../../Assets/Models/Vanguard/Walking.model");
	ModelCache::Get()->AddAnimation(mCharacter2.modelId, "../../Assets/Models/Vanguard/VGJumpD.model");
	ModelCache::Get()->AddAnimation(mCharacter2.modelId, "../../Assets/Models/Vanguard/VGJumpD.model");

	mCharacterAnimator2.Initialize(mCharacter2.modelId);
	mCharacter2.transform.position = { 0.0f, 0.000f, 0.0f };
	mCharacter2.transform.rotation = Quaternion::CreateFromYawPitchRoll(0.000f, 0.000f, 0.000f);


	mCharacter3.Initialize("../../Assets/Models/Ninja/Ninja.model", &mCharacterAnimator3);
	ModelCache::Get()->AddAnimation(mCharacter3.modelId, "../../Assets/Models/Ninja/BoredIdle.model");
	ModelCache::Get()->AddAnimation(mCharacter3.modelId, "../../Assets/Models/Ninja/HipHopDancing.model");
	ModelCache::Get()->AddAnimation(mCharacter3.modelId, "../../Assets/Models/Ninja/WaveDance.model");


	mCharacterAnimator3.Initialize(mCharacter3.modelId);
	mCharacter3.transform.position = { 0.300f, 0.000f, -0.500f };
	mCharacter3.transform.rotation = Quaternion::CreateFromYawPitchRoll(0.000f, 0.000f, 0.000f);

	mFirework.Initialize();
	mFirework2.Initialize();
	mFirework3.Initialize();
	mCamera.SetPosition({ 2.494f, 3.103f, -5.326f });
	mCamera.SetDirection(Normalize({ 0.600f, -0.456f, 0.657f }));


	mCameraAnimation = AnimationBuilder()
		.AddPositionKey({ -2.494f, 3.103f, -5.326f }, 0.0f)
		.AddPositionKey({ -3.147f, 2.894f, -0.221f }, 4.0f, EaseType::EaseInOut)
		.Build();


	mCurrentScene = SceneState::Scene1;

	mSceneTime = 0.0f;
	mScene2Timer = 0.0f;
	mScene3Timer = 0.0f;
	mScene4Timer = 0.0f;
	mScene5Timer = 0.0f;
	mScene6Timer = 0.0f;
	mScene7Timer = 0.0f;


	mFireworkTriggered = false;
	mFireworkTriggered1 = false;
	mIsCharacter2Visible = true;
	mIsPaused = false;

	MeshPX mesh = MeshBuilder::CreateSkySpherePX(50, 50, 100.0f);
	mSkyboxMeshBuffer.Initialize<MeshPX>(mesh);

	std::filesystem::path skyboxShader = L"../../Assets/Shaders/DoTexture.fx";
	mSkyboxVS.Initialize<VertexPX>(skyboxShader);
	mSkyboxPS.Initialize(skyboxShader);

	mSkyboxTexture.Initialize("../../Assets/Images/skysphere/sky.jpg");

	mSkyboxCB.Initialize(sizeof(Matrix4));
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
	
	Vector3 currentPos = mCameraAnimation.GetTransform(mSceneTime).position;
	mCamera.SetPosition(currentPos);

	// look at character2
	mCamera.SetLookAt({ 0.400f, 0.000f, -3.000f });


	mCharater.transform.position = { 0.000f, 0.000f, 0.000f };
	mCharater.transform.rotation = Quaternion::CreateFromYawPitchRoll(0.000f, 0.00f, 0.000f);

	mCharacter2.transform.position = { 0.400f, 0.000f, -3.000f };
	mCharacter2.transform.rotation = Quaternion::CreateFromYawPitchRoll(0.0f, 3.30f, 0.000f);

	mCharacter3.transform.position = { 2.400f, 0.000f, -1.200f };
	mCharacter3.transform.rotation = Quaternion::CreateFromYawPitchRoll(0.0f, 1.7f, 0.000f);

	if (mSceneTime >= 0.1f && !mFireworkTriggered1)
	{
		PlayBGM();
		mCharacterAnimator.PlayAnimation(0, true);
		mCharacterAnimator2.PlayAnimation(0, true);
		mCharacterAnimator3.PlayAnimation(0, true);
		mFireworkTriggered1 = true;
	}
	//can you move like this
	if (mSceneTime >= 4.5f) {
		mCurrentScene = SceneState::Scene2;
		mScene2Timer = 0.0f;
		mCharacterAnimator2.PlayAnimation(1, true);


	}

}

void GameState::UpdateScene2(float deltaTime)
{
	mScene2Timer += deltaTime;

	//its on
	if (mScene2Timer >= 6.0f) {

		mCurrentScene = SceneState::Scene3;
		mScene3Timer = 0.0f;
		mCharacterAnimator.PlayAnimation(4, true);
		mCharacterAnimator2.PlayAnimation(1, true);


	}


}

void GameState::UpdateScene3(float deltaTime)
{
	//zoom in camera
	mCamera.SetPosition({ -0.234f, 1.357f, -1.846f });
	mCamera.SetDirection({ 0.259f, -0.197f, 0.946f });
	

	mScene3Timer += deltaTime;
	if (mScene3Timer >= 5.0f)
	{
		mCurrentScene = SceneState::Scene4;
		mScene4Timer = 0.0f;
		mCharacterAnimator.PlayAnimation(4, true);
		mCharacterAnimator2.PlayAnimation(1, true);
	}

}

void GameState::UpdateScene4(float deltaTime)
{
	mCamera.SetPosition({ -2.494f, 3.103f, -5.326f });
	mCamera.SetDirection(Normalize({ 0.600f, -0.456f, 0.657f }));
	mScene4Timer += deltaTime;
	
	if (mScene4Timer >= 2.0f)
	{
		mCurrentScene = SceneState::Scene5;
		mCharacterAnimator.PlayAnimation(4, true);
		mCharacterAnimator2.PlayAnimation(2, true);
	}
}

void GameState::UpdateScene5(float deltaTime)
{
	mCamera.SetPosition({ 0.848f, 1.592f, -0.182f });
	mCamera.SetDirection(Normalize({ -0.162f, -0.234f, -0.959f }));
	

	mScene5Timer += deltaTime;
	//single dance
	if (mScene5Timer >= 5.0f)
	{
		mCurrentScene = SceneState::Scene6;
		mScene6Timer = 0.0f;
		mCharacterAnimator3.PlayAnimation(1, true);
	}
}

void GameState::UpdateScene6(float deltaTime)
{
	mCamera.SetPosition({ 0.289f, 1.805f, 1.642f });
	mCamera.SetDirection(Normalize({ 0.363f, -0.198f, -0.910f }));

	mScene6Timer += deltaTime;

	if (mScene6Timer >= 5.0f)
	{
		mCurrentScene = SceneState::Scene7;
		mScene7Timer = 0.0f;

		mCharacterAnimator.PlayAnimation(5, true);
		mCharacterAnimator2.PlayAnimation(2, true);
		mCharacterAnimator3.PlayAnimation(2, true);


		mCharater.transform.position = { 0.000f, 0.000f, 0.000f };
		mCharater.transform.rotation = Quaternion::CreateFromYawPitchRoll(0.000f, 0.00f, 0.000f);

		mCharacter2.transform.position = { 3.000f, 0.000f, 0.000f };
		mCharacter2.transform.rotation = Quaternion::CreateFromYawPitchRoll(0.0f, 0.0f, 0.000f);

		mCharacter3.transform.position = { 1.500f, 0.000f, 3.00f };
		mCharacter3.transform.rotation = Quaternion::CreateFromYawPitchRoll(0.0f, 0.0f, 0.000f);

		StopBGM();
		PlayBGM2();


		mFirework.Start({ -5.0f, 0.3f, 8.0f });
		mFirework2.Start({ 0.0f, 0.3f, 10.0f });
		mFirework3.Start({ 5.0f, 0.3f, 8.0f });
	
	

		mCameraLastAnimation = AnimationBuilder()
			.AddPositionKey( Vector3{ 6.0f,      2, -3.0 }, 0.0f, EaseType::Linear)
			.AddPositionKey( Vector3{ 0.0f,    2,  -3.0 }, 2.0f, EaseType::Linear)
			.AddPositionKey( Vector3{ -6.0f,    2,  -3.0 }, 6.0f, EaseType::Linear)
			.AddPositionKey( Vector3{ 0.0f,      2, -3.0 }, 8.0f, EaseType::Linear)
			.Build();

	/*	mCameraLastAnimation = AnimationBuilder()
			.AddPositionKey({ -0.463f, 0.106f, -2.697f }, 0.0f)
			.AddPositionKey({ 1.500f, 0.800f, -3.200f }, 1.5f, EaseType::EaseInOut)
			.AddPositionKey({ 3.500f, 1.000f, -1.500f }, 3.0f, EaseType::EaseInOut)
			.AddPositionKey({ 3.200f, 1.100f,  1.500f }, 4.5f, EaseType::EaseInOut)
			.AddPositionKey({ 1.500f, 0.900f,  3.500f }, 6.0f, EaseType::EaseInOut)
			.Build();*/


	}
}

void GameState::UpdateScene7(float deltaTime)
{
	mScene7Timer += deltaTime;
	Vector3 currentPos = mCameraLastAnimation.GetTransform(mScene7Timer).position;



	mCamera.SetPosition(currentPos);
	mCamera.SetLookAt({ 1.500f, 1.000f, 3.00f });

	/*mCamera.SetPosition({ -0.463, 0.106, -2.697 });
	mCamera.SetDirection({0.390, 0.208, 0.897 });*/

	
	
}



void GameState::Render()
{
	mSkyboxVS.Bind();
	mSkyboxPS.Bind();

	mSkyboxTexture.BindPS(0);

	Matrix4 matWorld = Matrix4::Translation(mCamera.GetPosition());
	Matrix4 matView = mCamera.GetViewMatrix();
	Matrix4 matProj = mCamera.GetProjectionMatrix();

	Matrix4 wvp = Transpose(matWorld * matView * matProj);
	mSkyboxCB.Update(&wvp);
	mSkyboxCB.BindVS(0);

	mSkyboxMeshBuffer.Render();

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

	mSkyboxCB.Terminate();
	mSkyboxTexture.Terminate();
	mSkyboxPS.Terminate();
	mSkyboxVS.Terminate();
	mSkyboxMeshBuffer.Terminate();
}