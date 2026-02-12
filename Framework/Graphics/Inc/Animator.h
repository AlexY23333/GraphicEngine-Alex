#pragma once

#include "ModelCache.h"

namespace SumEngine::Graphics
{
	struct Bone;

	class Animator
	{
	public:
		void Initialize(ModelId id);
		void PlayAnimation(int clipIndex, bool looping);
		void Update(float deltaTime);

		bool IsFinished() const;
		size_t GetAnimationCount() const;
		Math::Matrix4 GetToParentTransform(const Bone* bone) const;

	private:
		ModelId mModelId = 0;
		int mClipIndex = -1;
		float mAnimationTick = 0.0f;
		bool mIsLooping = false;
	};
}