#include "Precompiled.h"
#include "PhysicsWorld.h"
#include "PhysicsObject.h"

using namespace SumEngine;
using namespace SumEngine::Physics;

namespace
{
	std::unique_ptr<PhysicsWorld> sPhysicsWorld;
}

void PhysicsWorld::StaticInitialize(const Settings& settings)
{
    ASSERT(sPhysicsWorld == nullptr, "PhysicsWorld: is already initialized");
    sPhysicsWorld = std::make_unique<PhysicsWorld>();
    sPhysicsWorld->Initialize(settings);
}

void PhysicsWorld::StaticTerminate()
{
    if (sPhysicsWorld != nullptr)
    {
        sPhysicsWorld->Terminate();
        sPhysicsWorld.reset();
    }
}

PhysicsWorld* PhysicsWorld::Get()
{
    ASSERT(sPhysicsWorld != nullptr, "PhysicsWorld: must be initialized");
    return sPhysicsWorld.get();
}

PhysicsWorld::~PhysicsWorld()
{
    ASSERT(mDynamicsWorld == nullptr, "PhysicsWorld: must be terminated");
}

void PhysicsWorld::Initialize(const Settings& settings)
{
    mSettings = settings;

    mInterface = new btDbvtBroadphase();
    mSolver = new btSequentialImpulseConstraintSolver();
    mCollisionConfiguration = new btDefaultCollisionConfiguration();
    mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
    mDynamicsWorld = new btDiscreteDynamicsWorld(mDispatcher, mInterface, mSolver, mCollisionConfiguration);

    mDynamicsWorld->setGravity(TobtVector3(settings.gravity));
    mDynamicsWorld->setDebugDrawer(&mPhysicsDebugDrawer);
}

void PhysicsWorld::Terminate()
{
    SafeDelete(mDynamicsWorld);
    SafeDelete(mDispatcher);
    SafeDelete(mCollisionConfiguration);
    SafeDelete(mSolver);
    SafeDelete(mInterface);
}

void PhysicsWorld::Update(float deltaTime)
{
    mDynamicsWorld->stepSimulation(deltaTime, mSettings.simulationSteps, mSettings.fixedTimeStep);

    for (PhysicsObject* po : mPhysicsObjects)
    {
        po->SyncWithGraphics();
    }
}

void PhysicsWorld::DebugUI()
{
    if (ImGui::CollapsingHeader("Physics", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::DragFloat3("Gravity", &mSettings.gravity.x, 0.1f))
        {
            mDynamicsWorld->setGravity(TobtVector3(mSettings.gravity));
        }
        ImGui::Checkbox("DebugDraw", &mDebugDraw);
        if (mDebugDraw)
        {
            ImGui::Indent();
                int debugMode = mPhysicsDebugDrawer.getDebugMode();
                bool isEnabled = (debugMode & btIDebugDraw::DBG_DrawWireframe);
                if (ImGui::Checkbox("WireFrame", &isEnabled))
                {
                    debugMode = (isEnabled) ? debugMode | btIDebugDraw::DBG_DrawWireframe : debugMode & ~btIDebugDraw::DBG_DrawWireframe;
                }
                isEnabled = (debugMode & btIDebugDraw::DBG_DrawAabb);
                if (ImGui::Checkbox("DrawAABB", &isEnabled))
                {
                    debugMode = (isEnabled) ? debugMode | btIDebugDraw::DBG_DrawAabb : debugMode & ~btIDebugDraw::DBG_DrawAabb;
                }            
                mPhysicsDebugDrawer.setDebugMode(debugMode);
                mDynamicsWorld->debugDrawWorld();
            ImGui::Unindent();
        }
    }
}

void PhysicsWorld::Register(PhysicsObject* physicsObject)
{
    auto iter = std::find(mPhysicsObjects.begin(), mPhysicsObjects.end(), physicsObject);
    if (iter == mPhysicsObjects.end())
    {
        mPhysicsObjects.push_back(physicsObject);

        if (physicsObject->GetRigidBody() != nullptr) 
        {
            mDynamicsWorld->addRigidBody(physicsObject->GetRigidBody());
        }
    }
}

void PhysicsWorld::Unregister(PhysicsObject* physicsObject)
{
    auto iter = std::find(mPhysicsObjects.begin(), mPhysicsObjects.end(), physicsObject);
    if (iter != mPhysicsObjects.end())
    {

        if (physicsObject->GetRigidBody() != nullptr)
        {
            mDynamicsWorld->removeRigidBody(physicsObject->GetRigidBody());
        }

        mPhysicsObjects.erase(iter);

    }
}
