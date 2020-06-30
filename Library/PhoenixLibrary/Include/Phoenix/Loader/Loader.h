#pragma once

#include <memory>
#include "Phoenix/Types.h"
#include "Phoenix/FND/Base.h"
#include "Phoenix/Graphics/Model.h"
#include "Phoenix/Graphics/Animation.h"


namespace Phoenix
{
	namespace Loader
	{
		//****************************************************************************
		// FBX読み込み操作インターフェース
		//****************************************************************************
		class ILoader : public FND::Base
		{
		public:
			// 生成
			static std::unique_ptr<ILoader> Create();

			// 初期化
			virtual bool Initialize(const char* fileName) = 0;

			// 終了化
			virtual void Finalize() = 0;

			// モデルデータ読み込み
			virtual bool Load(Graphics::ModelData& data) = 0;

			// アニメーションデータ読み込み
			virtual bool Load(Graphics::AnimationData& data) = 0;
		};

	} // namespace Loader
} // namespace Phoenix