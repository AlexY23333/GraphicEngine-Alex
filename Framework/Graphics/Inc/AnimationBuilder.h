#pragma once
#include "Common.h"
#include "Animation.h"

namespace SumEngine::Graphics
{
	class AnimationBuilder
	{
	public:
		AnimationBuilder& AddPositionKey(const Math::Vector3& pos, float time, EaseType ease = EaseType::Linear);
		AnimationBuilder& AddRotationKey(const Math::Quaternion& rot, float time, EaseType ease = EaseType::Linear);
		AnimationBuilder& AddScaleKey(const Math::Vector3& scale, float time, EaseType ease = EaseType::Linear);

		[[nodiscard]] Animation Build();

	private:
		Animation mWorkingCopy;
	};
}