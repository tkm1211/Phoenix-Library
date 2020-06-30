#pragma once

#include <d3d11.h>
#include "Phoenix/Graphics/RenderState.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11版深度ステンシルステート操作オブジェクト
		//****************************************************************************
		class DepthStencilDX11 final : public IDepthStencil
		{
		private:
			ID3D11DepthStencilState* depthStencilState = nullptr;

		public:
			// 初期化
			bool Initialize(IDevice* device, DepthState state) override;

			// 終了化
			void Finalize() override;

			// D3Dブレンドステート取得
			ID3D11DepthStencilState* GetD3DDepthStencilState() const { return depthStencilState; }
		};
	} // namespace Graphics
} // namespace Phoenix