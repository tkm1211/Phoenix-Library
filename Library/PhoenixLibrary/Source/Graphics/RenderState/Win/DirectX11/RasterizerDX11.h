#pragma once

#include <d3d11.h>
#include "Phoenix/Graphics/RenderState.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11版ラスタライザステート操作オブジェクト
		//****************************************************************************
		class RasterizerDX11 final : public IRasterizer
		{
		private:
			ID3D11RasterizerState* rasterizerState = nullptr;

		public:
			// 初期化
			bool Initialize(IDevice* device, RasterizerState state) override;

			// 終了化
			void Finalize() override;

			// D3Dラスタライザステート取得
			ID3D11RasterizerState* GetD3DRasterizerState() const { return rasterizerState; }
		};
	} // namespace Graphics
} // namespace Phoenix