#include "pch.h"
#include "DepthStencilSurfaceDX11.h"
#include "Phoenix/FND/Assert.h"
#include "Phoenix/FND/Logger.h"
#include "Phoenix/FND/Util.h"
#include "Device/Win/DirectX11/DeviceDX11.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11版深度ステンシルサーフェイス操作オブジェクト
		//****************************************************************************
		// 生成
		std::unique_ptr<IDepthStencilSurface> IDepthStencilSurface::Create()
		{
			return std::make_unique<DepthStencilSurfaceDX11>();
		}

		// 初期化
		bool DepthStencilSurfaceDX11::Initialize(IDevice* device, const DepthStencilSurfaceDesc& desc)
		{
			HRESULT hr;
			ID3D11Device* d3dDevice = static_cast<DeviceDX11*>(device)->GetD3DDevice();

			D3D11_TEXTURE2D_DESC d3dDesc = {};
			d3dDesc.Width = desc.width;
			d3dDesc.Height = desc.height;
			d3dDesc.MipLevels = 1;
			d3dDesc.ArraySize = 1;
			d3dDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			d3dDesc.SampleDesc.Count = 1;
			d3dDesc.SampleDesc.Quality = 0;
			d3dDesc.Usage = D3D11_USAGE_DEFAULT;
			d3dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			d3dDesc.CPUAccessFlags = 0;
			d3dDesc.MiscFlags = 0;

			ID3D11Texture2D* d3dTexture = nullptr;
			hr = d3dDevice->CreateTexture2D(&d3dDesc, nullptr, &d3dTexture);
			if (FAILED(hr))
			{
				PHOENIX_LOG_GRP_ERROR("ID3D11Device::CreateTexture2D() : Failed!!\n");
				return false;
			}

			texture2D = d3dTexture;

			D3D11_DEPTH_STENCIL_VIEW_DESC  d3dDSVDesc;
			ZeroMemory(&d3dDSVDesc, sizeof(d3dDSVDesc));
			d3dDSVDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			d3dDSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

			hr = d3dDevice->CreateDepthStencilView(texture2D, &d3dDSVDesc, &depthStencilView);
			if (FAILED(hr))
			{
				PHOENIX_LOG_GRP_ERROR("ID3D11Device::CreateDepthStencilView() : Failed!!\n");
				return false;
			}

			return true;
		}

		// 終了化
		void DepthStencilSurfaceDX11::Finalize()
		{
			FND::SafeRelease(texture2D);
			FND::SafeRelease(depthStencilView);
		}
	} // namespace Graphics
} // namespace Phoenix