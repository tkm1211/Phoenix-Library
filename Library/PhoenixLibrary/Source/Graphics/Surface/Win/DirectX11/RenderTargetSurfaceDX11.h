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
			ID3D11ShaderResourceView* shaderResourceView = nullptr;
			ID3D11Texture2D* texture2D = nullptr;

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

			// D3Dレンダーターゲットビュー設定
			void SetD3DRenderTargetView(ID3D11RenderTargetView* d3dRenderTargetView) { renderTargetView = d3dRenderTargetView; }

			// D3Dレンダーターゲットビュー取得
			ID3D11RenderTargetView* GetD3DRenderTargetView() const { return renderTargetView; }

			// D3Dシェーダーリソースビュー取得
			ID3D11ShaderResourceView* GetD3DShaderResourceView() const { return shaderResourceView; }

			// D3Dテクスチャ2D取得
			ID3D11Texture2D* GetD3DTexture2D() const { return texture2D; }
		};
	} // namespace Graphics
} // namespace Phoenix