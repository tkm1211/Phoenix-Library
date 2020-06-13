#pragma once

#include <memory>

#include "Phoenix/Types.h"
#include "Phoenix/FND/Base.h"
#include "Phoenix/Graphics/Device.h"
#include "Phoenix/Graphics/Surface.h"
#include "Phoenix/Graphics/Viewport.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// コンテキスト操作インターフェース
		//****************************************************************************
		class IContext : public FND::Base
		{
		public:
			// 生成
			static std::unique_ptr<IContext> Create();

			// 初期化
			virtual bool Initialize(IDevice* device) = 0;

			// 終了化
			virtual void Finalize() = 0;

			// レンダーターゲットビュークリア
			virtual void ClearRenderTargetView(IRenderTargetSurface* renderTargetSurface, const f32* color) = 0;
			
			// 深度ステンシルビュークリア
			virtual void ClearDepthStencilView(IDepthStencilSurface* depthStencilSurface, f32 depth, u8 stencil) = 0;
			
			// レンダーターゲット設定
			virtual void SetRenderTargets(u32 count, IRenderTargetSurface* renderTargetSurface[], IDepthStencilSurface* depthStencilSurface) = 0;
			
			// レンダーターゲット取得
			virtual void GetRenderTargets(u32 count, IRenderTargetSurface* renderTargetSurface[], IDepthStencilSurface* depthStencilSurface) = 0;
			
			// ビューポート設定
			virtual void SetViewports(u32 numViews, const Viewport* viewports) = 0;
			
			// ビューポート取得
			virtual void GetViewports(u32 count, Viewport* viewports[]) = 0;
		};
	} // namespace Graphics
} // namespace Phoenix