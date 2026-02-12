#pragma once
#include "MeshBuffer.h"
#include "Transform.h"
#include "TextureCashe.h"
#include "Material.h"
#include "ModelCache.h"
#include "Animator.h"

namespace SumEngine::Graphics
{
	class RenderObject
	{
		public:
			void Terminate();

			Transform transform;
			MeshBuffer meshBuffer;
			Material material;

			TextureId diffuseId;
			TextureId normalId;
			TextureId specularId;
			TextureId bumpId;

	};

	class RenderGroup
	{
		public:
			void Initialize(const std::filesystem::path& modelFilePath, const Animator* anim = nullptr);
			void Initialize(const Model& model, const Animator* anim = nullptr);
			void Terminate();

			ModelId modelId;
			Transform transform;
			const Skeleton* skeleton = nullptr;
			const Animator* animator = nullptr;
			std ::vector<RenderObject> renderObjects;
	};

}