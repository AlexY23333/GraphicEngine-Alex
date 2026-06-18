#pragma once

#include "RenderObjectComponent.h"

namespace SumEngine
{
	class MeshComponent final : public RenderObjectComponent
	{
	public:
		SET_TYPE_ID(ComponentId::Mesh);

		void Deserialize(const rapidjson::Value& value) override;
		Graphics::ModelId GetModelId() const override;
		const Graphics::Model& GetModel() const;

	private:
		Graphics::Model mModel;
		Graphics::ModelId mModelId = 0;
		const Graphics::Model* mCachedModel = nullptr;
	};
}
