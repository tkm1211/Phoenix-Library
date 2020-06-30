#pragma once

#include <d3d11.h>
#include "Phoenix/Graphics/Surface.h"


namespace Phoenix
{
	namespace Graphics
	{
		class RenderTargetSurfaceDX11 final : public IRenderTargetSurface
		{
		private:
			ID3D11RenderTargetView* renderTargetView = nullptr;
			std::unique_ptr<ITexture> texture = nullptr;

		public:
			RenderTargetSurfaceDX11() {}
			~RenderTargetSurfaceDX11() override { Finalize(); }

		public:
			// 初期化
			bool Initialize(ID3D11Device* d3dDevice, ID3D11Texture2D* d3dTexture2D, ID3D11ShaderResourceView* d3dShaderResourceView);
			
			// 初期化
			bool Initialize(IDevice* device, const RenderTargetSurfaceDesc& desc) override;
			
			// 終了化
			void Finalize() override;

			// テクスチャ取得
			ITexture* GetTexture() override { return texture.get(); }

			// D3Dレンダーターゲットビュー設定
			void SetD3DRenderTargetView(ID3D11RenderTargetView* d3dRenderTargetView) { renderTargetView = d3dRenderTargetView; }

			// D3Dレンダーターゲットビュー取得
			ID3D11RenderTargetView* GetD3DRenderTargetView() const { return renderTargetView; }
		};
	} // namespace Graphics
} // namespace Phoenix