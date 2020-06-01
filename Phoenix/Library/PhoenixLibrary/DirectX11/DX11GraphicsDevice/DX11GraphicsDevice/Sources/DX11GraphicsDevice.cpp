#include <dxgi.h>
#include <assert.h>
#include <vector>

#include "DX11GraphicsDevice.h"


namespace Phoenix
{
	namespace Lib
	{
		void DX11GraphicsDevice::Init(HWND hwnd, bool enableMsaa, int subSamples)
		{
			// Window Size
			{
				RECT rect;
				GetClientRect(hwnd, &rect);

				width = rect.right - rect.left;
				height = rect.bottom - rect.top;
			}

			// Create Device
			CreateDevice();

			// Create SnapChain
			CreateSnapChain(hwnd, enableMsaa, subSamples);

			// Create BackBuffer
			CreateBackBufferResources();

			// Create DepthStesilView
			CreateDepthStensilView();

			// Create DepthStesilState
			CreateDepthStensilState();

			// Set Viewport
			SetViewports(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height));

			// Clear RenderTargetView And DepthStencilView
			Clear();

			// Set RenderTarget And DepthStesilState
			SetRenderTargetAndDepthStesilState();
		}

		void DX11GraphicsDevice::CreateDevice()
		{
			HRESULT hr = S_OK;

			UINT flags = 0;

			hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(dxgiFactory.GetAddressOf()));
			assert(!hr && "Error:CreateDXGIFactory");

			// Adapter
			IDXGIAdapter* dxgiAdapter;
			std::vector<IDXGIAdapter*> dxgiAdapters;

			for (size_t i = 0; dxgiFactory->EnumAdapters(i, &dxgiAdapter) != DXGI_ERROR_NOT_FOUND; i++)
			{
				dxgiAdapters.push_back(dxgiAdapter);
			}

			dxgiAdapter = dxgiAdapters[0];

			DXGI_ADAPTER_DESC dxgiAdapterDesc;
			dxgiAdapter->GetDesc(&dxgiAdapterDesc);


			// Driver Type
			D3D_DRIVER_TYPE driverTypes[] =
			{
				D3D_DRIVER_TYPE_UNKNOWN,
				D3D_DRIVER_TYPE_HARDWARE,
				D3D_DRIVER_TYPE_WARP,
				D3D_DRIVER_TYPE_REFERENCE,
			};
			UINT numDriverTypes = sizeof(driverTypes) / sizeof(driverTypes[0]);


			// Feature Level
			D3D_FEATURE_LEVEL featureLevels[] =
			{
				D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_10_1,
				D3D_FEATURE_LEVEL_10_0,
				D3D_FEATURE_LEVEL_9_3,
				D3D_FEATURE_LEVEL_9_2,
				D3D_FEATURE_LEVEL_9_1,
			};
			UINT numFeatureLevels = sizeof(featureLevels) / sizeof(featureLevels[0]);

			D3D_FEATURE_LEVEL featureLevel;


			// Create Device
			for (UINT idx = 0; idx < numDriverTypes; ++idx)
			{
				D3D_DRIVER_TYPE driverType = driverTypes[idx];
				hr = D3D11CreateDevice(
					dxgiAdapter,
					driverType,
					0,
					flags,
					featureLevels,
					_countof(featureLevels),
					D3D11_SDK_VERSION,
					device.GetAddressOf(),
					&featureLevel,
					immediateContext.GetAddressOf()
				);
				if (SUCCEEDED(hr))
				{
					break;
				}
			}
			assert(!hr && "Error:D3D11CreateDevice");
		}

		void DX11GraphicsDevice::CreateSnapChain(HWND hwnd, bool enableMsaa, int subSamples)
		{
			HRESULT hr = S_OK;

			DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
			swapChainDesc.BufferDesc.Width = width;
			swapChainDesc.BufferDesc.Height = height;
			swapChainDesc.BufferDesc.RefreshRate.Numerator = 0; // 0 is a variable Frame, 60 is a fixed Frame
			swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
			swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

			// Get Num Quality Level
			UINT msaaQualityLevel;
			UINT sampleCount = subSamples;

			hr = device->CheckMultisampleQualityLevels(swapChainDesc.BufferDesc.Format, sampleCount, &msaaQualityLevel);
			assert(!hr && "Error:CheckMultisampleQualityLevels");

			// Create Swap Chain
			swapChainDesc.SampleDesc.Count = enableMsaa ? sampleCount : 1;
			swapChainDesc.SampleDesc.Quality = enableMsaa ? msaaQualityLevel - 1 : 0;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount = 1;
			swapChainDesc.OutputWindow = hwnd;
			swapChainDesc.Windowed = TRUE;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			swapChainDesc.Flags = 0;

			hr = dxgiFactory->CreateSwapChain(device.Get(), &swapChainDesc, swapChain.GetAddressOf());
			assert(!hr && "Error:CreateSwapChain");
		}

		void DX11GraphicsDevice::CreateBackBufferResources()
		{
			HRESULT hr = S_OK;

			// Get Back Buffer
			hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
			assert(!hr && "Error:GetBuffer");

			hr = device->CreateRenderTargetView(backBuffer.Get(), NULL, renderTargetView.GetAddressOf());
			assert(!hr && "Error:CreateRenderTargetView");
		}

		void DX11GraphicsDevice::CreateDepthStensilView()
		{
			HRESULT hr = S_OK;

			Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStensilTextrue;

			// Get Back Buffer Desc
			D3D11_TEXTURE2D_DESC backBufferDesc = {};
			backBuffer->GetDesc(&backBufferDesc);

			D3D11_TEXTURE2D_DESC texture2dDesc = backBufferDesc;
			texture2dDesc.Width = width;
			texture2dDesc.Height = height;
			texture2dDesc.MipLevels = 1;
			texture2dDesc.ArraySize = 1;
			texture2dDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			texture2dDesc.SampleDesc.Count = 1;
			texture2dDesc.SampleDesc.Quality = 0;
			texture2dDesc.Usage = D3D11_USAGE_DEFAULT;
			texture2dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			texture2dDesc.CPUAccessFlags = 0;
			texture2dDesc.MiscFlags = 0;

			hr = device->CreateTexture2D(&texture2dDesc, nullptr, depthStensilTextrue.GetAddressOf());
			assert(!hr && "Error:CreateTexture2D");

			D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
			depthStencilViewDesc.Format = texture2dDesc.Format;
			depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			depthStencilViewDesc.Flags = 0;
			depthStencilViewDesc.Texture2D.MipSlice = 0;

			hr = device->CreateDepthStencilView(depthStensilTextrue.Get(), &depthStencilViewDesc, depthStencilView.GetAddressOf());
			assert(!hr && "Error:CreateDepthStencilView");
		}

		void DX11GraphicsDevice::CreateDepthStensilState()
		{
			HRESULT hr = S_OK;

			D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
			depthStencilDesc.DepthEnable = TRUE;
			depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
			depthStencilDesc.StencilEnable = FALSE;
			depthStencilDesc.StencilReadMask = 0xFF;
			depthStencilDesc.StencilWriteMask = 0xFF;
			depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
			depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
			depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

			hr = device->CreateDepthStencilState(&depthStencilDesc, defaultDepthStencilState.GetAddressOf());
			assert(!hr && "Error:CreateDepthStencilState");
		}

		void DX11GraphicsDevice::CreateRasterizerState()
		{
			HRESULT hr = S_OK;

			D3D11_RASTERIZER_DESC rasterizerDesc = {};
			rasterizerDesc.FillMode = D3D11_FILL_SOLID;
			rasterizerDesc.CullMode = D3D11_CULL_BACK;
			rasterizerDesc.FrontCounterClockwise = TRUE;
			rasterizerDesc.DepthBias = 0;
			rasterizerDesc.DepthBiasClamp = 0;
			rasterizerDesc.SlopeScaledDepthBias = 0;
			rasterizerDesc.DepthClipEnable = TRUE;
			rasterizerDesc.ScissorEnable = FALSE;
			rasterizerDesc.MultisampleEnable = TRUE;
			rasterizerDesc.AntialiasedLineEnable = FALSE;

			hr = device->CreateRasterizerState(&rasterizerDesc, defaultRasterizerState.GetAddressOf());
			assert(!hr && "Error:CreateRasterizerState");
		}

		void DX11GraphicsDevice::CreateBlendState()
		{
			HRESULT hr = S_OK;

			D3D11_BLEND_DESC blend_desc = {};
			blend_desc.AlphaToCoverageEnable = FALSE;
			blend_desc.IndependentBlendEnable = FALSE;
			blend_desc.RenderTarget[0].BlendEnable = TRUE;
			blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
			blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			hr = device->CreateBlendState(&blend_desc, defaultBlendState.GetAddressOf());
			assert(!hr && "Error:CreateBlendState");
		}

		void DX11GraphicsDevice::CreateSamplerState()
		{
			HRESULT hr = S_OK;

			D3D11_SAMPLER_DESC sampler_desc;
			sampler_desc.Filter = D3D11_FILTER_ANISOTROPIC;
			sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			sampler_desc.MipLODBias = 0;
			sampler_desc.MaxAnisotropy = 16;
			sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
			sampler_desc.BorderColor[0] = 0.0f;
			sampler_desc.BorderColor[1] = 0.0f;
			sampler_desc.BorderColor[2] = 0.0f;
			sampler_desc.BorderColor[3] = 0.0f;
			sampler_desc.MinLOD = 0;
			sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

			hr = device->CreateSamplerState(&sampler_desc, defaultSamplerState.GetAddressOf());
			assert(!hr && "Error:CreateSamplerState");
		}

		void DX11GraphicsDevice::SetViewports(float topLeftX, float topLeftY, float width, float height, float minDepth, float maxDepth)
		{
			D3D11_VIEWPORT viewport = {};
			viewport.TopLeftX = topLeftX;
			viewport.TopLeftY = topLeftY;
			viewport.Width = width;
			viewport.Height = height;
			viewport.MinDepth = minDepth;
			viewport.MaxDepth = maxDepth;

			immediateContext->RSSetViewports(1, &viewport);
		}

		void DX11GraphicsDevice::Clear(float r, float g, float b, float a)
		{
			float color[4] = { r, g, b, a };

			immediateContext->ClearRenderTargetView(renderTargetView.Get(), color);
			immediateContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
		}

		void DX11GraphicsDevice::SetRenderTargetAndDepthStesilState()
		{
			immediateContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());
			immediateContext->OMSetDepthStencilState(defaultDepthStencilState.Get(), 1);
		}

		void DX11GraphicsDevice::ClearRenderTargets()
		{
			ID3D11RenderTargetView* nullRenderTargetViews[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT] = {};
			immediateContext->OMSetRenderTargets(_countof(nullRenderTargetViews), nullRenderTargetViews, 0);
		}

		void DX11GraphicsDevice::ClearShaderResources()
		{
			ID3D11ShaderResourceView* null_shader_resource_views[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = {};
			immediateContext->VSSetShaderResources(0, _countof(null_shader_resource_views), null_shader_resource_views);
			immediateContext->PSSetShaderResources(0, _countof(null_shader_resource_views), null_shader_resource_views);
		}

		void DX11GraphicsDevice::SetSamplers()
		{
			immediateContext->VSSetSamplers(0, 1, defaultSamplerState.GetAddressOf());
			immediateContext->PSSetSamplers(0, 1, defaultSamplerState.GetAddressOf());
		}

		void DX11GraphicsDevice::SetRasterizerState()
		{
			immediateContext->RSSetState(defaultRasterizerState.Get());
		}

		void DX11GraphicsDevice::SetBlendState()
		{
			immediateContext->OMSetBlendState(defaultBlendState.Get(), 0, 0xFFFFFFFF);
		}

		void DX11GraphicsDevice::Present(int syncInterval)
		{
			HRESULT hr = S_OK;

			hr = swapChain->Present(syncInterval, 0);
			assert(!hr && "Error:Present");
		}


		Microsoft::WRL::ComPtr<ID3D11Device> DX11GraphicsDevice::GetDevice()
		{
			return device;
		}

		Microsoft::WRL::ComPtr<ID3D11DeviceContext> DX11GraphicsDevice::GetImmediateContext()
		{
			return immediateContext;
		}
	}
}