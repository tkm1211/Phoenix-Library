#pragma once

#include <d3d11.h>
#include <vector>
#include "Phoenix/Graphics/Device.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11版デバイス操作クラス
		//****************************************************************************
		class DeviceDX11 final : public IDevice
		{
		private:
			ID3D11Device*			device = nullptr;
			ID3D11DeviceContext*	immediateContext = nullptr;
			IDXGIFactory*			dxgiFactory = nullptr;

		public:
			DeviceDX11() {}
			~DeviceDX11() override { Finalize(); }

		public:
			// 初期化
			bool Initialize() override;

			// 終了化
			void Finalize() override;

			// D3Dデバイス取得
			ID3D11Device* GetD3DDevice() const { return device; }

			// D3Dデバイスコンテキスト取得
			ID3D11DeviceContext* GetD3DContext() const { return immediateContext; }

			// DXGIファクトリ取得
			IDXGIFactory* GetDXGIFactory() const { return dxgiFactory; }
		};
	} // namespace Graphics
} // namespace Phoenix