#include "Precompiled.h"
#include "AnimationBuilder.h"

using namespace SumEngine;
using namespace SumEngine::Graphics;

namespace
{
	template<class T>
	inline void PushKey(Keyframes<T>& keyframes, const T& value, float t)
	{
		ASSERT(keyframes.empty() || keyframes.back().time <= t, "AnimationBuilder: cannot add keyframe back in time");
		keyframes.emplace_back(value, t);
	}
}

AnimationBuilder& AnimationBuilder::AddPositionKey(const Math::Vector3& pos, float time)
{
	PushKey(mWorkingCopy.mPositionKeys, pos, time);
	mWorkingCopy.mDuration = Math::Max(mWorkingCopy.mDuration, time);
	return *this;
}

AnimationBuilder& AnimationBuilder::AddRotationKey(const Math::Quaternion& rot, float time)
{
	PushKey(mWorkingCopy.mRotationKeys, rot, time);
	mWorkingCopy.mDuration = Math::Max(mWorkingCopy.mDuration, time);
	return *this;
}

AnimationBuilder& AnimationBuilder::AddScaleKey(const Math::Vector3& scale, float time)
{
	PushKey(mWorkingCopy.mScaleKeys, scale, time);
	mWorkingCopy.mDuration = Math::Max(mWorkingCopy.mDuration, time);
	return *this;
}

AnimationBuilder& SumEngine::Graphics::AnimationBuilder::AddEventKey(AnimationCallback cb, float time)
{
	PushKey(mWorkingCopy.mEventKeys, cb, time);
	mWorkingCopy.mDuration = Math::Max(mWorkingCopy.mDuration, time);
	return *this;
}

AnimationBuilder& SumEngine::Graphics::AnimationBuilder::AddParameterEventKey(AnimationParameterCallback cb, float time)
{
	PushKey(mWorkingCopy.mEventParameterKeys, cb, time);
	mWorkingCopy.mDuration = Math::Max(mWorkingCopy.mDuration, time);
	return *this;
}

Animation AnimationBuilder::Build()
{
	ASSERT(!mWorkingCopy.mPositionKeys.empty()
		|| !mWorkingCopy.mRotationKeys.empty()
		|| !mWorkingCopy.mScaleKeys.empty()
		|| !mWorkingCopy.mEventKeys.empty()
		|| !mWorkingCopy.mEventParameterKeys.empty(),
		"AnimationBuilder: no animation keys are present");
	return std::move(mWorkingCopy);
}
