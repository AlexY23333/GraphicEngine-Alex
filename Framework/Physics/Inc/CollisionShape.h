#pragma once

namespace SumEngine::Physics
{
	class CollisionShape final
	{
	public:
		CollisionShape() = default;
		~CollisionShape();

		void InitializeEmpty();
		void InitializeSphere(float radius);
		void InitializeCapsule(float radius, float height);
		void InitializeBox(const SumEngine::Math::Vector3& halfExtents);
		void InitializeHull(const SumEngine::Math::Vector3& halfExtents, const SumEngine::Math::Vector3& origin);

		void Terminate();

	private:
		friend class RigidBody;
		btCollisionShape* mCollisionShape = nullptr;
	};
}