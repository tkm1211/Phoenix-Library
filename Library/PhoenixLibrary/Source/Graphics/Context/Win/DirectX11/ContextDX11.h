#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d11.h>
#include <assert.h>

#include "Phoenix/Graphics/Context.h"
#include "Phoenix/Graphics/Surface.h"
#include "Phoenix/Graphics/Viewport.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11版コンテキスト操作オブジェクト
		//****************************************************************************
		class ContextDX11 final : public IContext
		{
		private:
			ID3D11DeviceContext* deviceContext = nullptr;

		public:
			ContextDX11() {}
			~ContextDX11() override { Finalize(); }

		public:
			// 初期化
			bool Initialize(IDevice* device) override;

			// 終了化
			void Finalize() override;

			// レンダーターゲットビュークリア
			void ClearRenderTargetView(IRenderTargetSurface* renderTargetSurface, const f32* color) override;
			
			// 深度ステンシルビュークリア
			void ClearDepthStencilView(IDepthStencilSurface* depthStencilSurface, f32 depth, u8 stencil) override;
			
			// レンダーターゲット設定
			void SetRenderTargets(u32 count, IRenderTargetSurface* renderTargetSurface[], IDepthStencilSurface* depthStencilSurface) override;
			
			// レンダーターゲット取得
			void GetRenderTargets(u32 count, IRenderTargetSurface* renderTargetSurface[], IDepthStencilSurface* depthStencilSurface) override;
			
			// ビューポート設定
			void SetViewports(u32 count, const Viewport viewports[]) override;
			
			// ビューポート取得
			void GetViewports(u32 count, Viewport* viewports[]) override;
		};
	} // namespace Graphics
} // namespace Phoenix