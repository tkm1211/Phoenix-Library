#include "pch.h"
#include "ContextDX11.h"
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
		// DirectX11版コンテキスト操作オブジェクト
		//****************************************************************************
		// 生成
		std::unique_ptr<IContext> IContext::Create()
		{
			return std::make_unique<ContextDX11>();
		}

		// 初期化
		bool ContextDX11::Initialize(IDevice* device)
		{
			deviceContext = static_cast<DeviceDX11*>(device)->GetD3DContext();
			deviceContext->AddRef();

			return true;
		}

		void ContextDX11::Finalize()
		{
			FND::SafeRelease(deviceContext);
		}

		void ContextDX11::ClearRenderTargetView(IRenderTargetSurface* renderTargetSurface, const f32* color)
		{
			ID3D11RenderTargetView* renderTargetView = static_cast<RenderTargetSurfaceDX11*>(renderTargetSurface)->GetD3DRenderTargetView();
			deviceContext->ClearRenderTargetView(renderTargetView, color);
		}

		void ContextDX11::ClearDepthStencilView(IDepthStencilSurface* depthStencilSurface, f32 depth, u8 stencil)
		{
			ID3D11DepthStencilView* depthStencilView = static_cast<DepthStencilSurfaceDX11*>(depthStencilSurface)->GetD3DDepthStencilView();
			deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
		}

		void ContextDX11::SetRenderTargets(u32 count, IRenderTargetSurface* renderTargetSurface[], IDepthStencilSurface* depthStencilSurface)
		{
			ID3D11RenderTargetView* renderTargetViews[8];
			u32 size = static_cast<u32>(sizeof(renderTargetViews));
			count = count < size ? count : size;

			for (u32  i = 0; i < count; ++i)
			{
				if (renderTargetSurface[i])
				{
					renderTargetViews[i] = static_cast<RenderTargetSurfaceDX11*>(renderTargetSurface[i])->GetD3DRenderTargetView();
				}
				else
				{
					renderTargetViews[i] = nullptr;
				}
			}

			ID3D11DepthStencilView* depthStencilView;
			if (depthStencilSurface)
			{
				depthStencilView = static_cast<DepthStencilSurfaceDX11*>(depthStencilSurface)->GetD3DDepthStencilView();
			}
			else
			{
				depthStencilView = nullptr;
			}

			deviceContext->OMSetRenderTargets(count, renderTargetViews, depthStencilView);
		}

		void ContextDX11::GetRenderTargets(u32 count, IRenderTargetSurface* renderTargetSurface[], IDepthStencilSurface* depthStencilSurface)
		{
			RenderTargetSurfaceDX11* renderTargetSurfaceDX11[8];
			ID3D11RenderTargetView* renderTargetViews[8];
			u32 size = static_cast<u32>(sizeof(renderTargetViews));
			count = count < size ? count : size;

			for (u32 i = 0; i < count; ++i)
			{
				renderTargetSurfaceDX11[i] = static_cast<RenderTargetSurfaceDX11*>(renderTargetSurface[i]);
				renderTargetViews[i] = renderTargetSurfaceDX11[i]->GetD3DRenderTargetView();
			}

			DepthStencilSurfaceDX11* depthStencilSurfaceDX11 = static_cast<DepthStencilSurfaceDX11*>(depthStencilSurface);
			ID3D11DepthStencilView* depthStencilView = depthStencilSurfaceDX11->GetD3DDepthStencilView();

			deviceContext->OMGetRenderTargets(count, renderTargetViews, &depthStencilView);

			for (u32 i = 0; i < count; ++i)
			{
				renderTargetSurfaceDX11[i]->SetD3DRenderTargetView(renderTargetViews[i]);
			}
			depthStencilSurfaceDX11->SetDepthStencilView(depthStencilView);
		}

		void ContextDX11::SetViewports(u32 count, const Viewport viewports[])
		{
			D3D11_VIEWPORT d3dViewports[8];
			u32 size = static_cast<u32>(sizeof(viewports));
			count = count < size ? count : size;

			if (count > 0)
			{
				for (u32 i = 0; i < count; ++i)
				{
					d3dViewports[i].TopLeftX = viewports[i].x;
					d3dViewports[i].TopLeftY = viewports[i].y;
					d3dViewports[i].Width = viewports[i].width;
					d3dViewports[i].Height = viewports[i].height;
					d3dViewports[i].MinDepth = viewports[i].minDepth;
					d3dViewports[i].MaxDepth = viewports[i].maxDepth;
				}
				deviceContext->RSSetViewports(count, d3dViewports);
			}
			else
			{
				deviceContext->RSSetViewports(0, nullptr);
			}
		}

		void ContextDX11::GetViewports(u32 count, Viewport* viewports[])
		{
			D3D11_VIEWPORT d3dViewports[8];
			u32 size = static_cast<u32>(sizeof(viewports));
			count = count < size ? count : size;

			deviceContext->RSGetViewports(&count, d3dViewports);

			for (u32 i = 0; i < count; ++i)
			{
				viewports[i]->x = d3dViewports[i].TopLeftX;
				viewports[i]->y = d3dViewports[i].TopLeftY;
				viewports[i]->width = d3dViewports[i].Width;
				viewports[i]->height = d3dViewports[i].Height;
				viewports[i]->minDepth = d3dViewports[i].MinDepth;
				viewports[i]->maxDepth = d3dViewports[i].MaxDepth;
			}
		}
	} // namespace Graphics
} // namespace Phoenix