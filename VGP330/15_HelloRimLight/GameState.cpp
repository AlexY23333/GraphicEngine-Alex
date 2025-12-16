#include "GameState.h"
using namespace SumEngine;
using namespace SumEngine::Math;
using namespace SumEngine::Graphics;
using namespace SumEngine::Core;
using namespace SumEngine::Input;

void GameState::Initialize()
{
    // Camera setup
    mCamera.SetPosition({ 0.0f, 1.0f, -5.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

    // Lighting setup
    mDirectionalLight.direction = Normalize({ 1.0f, -1.0f, 1.0f });
    mDirectionalLight.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
    mDirectionalLight.diffuse = { 0.7f, 0.7f, 0.7f, 1.0f };
    mDirectionalLight.specular = { 0.9f, 0.9f, 0.9f, 1.0f };

    // Standard effect setup
    std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
    mStandardEffect.Initialize(shaderFile);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);
    mStandardEffect.SetShadowMap(mShadowEffect.GetDepthMap());
    mStandardEffect.SetLightCamera(mShadowEffect.GetLightCamera());

    // Shadow effect setup
    mShadowEffect.Initialize();
    mShadowEffect.SetDirectionalLight(mDirectionalLight);

    // Outline effect setup
    mOutlineEffect.Initialize();
    mOutlineEffect.SetCamera(mCamera);

    Mesh ground = MeshBuilder::CreatePlane(10.0f, 10.0f, 1);
    mGround.meshBuffer.Initialize(ground);
    mGround.diffuseId = TextureCache::Get()->LoadTexture("misc/concrete.jpg");

    // Character setup
    mCharater.Initialize("../../Assets/Models/Amy/Amy.model");
    mCharater.transform.position = { -2.5f, 0.0f, 0.0f };

    mCharaterOutLine.Initialize("../../Assets/Models/Amy/Amy.model");
    mCharaterOutLine.transform.position = { -2.5f, 0.0f, 0.0f };
    mCharaterOutLine.transform.scale *= 1.01f;

    // Sphere setup
    Mesh sphere = MeshBuilder::CreateSphere(500, 20, 1.5f);
    mSphere.meshBuffer.Initialize(sphere);
    mSphere.transform.position.x = 0.0f;
    mSphere.transform.position.y = 1.5f;

    // Outline sphere
    mSphereOutline.meshBuffer.Initialize(sphere);
    mSphereOutline.transform.position.x = 0.0f;
    mSphereOutline.transform.position.y = 1.5f;
    mSphereOutline.transform.scale = { 1.05f, 1.05f, 1.05f };

    // Cube setup
    Mesh cube = MeshBuilder::CreateCube(1.0f);
    mCube.meshBuffer.Initialize(cube);
    mCube.transform.position.x = 2.0f;
    mCube.transform.position.y = 0.5f;

    // Cube outline
    mCubeOutline.meshBuffer.Initialize(cube);
    mCubeOutline.transform.position.x = 2.0f;
    mCubeOutline.transform.position.y = 0.5f;
    mCubeOutline.transform.scale = { 1.05f, 1.05f, 1.05f };
}

void GameState::Terminate()
{
    mGround.Terminate();

    mCube.Terminate();
    mCubeOutline.Terminate();

    mSphere.Terminate();
    mSphereOutline.Terminate();

    mCharater.Terminate();
    mCharaterOutLine.Terminate();

    mStandardEffect.Terminate();
    mShadowEffect.Terminate();
    mOutlineEffect.Terminate();
}

void GameState::Update(float deltaTime)
{
    UpdateCamera(deltaTime);
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

void GameState::Render()
{
    // Shadow pass
    mShadowEffect.Begin();
    mShadowEffect.Render(mSphere);
    mShadowEffect.Render(mCube);
    mShadowEffect.Render(mCharater);
    mShadowEffect.End();

    // Outline pass - render slightly larger version first
    mOutlineEffect.Begin();
    mOutlineEffect.Render(mSphereOutline);
    mOutlineEffect.Render(mCubeOutline);
    mOutlineEffect.Render(mCharaterOutLine);
    mOutlineEffect.End();

    // Standard pass - render normal version
    mStandardEffect.Begin();
    mStandardEffect.Render(mSphere);
    mStandardEffect.Render(mCube);
    mStandardEffect.Render(mGround);
    mStandardEffect.Render(mCharater);
    mStandardEffect.End();
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

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

    if (ImGui::CollapsingHeader("Outline Settings", ImGuiTreeNodeFlags_DefaultOpen))
    {
        static float outlineWidth = 0.02f;
        static float outlineColor[3] = { 0.0f, 0.0f, 0.0f };

        if (ImGui::DragFloat("Outline Width", &outlineWidth, 0.001f, 0.0f, 1.0f))
        {
            mOutlineEffect.SetOutlineWidth(outlineWidth);
        }

        if (ImGui::ColorEdit3("Outline Color", outlineColor))
        {
            mOutlineEffect.SetOutlineColor({ outlineColor[0], outlineColor[1], outlineColor[2] });
        }
    }

    mStandardEffect.DebugUI();
    mShadowEffect.DebugUI();

    ImGui::End();
}