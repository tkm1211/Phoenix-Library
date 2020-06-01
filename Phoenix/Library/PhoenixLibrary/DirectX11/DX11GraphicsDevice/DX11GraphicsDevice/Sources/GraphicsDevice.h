#pragma once

#ifdef DX11GRAPHICSDEVICE_EXPORTS
#define DX11GRAPHICSDEVICE_API __declspec(dllexport)
#else
#define DX11GRAPHICSDEVICE_API __declspec(dllimport)
#endif

#include <Windows.h>
#include <wrl.h>
#include <d3d11.h>


class DX11GRAPHICSDEVICE_API GraphicsDevice
{
public:
	virtual void Init(HWND hwnd, bool enableMsaa = false, int subSamples = 1) = 0;
	virtual void UnInit() = 0;

	virtual void SetViewports(float topLeftX, float topLeftY, float width, float height, float minDepth = 0.0f, float maxDepth = 1.0f) = 0;
	virtual void Clear(float r = 0.75f, float g = 0.75f, float b = 0.75f, float a = 1.0f) = 0;
	virtual void SetRenderTargetAndDepthStesilState() = 0;

	virtual void ClearRenderTargets() = 0;
	virtual void ClearShaderResources() = 0;

	virtual void SetSamplers() = 0;
	virtual void SetRasterizerState() = 0;
	virtual void SetBlendState() = 0;

	virtual void Present(int syncInterval) = 0;

public:
	virtual Microsoft::WRL::ComPtr<ID3D11Device> GetDevice() = 0;
	virtual Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetImmediateContext() = 0;
};