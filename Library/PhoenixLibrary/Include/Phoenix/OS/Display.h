#pragma once

#include <memory>
#include "Phoenix/Types.h"
#include "Phoenix/FND/Base.h"


namespace Phoenix
{
	namespace OS
	{
		// ウインドウハンドル
		typedef uintPtr WindowHandle;

		//****************************************************************************
		// ディスプレイ操作インターフェース
		//****************************************************************************
		class IDisplay : public FND::Base
		{
		public:
			// 生成
			static std::unique_ptr<IDisplay> Create();

			// 初期化
			virtual bool Initialize(const wchar_t* name, s32 width, s32 height, uintPtr instance) = 0;
			
			// 終了化
			virtual void Finalize() = 0;

			// 幅取得
			virtual s32 GetWidth() = 0;

			// 高さ取得
			virtual s32 GetHeight() = 0;

			// ウインドウハンドル取得
			virtual WindowHandle GetHandle() = 0;
		};
	} // namespace OS
} // namespace Phoenix