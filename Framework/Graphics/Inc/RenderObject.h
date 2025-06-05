#pragma once
#include "MeshBuffer.h"
#include "Transform.h"
#include "TextureCashe.h"
#include "Material.h"
#include "ModelCache.h"

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
			void Initialize(const std::filesystem::path& modelFilePath);
			void Initialize(const Model& model);
			void Terminate();

			ModelId modelId;
			Transform transform;
			std ::vector<RenderObject> renderObjects;
	};

}