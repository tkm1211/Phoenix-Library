#pragma once

#include <memory>
#include "Phoenix/Types.h"
#include "Phoenix/FND/Base.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// デバイス操作インターフェース
		//****************************************************************************
		class IDevice : public FND::Base
		{
		public:
			// 生成
			static std::unique_ptr<IDevice> Create();

			// 初期化
			virtual bool Initialize() = 0;

			// 終了化
			virtual void Finalize() = 0;
		};
	} // namespace Graphics
} // namespace Phoenix