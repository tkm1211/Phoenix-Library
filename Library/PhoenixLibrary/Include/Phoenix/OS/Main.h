#pragma once

#include <memory>
#include "Phoenix/Types.h"
#include "Phoenix/FND/Base.h"


namespace Phoenix
{
	namespace OS
	{
		//============================================================================
		// メインループ操作インターフェース
		//----------------------------------------------------------------------------
		class IMain : public FND::Base
		{
		public:
			// 生成
			static std::unique_ptr<IMain> Create();

			// 初期化
			virtual bool Initialize(const wchar_t* name, s32 width, s32 height, uintPtr instance) = 0;
			
			// 終了化
			virtual void Finalize() = 0;
			
			// ループ処理
			virtual void Run() = 0;
		};
	} // namespace OS
} // namespace Phoenix