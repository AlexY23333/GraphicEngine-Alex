#pragma once
// Framework Headers
#include <Math/Inc/SumMath.h>
#include <Core/Inc/Core.h>
#include <Graphics/Inc/Graphics.h>

// Bullet headers
#include <Bullet/btBulletCollisionCommon.h>
#include <Bullet/btBulletDynamicsCommon.h>

// Helpers Functions
template<class T>
inline void SafeDelete(T*& ptr)
{
	if (ptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}

inline btVector3 TobtVector3(const SumEngine::Math::Vector3& v)
{
	return btVector3(v.x, v.y, v.z);
}

inline SumEngine::Math::Vector3 ToVector3(const btVector3& v)
{
	return {
		static_cast<float>(v.getX()),
		static_cast<float>(v.getY()),
		static_cast<float>(v.getZ())
	};
}

inline SumEngine::Color ToColor(const btVector3& c)
{
	return {
		static_cast<float>(c.getX()),
		static_cast<float>(c.getY()),
		static_cast<float>(c.getZ()),
		1.0f
	};
}


inline btQuaternion TobtQuaternion(const SumEngine::Math::Quaternion& q)
{
	return btQuaternion(q.x, q.y, q.z, q.w);
}

inline SumEngine::Math::Quaternion ToQuaternion(const btQuaternion& q)
{
	return {
		static_cast<float>(q.getX()),
		static_cast<float>(q.getY()),
		static_cast<float>(q.getZ()),
		static_cast<float>(q.getW())
	};
}

inline btTransform ConvertTobtTransform(const SumEngine::Graphics::Transform& transform)
{
	return btTransform(TobtQuaternion(transform.rotation), TobtVector3(transform.position));
}