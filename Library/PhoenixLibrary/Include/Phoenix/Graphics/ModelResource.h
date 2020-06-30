#pragma once

#include "Phoenix/Graphics/Model.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// モデルリソースクラス
		//****************************************************************************
		class ModelResource : public IModelResource
		{
		public:
			// モデルデータ取得
			const ModelData& GetModelData() override { return data; }

			// メッシュ取得
			IMesh* GetMesh(s32 index) override { return meshes.at(index).get(); }

			// 初期化
			bool Initialize(Graphics::IDevice* device, const char* filename);

		private:
			std::vector<std::unique_ptr<Graphics::IMesh>> meshes;
			ModelData data;
		};

		//****************************************************************************
		// モデルリソースファクトリクラス
		//****************************************************************************
		class ModelResourceFactory : public IModelResourceFactory
		{
		public:
			ModelResourceFactory(Graphics::IDevice* device) : device(device) {}

			// リソース作成
			std::unique_ptr<OS::Resource> CreateResource(u32 type) override;

			// リソース読み込み
			bool LoadResource(OS::Resource* resource, OS::IFileStream* stream, const char* filename)  override;

		private:
			Graphics::IDevice* device = nullptr;
		};

	}	// namespace grp
}	// namespace ys