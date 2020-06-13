#include "pch.h"
#include "RenderTargetSurfaceDX11.h"
#include "Phoenix/FND/Assert.h"
#include "Phoenix/FND/Logger.h"
#include "Phoenix/FND/Util.h"
#include "Device/Win/DirectX11/DeviceDX11.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11�Ń����_�[�^�[�Q�b�g�T�[�t�F�C�X����I�u�W�F�N�g
		//****************************************************************************
		// ����
		std::unique_ptr<IRenderTargetSurface> IRenderTargetSurface::Create()
		{
			return std::make_unique<RenderTargetSurfaceDX11>();
		}

		// ������
		bool RenderTargetSurfaceDX11::Initialize(ID3D11Device* d3dDevice, ID3D11Texture2D* d3dTexture2D, ID3D11ShaderResourceView* d3dShaderResourceView)
		{
			texture2D = d3dTexture2D;
			shaderResourceView = d3dShaderResourceView;

			HRESULT hr = d3dDevice->CreateRenderTargetView(texture2D, nullptr, &renderTargetView);
			if (FAILED(hr))
			{
				PHOENIX_LOG_GRP_ERROR("ID3D11Device::CreateRenderTargetView() : Failed!!\n");
				return false;
			}

			return true;
		}

		// ������
		bool RenderTargetSurfaceDX11::Initialize(IDevice* device, const RenderTargetSurfaceDesc& desc)
		{
			HRESULT hr;
			ID3D11Device* d3dDevice = static_cast<DeviceDX11*>(device)->GetD3DDevice();

			D3D11_TEXTURE2D_DESC d3dDesc = {};
			d3dDesc.Width = desc.width;
			d3dDesc.Height = desc.height;
			d3dDesc.MipLevels = 1;
			d3dDesc.ArraySize = 1;
			d3dDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			d3dDesc.SampleDesc.Count = 1;
			d3dDesc.SampleDesc.Quality = 0;
			d3dDesc.Usage = D3D11_USAGE_DEFAULT;
			d3dDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			d3dDesc.CPUAccessFlags = 0;
			d3dDesc.MiscFlags = 0;

			ID3D11Texture2D* d3dTexture;
			hr = d3dDevice->CreateTexture2D(&d3dDesc, nullptr, &d3dTexture);
			if (FAILED(hr))
			{
				PHOENIX_LOG_GRP_ERROR("ID3D11Device::CreateTexture2D() : Failed!!\n");
				return false;
			}

			ID3D11ShaderResourceView* d3dShaderResourceView;
			hr = d3dDevice->CreateShaderResourceView(d3dTexture, nullptr, &d3dShaderResourceView);
			if (FAILED(hr))
			{
				d3dTexture->Release();
				PHOENIX_LOG_GRP_ERROR("ID3D11Device::CreateShaderResourceView() : Failed!!\n");
				return false;
			}

			texture2D = d3dTexture;
			shaderResourceView = d3dShaderResourceView;

			hr = d3dDevice->CreateRenderTargetView(texture2D, nullptr, &renderTargetView);
			if (FAILED(hr))
			{
				PHOENIX_LOG_GRP_ERROR("ID3D11Device::CreateRenderTargetView() : Failed!!\n");
				return false;
			}

			return true;
		}

		// �I����
		void RenderTargetSurfaceDX11::Finalize()
		{
			FND::SafeRelease(texture2D);
			FND::SafeRelease(shaderResourceView);
			FND::SafeRelease(renderTargetView);
		}
	} // namespace Graphics
} // namespace Phoenix