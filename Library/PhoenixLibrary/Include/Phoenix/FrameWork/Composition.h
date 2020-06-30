#pragma once

#include "Phoenix/FND/Base.h"


namespace Phoenix
{
	namespace FrameWork
	{
		// 先行宣言
		class Container;

		//****************************************************************************
		// コンポジション
		//****************************************************************************
		class Composition : public FND::Base
		{
		public:
			// 名前取得
			virtual const char* GetName() = 0;

			// コンストラクション
			virtual void Construct(Container* container) {}

			// 初期化
			virtual bool Initialize() { return true; }

			// 終了化
			virtual void Finalize() {}
		};

	} // namespace FrameWork
} // namespace Phoenix