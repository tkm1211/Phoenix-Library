#pragma once

#include <memory>
#include "Phoenix/Types.h"
#include "Phoenix/FND/Base.h"
#include "Phoenix/Graphics/Device.h"
#include "Phoenix/Graphics/Surface.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// スワップチェーン設定記述
		//****************************************************************************
		struct SwapChainDesc
		{
			u32 width;
			u32 heigth;
			void* windowHandle;
		};

		//****************************************************************************
		// スワップチェーン操作インターフェース
		//****************************************************************************
		class ISwapChain : public FND::Base
		{
		public:
			// 生成
			static std::unique_ptr<ISwapChain> Create();

			// 初期化
			virtual bool Initialize(IDevice* device, const SwapChainDesc& desc) = 0;

			// 終了化
			virtual void Finalize() = 0;

			// レンダリングされたイメージを表示
			virtual void Present(int syncInterval) = 0;

			// レンダーターゲットサーフェイス操作オブジェクト取得
			virtual IRenderTargetSurface* GetRenderTargerSurface() = 0;

			// 深度ステンシルサーフェイス操作オブジェクト取得
			virtual IDepthStencilSurface* GetDepthStencilSurface() = 0;
		};
	} // namespace Graphics
} // namespace Phoenix