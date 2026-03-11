#include "Precompiled.h"
#include "PhysicsDebugDraw.h"

using namespace SumEngine;
using namespace SumEngine::Physics;
using namespace SumEngine::Graphics;

void PhysicsDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	SimpleDraw::AddLine(ToVector3(from), ToVector3(to), ToColor(color));
}

void PhysicsDebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	SimpleDraw::AddLine(ToVector3(PointOnB), ToVector3(PointOnB + normalOnB), ToColor(color));
}

void PhysicsDebugDrawer::reportErrorWarning(const char* warningString)
{
	LOG("[PhysicsWarning] %s", warningString);
}

void PhysicsDebugDrawer::draw3dText(const btVector3& location, const char* textString)
{
	// Nothing here
}

void PhysicsDebugDrawer::setDebugMode(int debugMode)
{
	mDebugMode = debugMode;
}

int PhysicsDebugDrawer::getDebugMode() const
{
	return mDebugMode;
}
