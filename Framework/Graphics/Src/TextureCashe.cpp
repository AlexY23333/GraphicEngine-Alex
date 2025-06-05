#include "Precompiled.h"
#include "RenderObject.h"
#include "TextureCashe.h"

using namespace SumEngine;
using namespace SumEngine::Graphics;

namespace
{
	std::unique_ptr<TextureCache> sInstance;
}

void TextureCache::StaticInitialize(const std::filesystem::path& root)
{
	ASSERT(sInstance == nullptr, "TextureCache: is already initialized");
	sInstance = std::make_unique<TextureCache>();
	sInstance->SetRootDirectory(root);
}

void TextureCache::StaticTerminate()
{
	sInstance.reset();
}

TextureCache* TextureCache::Get()
{
	ASSERT(sInstance != nullptr, "TextureCache: is not initialized yet");
	return sInstance.get();
}

TextureCache::~TextureCache()
{
	for (auto& [id, texture] : mInventory)
	{
		texture->Terminate();
	}
	mInventory.clear();
}

void TextureCache::SetRootDirectory(std::filesystem::path root)
{
	mRootDirectory = std::move(root);
}

TextureId TextureCache::LoadTexture(const std::filesystem::path& fileName, bool useRoot)
{
	TextureId textureId = std::filesystem::hash_value(fileName);
	// if is the same texture
	auto [iter, success] = mInventory.insert({ textureId, nullptr });
	if (success)
	{
		auto& texturePtr = iter->second;
		texturePtr = std::make_unique<Texture>();
		texturePtr->Initialize((useRoot) ? mRootDirectory / fileName : fileName);
	}
	return textureId;
}

const Texture* TextureCache::GetTexture(TextureId id) const
{
	auto iter = mInventory.find(id);
	if (iter != mInventory.end())
	{
		return iter->second.get();
	}
	return nullptr;
}

void TextureCache::BindVS(TextureId id, uint32_t slot) const
{
	auto iter = mInventory.find(id);
	if (iter != mInventory.end())
	{
		iter->second->BindVS(slot);
	}
}

void TextureCache::BindPS(TextureId id, uint32_t slot) const
{
	auto iter = mInventory.find(id);
	if (iter != mInventory.end())
	{
		iter->second->BindPS(slot);
	}
}
