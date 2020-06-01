#pragma once

#include "GraphicsDevice.h"

#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "dxgi.lib" )


class DX11GraphicsDevice : public GraphicsDevice
{
public:
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
	DX11GRAPHICSDEVICE_API DX11GraphicsDevice() {}
	DX11GRAPHICSDEVICE_API ~DX11GraphicsDevice() {}

public:
	DX11GRAPHICSDEVICE_API void Init(HWND hwnd, bool enableMsaa = false, int subSamples = 1) override;
	DX11GRAPHICSDEVICE_API void UnInit() override {}

	DX11GRAPHICSDEVICE_API void SetViewports(float topLeftX, float topLeftY, float width, float height, float minDepth = 0.0f, float maxDepth = 1.0f) override;
	DX11GRAPHICSDEVICE_API void Clear(float r = 0.75f, float g = 0.75f, float b = 0.75f, float a = 1.0f) override;
	DX11GRAPHICSDEVICE_API void SetRenderTargetAndDepthStesilState() override;

	DX11GRAPHICSDEVICE_API void ClearRenderTargets() override;
	DX11GRAPHICSDEVICE_API void ClearShaderResources() override;

	DX11GRAPHICSDEVICE_API void SetSamplers()  override;
	DX11GRAPHICSDEVICE_API void SetRasterizerState() override;
	DX11GRAPHICSDEVICE_API void SetBlendState() override;

	DX11GRAPHICSDEVICE_API void Present(int syncInterval) override;

public:
	DX11GRAPHICSDEVICE_API Microsoft::WRL::ComPtr<ID3D11Device> GetDevice() override;
	DX11GRAPHICSDEVICE_API Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetImmediateContext() override;

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