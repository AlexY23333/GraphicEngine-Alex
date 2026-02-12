#include "Precompiled.h"
#include "AnimationBuilder.h"

using namespace SumEngine;
using namespace SumEngine::Graphics;

namespace
{
	template<class T>
	inline void PushKey(Keyframes<T>& keyframes, const T& value, float t, EaseType ease)
	{
		ASSERT(keyframes.empty() || keyframes.back().time <= t, "AnimationBuilder: cannot add keyframe back in time");
		keyframes.emplace_back(value, t, ease);
	}
}
AnimationBuilder& AnimationBuilder::AddPositionKey(const Math::Vector3& pos, float time, EaseType ease)
{
	PushKey(mWorkingCopy.mPositionKeys, pos, time, ease);
	mWorkingCopy.mDuration = Math::Max(mWorkingCopy.mDuration, time);
	return *this;
}

AnimationBuilder& AnimationBuilder::AddRotationKey(const Math::Quaternion& rot, float time, EaseType ease)
{
	PushKey(mWorkingCopy.mRotationKeys, rot, time, ease);
	mWorkingCopy.mDuration = Math::Max(mWorkingCopy.mDuration, time);
	return *this;
}

AnimationBuilder& AnimationBuilder::AddScaleKey(const Math::Vector3& scale, float time, EaseType ease)
{
	PushKey(mWorkingCopy.mScaleKeys, scale, time, ease);
	mWorkingCopy.mDuration = Math::Max(mWorkingCopy.mDuration, time);
	return *this;
}

Animation AnimationBuilder::Build()
{
	ASSERT(!mWorkingCopy.mPositionKeys.empty()
		|| !mWorkingCopy.mRotationKeys.empty()
		|| !mWorkingCopy.mScaleKeys.empty(),
		"animationBuilder: no animation keys are present");
	return std::move(mWorkingCopy);
}