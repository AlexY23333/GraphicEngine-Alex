#pragma once

#include "Material.h"
#include "MeshBuffer.h"
#include "ModelCache.h"
#include "TextureCache.h"
#include "Transform.h"
#include "Animator.h"

namespace SumEngine::Graphics
{
	struct Model;

	class RenderObject
	{
	public:
		void Terminate();

		Transform transform;
		MeshBuffer meshBuffer;

		Material material;
		TextureId diffuseMapId;
		TextureId normalMapId;
		TextureId specMapId;
		TextureId bumpMapId;
	};

	class RenderGroup
	{
	public:
		void Initialize(const std::filesystem::path& modelFilePath, const Animator* animator = nullptr);
		void Initialize(const Model& model, const Animator* animator = nullptr);
		void Terminate();

		ModelId modelId;
		Transform transform;
		const Skeleton* skeleton = nullptr;
		const Animator* animator = nullptr;

		std::vector<RenderObject> renderObjects;
	};
}