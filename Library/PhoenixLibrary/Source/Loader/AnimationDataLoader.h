#pragma once

#include <fbxsdk.h>
#include "Phoenix/Graphics/Animation.h"


namespace Phoenix
{
	namespace Loader
	{
		//****************************************************************************
		// アニメーションデータローダークラス
		//****************************************************************************
		class AnimationDataLoader
		{
		public:
			// アニメーションデータ読み込み
			bool Load(FbxScene* fbxScene, Graphics::AnimationData& data);
		};

	} // namespace Loader
} // namespace Phoenix