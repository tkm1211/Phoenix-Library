#pragma once

#ifdef DX11GRAPHICSDEVICE_EXPORTS
#define DX11GRAPHICSDEVICE_API __declspec(dllexport)
#else
#define DX11GRAPHICSDEVICE_API __declspec(dllimport)
#endif

#include <Windows.h>
#include <wrl.h>
#include <d3d11.h>

#include "DX11GraphicsDevice.h"


#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "dxgi.lib" )


namespace Phoenix
{
	namespace Lib
	{
		class DX11GraphicsDevice
		{
		private:
			UINT width;
			UINT height;

			Microsoft::WRL::ComPtr<ID3D11Device> device;
			Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext;
			Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;

			Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
			Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;

			Microsoft::WRL::ComPtr<ID3D11DepthStencilState> defaultDepthStencilState;
			Microsoft::WRL::ComPtr<ID3D11RasterizerState> defaultRasterizerState;
			Microsoft::WRL::ComPtr<ID3D11BlendState> defaultBlendState;
			Microsoft::WRL::ComPtr<ID3D11SamplerState> defaultSamplerState;

		private:
			Microsoft::WRL::ComPtr<IDXGIFactory> dxgiFactory;
			Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;

		public:
			DX11GRAPHICSDEVICE_API DX11GraphicsDevice() : width(0), height(0) {}
			DX11GRAPHICSDEVICE_API ~DX11GraphicsDevice() {}

		public:
			DX11GRAPHICSDEVICE_API void Init(HWND hwnd, bool enableMsaa = false, int subSamples = 1);
			DX11GRAPHICSDEVICE_API void UnInit() {}

			DX11GRAPHICSDEVICE_API void SetViewports(float topLeftX, float topLeftY, float width, float height, float minDepth = 0.0f, float maxDepth = 1.0f);
			DX11GRAPHICSDEVICE_API void Clear(float r = 0.75f, float g = 0.75f, float b = 0.75f, float a = 1.0f);
			DX11GRAPHICSDEVICE_API void SetRenderTargetAndDepthStesilState();

			DX11GRAPHICSDEVICE_API void ClearRenderTargets();
			DX11GRAPHICSDEVICE_API void ClearShaderResources();

			DX11GRAPHICSDEVICE_API void SetSamplers();
			DX11GRAPHICSDEVICE_API void SetRasterizerState();
			DX11GRAPHICSDEVICE_API void SetBlendState();

			DX11GRAPHICSDEVICE_API void Present(int syncInterval);

		public:
			DX11GRAPHICSDEVICE_API Microsoft::WRL::ComPtr<ID3D11Device> GetDevice();
			DX11GRAPHICSDEVICE_API Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetImmediateContext();

		private:
			void CreateDevice();
			void CreateSnapChain(HWND hwnd, bool enableMsaa, int subSamples);
			void CreateBackBufferResources();
			void CreateDepthStensilView();
			void CreateDepthStensilState();
			void CreateRasterizerState();
			void CreateBlendState();
			void CreateSamplerState();
		};
	}
}