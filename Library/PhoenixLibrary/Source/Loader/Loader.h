#pragma once

//#include <fbxsdk.h>
#include "fbxsdk.h"
#include "Phoenix/Loader/Loader.h"


namespace Phoenix
{
	namespace Loader
	{
		//****************************************************************************
		// FBX読み込み操作オブジェクトクラス
		//****************************************************************************
		class Loader final : public ILoader
		{
		private:
			FbxManager* fbxManager = nullptr;
			FbxScene* fbxScene = nullptr;
			std::string filename;

		public:
			Loader() {}
			~Loader() override { Finalize(); }

		public:
			// 初期化
			bool Initialize(const char* fileName) override;

			// 終了化
			void Finalize() override;

			// モデルデータ読み込み
			bool Load(Graphics::ModelData& data) override;

			// アニメーションデータ読み込み
			bool Load(Graphics::AnimationData& data) override;

		private:
			// 三角形化
			void Triangulate();
		};

	} // namespace Loader
} // namespace Phoenix