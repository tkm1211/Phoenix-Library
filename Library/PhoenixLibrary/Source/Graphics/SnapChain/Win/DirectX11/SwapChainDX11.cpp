#include "pch.h"
#include "SwapChainDX11.h"
#include "Phoenix/FND/Assert.h"
#include "Phoenix/FND/Logger.h"
#include "Phoenix/FND/Util.h"
#include "Device/Win/DirectX11/DeviceDX11.h"
#include "Surface/Win/DirectX11/RenderTargetSurfaceDX11.h"
#include "Surface/Win/DirectX11/DepthStencilSurfaceDX11.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11�ŃX���b�v�`�F�[������I�u�W�F�N�g
		//****************************************************************************
		// ����
		std::unique_ptr<ISwapChain> ISwapChain::Create()
		{
			return std::make_unique<SwapChainDX11>();
		}

		// ������
		bool SwapChainDX11::Initialize(IDevice* device, const SwapChainDesc& desc)
		{
			DeviceDX11* deviceDX11 = static_cast<DeviceDX11*>(device);
			ID3D11Device* d3dDevice = deviceDX11->GetD3DDevice();
			IDXGIFactory* dxgiFactory = deviceDX11->GetDXGIFactory();
			HWND hwnd = (HWND)desc.windowHandle;

			if (!CreateSwapChain(d3dDevice, dxgiFactory, hwnd, desc.width, desc.heigth))
			{
				return false;
			}

			renderTargerSurface = IRenderTargetSurface::Create();
			depthStencilSurface = IDepthStencilSurface::Create();

			if (!CreateBackBuffer(device, desc.width, desc.heigth))
			{
				return false;
			}

			return true;
		}

		// �I����
		void SwapChainDX11::Finalize()
		{
			FND::SafeRelease(swapChain);
			renderTargerSurface.reset();
			depthStencilSurface.reset();
		}

		// ��ʕ\��
		void SwapChainDX11::Present(int syncInterval)
		{
			swapChain->Present(syncInterval, 0);
		}

		// �X���b�v�`�F�[���쐬
		bool SwapChainDX11::CreateSwapChain(ID3D11Device* device, IDXGIFactory* dxgiFactory, HWND hwnd, unsigned int width, unsigned int height, bool enableMsaa, int subSamples)
		{
			HRESULT hr = S_OK;

			DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
			swapChainDesc.BufferDesc.Width = width;
			swapChainDesc.BufferDesc.Height = height;
			swapChainDesc.BufferDesc.RefreshRate.Numerator = 60; // 0 is a variable Frame, 60 is a fixed Frame
			swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
			swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			//swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
			swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

			// Get Num Quality Level
			UINT msaaQualityLevel;
			UINT sampleCount = subSamples;

			hr = device->CheckMultisampleQualityLevels(swapChainDesc.BufferDesc.Format, sampleCount, &msaaQualityLevel);
			if (FAILED(hr))
			{
				PHOENIX_LOG_GRP_ERROR("ID3D11Device::CheckMultisampleQualityLevels() : Failed!!\n");
				return false;
			}

			// Create Swap Chain
			swapChainDesc.SampleDesc.Count = enableMsaa ? sampleCount : 1;
			swapChainDesc.SampleDesc.Quality = enableMsaa ? msaaQualityLevel - 1 : 0;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount = 1;
			swapChainDesc.OutputWindow = hwnd;
			swapChainDesc.Windowed = TRUE;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			swapChainDesc.Flags = 0;

			hr = dxgiFactory->CreateSwapChain(device, &swapChainDesc, &swapChain);
			if (FAILED(hr))
			{
				PHOENIX_LOG_GRP_ERROR("IDXGIFactory::CreateSwapChain() : Failed!!\n");
				return false;
			}

			return true;
		}

		// �o�b�N�o�b�t�@�쐬
		bool SwapChainDX11::CreateBackBuffer(IDevice* device, unsigned int width, unsigned int height)
		{
			ID3D11Device* d3dDevice = static_cast<DeviceDX11*>(device)->GetD3DDevice();

			// �����_�[�^�[�Q�b�g
			{
				// �o�b�N�o�b�t�@���擾
				ID3D11Texture2D* texture2d;
				HRESULT hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&texture2d));
				if (FAILED(hr))
				{
					PHOENIX_LOG_GRP_ERROR("IDXGISwapChain::GetBuffer() : Failed!!\n");
					return false;
				}

				RenderTargetSurfaceDX11* renderTargetSurfaceDX11 = static_cast<RenderTargetSurfaceDX11*>(renderTargerSurface.get());
				if (!renderTargetSurfaceDX11->Initialize(d3dDevice, texture2d, nullptr))
				{
					return false;
				}
			}

			// �[�x�X�e���V���r���[
			{
				DepthStencilSurfaceDesc desc = {};
				desc.width = width;
				desc.height = height;
				if (!depthStencilSurface->Initialize(device, desc))
				{
					return false;
				}
			}

			return true;
		}
	} // namespace Graphics
} // namespace Phoenix