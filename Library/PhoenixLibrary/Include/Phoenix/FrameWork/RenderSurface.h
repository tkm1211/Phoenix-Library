#pragma once

#include "Phoenix/FND/NonCopyable.h"
#include "Phoenix/Graphics/Device.h"
#include "Phoenix/Graphics/Context.h"
#include "Phoenix/Graphics/Surface.h"
#include "Phoenix/Math/Color.h"


namespace Phoenix
{
	namespace FrameWork
	{
		//****************************************************************************
		// レンダーサーフェイス
		//****************************************************************************
		class RenderSurface final : public FND::NonCopyable
		{
		private:
			std::unique_ptr<Graphics::IRenderTargetSurface>	rts;
			std::unique_ptr<Graphics::IDepthStencilSurface>	dss;
			Math::Color backgroundColor = Math::Color::Gray;
			u32 width = 0;
			u32 height = 0;

		public:
			// 初期化
			bool Initialize(Graphics::IDevice* device, u32 width, u32 height);

			// 終了化
			void Finalize();

			// 幅取得
			u32 GetWidth() const { return width; }

			// 高さ取得
			u32 GetHeight() const { return height; }

			// 背景色を設定
			void SetBackgroundColor(const Math::Color& color) { backgroundColor = color; }

			// 背景色を取得
			const Math::Color& GetBackgroundColor() const { return backgroundColor; }

			// リサイズ
			void Resize(Graphics::IDevice* device, u32 width, u32 height);

			// レンダーターゲットサーフェイス取得
			Graphics::IRenderTargetSurface* GetRenderTargetSurface() { return rts.get(); }

			// デプスステンシルフェイス取得
			Graphics::IDepthStencilSurface* GetDepthStencilSurface() { return dss.get(); }

			// テクスチャ取得
			Graphics::ITexture* GetTexture() const { return rts->GetTexture(); }
		};
	} // namespace FrameWork
} // namespace Phoenix