#pragma once

#include <d3d11.h>
#include "Phoenix/Graphics/Surface.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11版深度ステンシルサーフェイス操作オブジェクト
		//****************************************************************************
		class DepthStencilSurfaceDX11 final : public IDepthStencilSurface
		{
		private:
			ID3D11DepthStencilView* depthStencilView = nullptr;
			ID3D11Texture2D* texture2D = nullptr;

		public:
			DepthStencilSurfaceDX11() {}
			~DepthStencilSurfaceDX11() override { Finalize(); }

		public:
			// 初期化
			bool Initialize(IDevice* device, const DepthStencilSurfaceDesc& desc) override;

			// 終了化
			void Finalize() override;

			// D3D深度ステンシルビュー設定
			void SetDepthStencilView(ID3D11DepthStencilView* d3dDepthStencilView) { depthStencilView = d3dDepthStencilView; }
			
			// D3D深度ステンシルビュー取得
			ID3D11DepthStencilView* GetD3DDepthStencilView() const { return depthStencilView; }
			
			// D3Dテクスチャ2D取得
			ID3D11Texture2D* GetD3DTexture2D() const { return texture2D; }
		};
	} // namespace Graphics
} // namespace Phoenix