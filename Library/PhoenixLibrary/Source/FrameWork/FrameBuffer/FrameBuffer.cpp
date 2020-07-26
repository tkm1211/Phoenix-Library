#include "pch.h"
#include "Phoenix/FrameWork/FrameBuffer/FrameBuffer.h"
#include "Phoenix/FND/Logger.h"
#include "Phoenix/FND/Util.h"
#include "../Source/Graphics/Device/Win/DirectX11/DeviceDX11.h"
#include "../Source/Graphics/Texture/Win/DirectX11/TextureDX11.h"
#include "../Source/Graphics/Surface/Win/DirectX11/RenderTargetSurfaceDX11.h"
#include "../Source/Graphics/Surface/Win/DirectX11/DepthStencilSurfaceDX11.h"


namespace Phoenix
{
	namespace FrameWork
	{
		std::unique_ptr<FrameBuffer> FrameBuffer::Create()
		{
			return std::make_unique<FrameBuffer>();
		}

		bool FrameBuffer::Initialize
		(
			Graphics::IGraphicsDevice* graphicsDevice,
			u32 width,
			u32 height,
			bool enableMSAA,
			int subsamples,
			Graphics::TextureFormatDx renderTargetTexture2dFormat,
			Graphics::TextureFormatDx depthStencilTexture2dFormat,
			bool needRenderTargetShaderResourceView,
			bool needDepthStencilShaderResourceView,
			bool generateMips
		)
		{
			Graphics::IDevice* device = graphicsDevice->GetDevice();
			Graphics::DeviceDX11* d3dDevice = static_cast<Graphics::DeviceDX11*>(device);

			HRESULT hr = S_OK;

			UINT msaaQualityLevel;
			UINT sampleCount = subsamples;
			DXGI_FORMAT dxgiFormat = static_cast<DXGI_FORMAT>(renderTargetTexture2dFormat);
			hr = d3dDevice->GetD3DDevice()->CheckMultisampleQualityLevels(dxgiFormat, sampleCount, &msaaQualityLevel);
			if (FAILED(hr))
			{
				PHOENIX_LOG_GRP_ERROR("ID3D11Device::CheckMultisampleQualityLevels() : Failed!!\n");
				return false;
			}

			if (renderTargetTexture2dFormat != Graphics::TextureFormatDx::UNKNOWN)
			{
				D3D11_TEXTURE2D_DESC texture2dDesc = {};
				texture2dDesc.Width = width;
				texture2dDesc.Height = height;
				texture2dDesc.MipLevels = generateMips ? 0 : 1;
				texture2dDesc.ArraySize = 1;
				texture2dDesc.Format = dxgiFormat;
				texture2dDesc.SampleDesc.Count = enableMSAA ? sampleCount : 1;
				texture2dDesc.SampleDesc.Quality = enableMSAA ? msaaQualityLevel - 1 : 0;
				texture2dDesc.Usage = D3D11_USAGE_DEFAULT;
				texture2dDesc.BindFlags = needRenderTargetShaderResourceView ? D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE : D3D11_BIND_RENDER_TARGET;
				texture2dDesc.CPUAccessFlags = 0;
				texture2dDesc.MiscFlags = generateMips ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

				D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
				rtvDesc.Format = texture2dDesc.Format;
				rtvDesc.ViewDimension = enableMSAA ? D3D11_RTV_DIMENSION_TEXTURE2DMS : D3D11_RTV_DIMENSION_TEXTURE2D;

				D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
				if (needRenderTargetShaderResourceView)
				{
					srvDesc.Format = texture2dDesc.Format;
					srvDesc.ViewDimension = enableMSAA ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;
					srvDesc.Texture2D.MipLevels = generateMips ? -1 : 1;
				}

				renderTargerSurface = Graphics::IRenderTargetSurface::Create();
				Graphics::RenderTargetSurfaceDX11* rtsDX11 = static_cast<Graphics::RenderTargetSurfaceDX11*>(renderTargerSurface.get());
				if (!rtsDX11->Initialize(d3dDevice->GetD3DDevice(), texture2dDesc, &rtvDesc, &srvDesc))
				{
					PHOENIX_LOG_GRP_ERROR("Phoenix::Graphics::RenderTargetSurfaceDX11::Initialize() : Failed!!\n");
					return false;
				}
			}

			if (depthStencilTexture2dFormat != Graphics::TextureFormatDx::UNKNOWN)
			{
				const DXGI_FORMAT combinationsOfDepthStencilFormats[3][3] =
				{
					{ DXGI_FORMAT_R24G8_TYPELESS, DXGI_FORMAT_D24_UNORM_S8_UINT ,DXGI_FORMAT_R24_UNORM_X8_TYPELESS },
					{ DXGI_FORMAT_R32_TYPELESS, DXGI_FORMAT_D32_FLOAT, DXGI_FORMAT_R32_FLOAT },
					{ DXGI_FORMAT_R16_TYPELESS, DXGI_FORMAT_D16_UNORM , DXGI_FORMAT_R16_UNORM },
				};

				int depthStencilTexture2dFormatIndex = 0;
				switch (static_cast<DXGI_FORMAT>(depthStencilTexture2dFormat))
				{
				case DXGI_FORMAT_R24G8_TYPELESS:
					depthStencilTexture2dFormatIndex = 0;
					break;
				case DXGI_FORMAT_R32_TYPELESS:
					depthStencilTexture2dFormatIndex = 1;
					break;
				case DXGI_FORMAT_R16_TYPELESS:
					depthStencilTexture2dFormatIndex = 2;
					break;
				}

				D3D11_TEXTURE2D_DESC texture2dDesc = {};
				texture2dDesc.Width = width;
				texture2dDesc.Height = height;
				texture2dDesc.MipLevels = 1;
				texture2dDesc.ArraySize = 1;
				texture2dDesc.Format = combinationsOfDepthStencilFormats[depthStencilTexture2dFormatIndex][0];
				texture2dDesc.SampleDesc.Count = enableMSAA ? sampleCount : 1;
				texture2dDesc.SampleDesc.Quality = enableMSAA ? msaaQualityLevel - 1 : 0;
				texture2dDesc.Usage = D3D11_USAGE_DEFAULT;
				texture2dDesc.BindFlags = needDepthStencilShaderResourceView ? D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE : D3D11_BIND_DEPTH_STENCIL;
				texture2dDesc.CPUAccessFlags = 0;
				texture2dDesc.MiscFlags = 0;

				D3D11_DEPTH_STENCIL_VIEW_DESC dssDesc = {};
				dssDesc.Format = combinationsOfDepthStencilFormats[depthStencilTexture2dFormatIndex][1];
				dssDesc.ViewDimension = enableMSAA ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
				dssDesc.Flags = 0;

				D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
				if (needDepthStencilShaderResourceView)
				{
					srvDesc.Format = combinationsOfDepthStencilFormats[depthStencilTexture2dFormatIndex][2];
					srvDesc.ViewDimension = enableMSAA ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;
					srvDesc.Texture2D.MipLevels = 1;
				}

				depthStencilSurface = Graphics::IDepthStencilSurface::Create();
				Graphics::DepthStencilSurfaceDX11* dssDX11 = static_cast<Graphics::DepthStencilSurfaceDX11*>(depthStencilSurface.get());
				if (!dssDX11->Initialize(d3dDevice->GetD3DDevice(), texture2dDesc, &dssDesc, &srvDesc))
				{
					PHOENIX_LOG_GRP_ERROR("Phoenix::Graphics::DepthStencilSurfaceDX11::Initialize() : Failed!!\n");
					return false;
				}
			}

			viewport.width = static_cast<float>(width);
			viewport.height = static_cast<float>(height);
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;
			viewport.x = 0.0f;
			viewport.y = 0.0f;

			cachedRenderTargerSurface = Graphics::IRenderTargetSurface::Create();
			cachedDepthStencilSurface = Graphics::IDepthStencilSurface::Create();

			for (u32 i = 0; i < ViewportCount; ++i)
			{
				cachedViewports[i] = new Graphics::Viewport();
			}

			return true;
		}

		void FrameBuffer::Finalize()
		{
			for (u32 i = 0; i < ViewportCount; ++i)
			{
				FND::SafeDelete(cachedViewports[i]);
			}

			cachedDepthStencilSurface.reset();
			cachedRenderTargerSurface.reset();

			depthStencilSurface.reset();
			renderTargerSurface.reset();
		}

		void FrameBuffer::Clear(Graphics::IGraphicsDevice* graphicsDevice, float r, float g, float b, float a, float depth, u8 stencil)
		{
			Graphics::IContext* context = graphicsDevice->GetContext();

			float color[4] = { r, g, b, a };
			if (renderTargerSurface.get())
			{
				context->ClearRenderTargetView(renderTargerSurface.get(), color);
			}
			if (depthStencilSurface.get())
			{
				context->ClearDepthStencilView(depthStencilSurface.get(), 1.0f, 0);
			}
		}

		//clear only 'render_target_view'
		void FrameBuffer::ClearRenderTargetView(Graphics::IGraphicsDevice* graphicsDevice, float r, float g, float b, float a)
		{
			Graphics::IContext* context = graphicsDevice->GetContext();

			float color[4] = { r, g, b, a };
			context->ClearRenderTargetView(renderTargerSurface.get(), color);
		}

		//clear only 'depth_stencil_view'
		void FrameBuffer::ClearDepthStencilView(Graphics::IGraphicsDevice* graphicsDevice, FLOAT depth, UINT8 stencil)
		{
			Graphics::IContext* context = graphicsDevice->GetContext();
			context->ClearDepthStencilView(depthStencilSurface.get(), depth, stencil);
		}

		void FrameBuffer::Activate(Graphics::IGraphicsDevice* graphicsDevice)
		{
			numberOfStoredViewports = ViewportCount;

			Graphics::IContext* context = graphicsDevice->GetContext();
			Graphics::Viewport* v = { &viewport };
			Graphics::IRenderTargetSurface* rts = renderTargerSurface.get();
			Graphics::IDepthStencilSurface* dss = depthStencilSurface.get();
			Graphics::IRenderTargetSurface* cachedRTS[] = { cachedRenderTargerSurface.get() };
			Graphics::IDepthStencilSurface* cachedDSS = { cachedDepthStencilSurface.get() };

			context->GetViewports(numberOfStoredViewports, cachedViewports);
			context->SetViewports(1, v);

			context->GetRenderTargets(1, cachedRTS, cachedDSS);
			context->SetRenderTargets(1, &rts, dss);
		}

		//activate only 'render_target_view'
		void FrameBuffer::ActivateRenderTargetView(Graphics::IGraphicsDevice* graphicsDevice)
		{
			numberOfStoredViewports = ViewportCount;

			Graphics::IContext* context = graphicsDevice->GetContext();
			Graphics::Viewport* v = { &viewport };
			Graphics::IRenderTargetSurface* rts = renderTargerSurface.get();
			Graphics::IRenderTargetSurface* cachedRTS[] = { cachedRenderTargerSurface.get() };
			Graphics::IDepthStencilSurface* cachedDSS = { cachedDepthStencilSurface.get() };

			context->GetViewports(numberOfStoredViewports, cachedViewports);
			context->SetViewports(1, v);

			context->GetRenderTargets(1, cachedRTS, cachedDSS);
			context->SetRenderTargets(1, &rts, 0);
		}

		//activate only 'depth_stencil_view'
		void FrameBuffer::ActivateDepthStencilView(Graphics::IGraphicsDevice* graphicsDevice)
		{
			numberOfStoredViewports = ViewportCount;

			Graphics::IContext* context = graphicsDevice->GetContext();
			Graphics::Viewport* v = { &viewport };
			Graphics::IRenderTargetSurface* nullRenderTargetSurface = 0;
			Graphics::IRenderTargetSurface* cachedRTS = { cachedRenderTargerSurface.get() };
			Graphics::IDepthStencilSurface* cachedDSS = { cachedDepthStencilSurface.get() };

			context->GetViewports(numberOfStoredViewports, cachedViewports);
			context->SetViewports(1, v);

			context->GetRenderTargets(1, &cachedRTS, cachedDSS);
			context->SetRenderTargets(1, &nullRenderTargetSurface, depthStencilSurface.get());
		}

		void FrameBuffer::Deactivate(Graphics::IGraphicsDevice* graphicsDevice)
		{
			Graphics::IContext* context = graphicsDevice->GetContext();
			Graphics::IRenderTargetSurface* rts = cachedRenderTargerSurface.get();
			Graphics::IDepthStencilSurface* dss = cachedDepthStencilSurface.get();

			context->SetViewports(numberOfStoredViewports, *cachedViewports);
			context->SetRenderTargets(1, &rts, dss);
		}


		std::unique_ptr<MSAAResolve> MSAAResolve::Create()
		{
			return std::make_unique<MSAAResolve>();
		}

		bool MSAAResolve::Initialize(Graphics::IGraphicsDevice* graphicsDevice)
		{
			Graphics::IDevice* device = graphicsDevice->GetDevice();

			embeddedShader = Graphics::IShader::Create();
			embeddedShader->LoadVS
			(
				device,
				"MSAAResolveVS.cso",
				0,
				0
			);
			embeddedShader->LoadPS
			(
				device,
				"MSAAResolvePS.cso"
			);

			embeddedRasterizerState = Graphics::IRasterizer::Create();
			if (!embeddedRasterizerState->Initialize(device, Graphics::RasterizerState::SolidCullNone))
			{
				return false;
			}

			embeddedDepthStencilState = Graphics::IDepthStencil::Create();
			if (!embeddedDepthStencilState->Initialize(device, Graphics::DepthState::NoTestNoWrite))
			{
				return false;
			}

			return true;
		}

		void MSAAResolve::Finalize()
		{
			embeddedDepthStencilState.reset();
			embeddedRasterizerState.reset();
			embeddedShader.reset();
		}

		void MSAAResolve::Resolve(Graphics::IGraphicsDevice* graphicsDevice, const FrameBuffer* source, FrameBuffer* destination)
		{
			Graphics::IDevice* device = graphicsDevice->GetDevice();
			Graphics::IContext* context = graphicsDevice->GetContext();
			ID3D11DeviceContext* d3dContext = static_cast<Graphics::DeviceDX11*>(device)->GetD3DContext();

			if (destination->renderTargerSurface.get()->GetTexture()->Handle() && source->renderTargerSurface.get()->GetTexture()->Handle())
			{
				Microsoft::WRL::ComPtr<ID3D11Resource> sourceResource;
				Microsoft::WRL::ComPtr<ID3D11Resource> destinationResource;

				static_cast<ID3D11ShaderResourceView*>(source->renderTargerSurface->GetTexture()->Handle())->GetResource(sourceResource.GetAddressOf());
				static_cast<ID3D11ShaderResourceView*>(destination->renderTargerSurface->GetTexture()->Handle())->GetResource(destinationResource.GetAddressOf());

				Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2d;
				HRESULT hr = sourceResource.Get()->QueryInterface<ID3D11Texture2D>(texture2d.GetAddressOf());
				//_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

				D3D11_TEXTURE2D_DESC texture2dDesc;
				texture2d->GetDesc(&texture2dDesc);
				//_ASSERT_EXPR(texture2d_desc.SampleDesc.Count > 1, L"source texture must be multisample texture");

				d3dContext->ResolveSubresource(destinationResource.Get(), D3D11CalcSubresource(0, 0, 1), sourceResource.Get(), D3D11CalcSubresource(0, 0, 1), texture2dDesc.Format);
			}
			if (destination->depthStencilSurface.get()->GetTexture()->Handle() && source->depthStencilSurface.get()->GetTexture()->Handle())
			{
				destination->ClearDepthStencilView(graphicsDevice);
				destination->ActivateDepthStencilView(graphicsDevice);

				std::unique_ptr<Graphics::IRasterizer> cachedRasterizerState = Graphics::IRasterizer::Create();
				context->GetRasterizer(cachedRasterizerState.get());
				context->SetRasterizer(embeddedRasterizerState.get());

				std::unique_ptr<Graphics::IDepthStencil> cachedDepthStencilState = Graphics::IDepthStencil::Create();
				context->GetDepthStencil(cachedDepthStencilState.get(), 0);
				context->SetDepthStencil(embeddedDepthStencilState.get(), 1);

				d3dContext->IASetVertexBuffers(0, 0, 0, 0, 0);
				d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
				d3dContext->IASetInputLayout(0);

				embeddedShader->Activate(device);

				Graphics::ITexture* texture[] = { source->depthStencilSurface->GetTexture() };
				context->SetShaderResources(Graphics::ShaderType::Pixel, 0, 1, texture);
				context->Draw(4, 0);

				context->SetRasterizer(cachedRasterizerState.get());
				context->SetDepthStencil(cachedDepthStencilState.get(), 1);

				embeddedShader->Deactivate(device);

				Graphics::ITexture* nullTexture[] = { nullptr };
				context->SetShaderResources(Graphics::ShaderType::Pixel, 0, 1, nullTexture);

				destination->Deactivate(graphicsDevice);
			}
		}
	}
}