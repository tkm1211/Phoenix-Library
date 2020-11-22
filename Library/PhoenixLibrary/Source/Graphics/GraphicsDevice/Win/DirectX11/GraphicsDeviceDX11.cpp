#include "pch.h"
#include "GraphicsDeviceDX11.h"
#include "Context/Win/DirectX11/ContextDX11.h"


namespace Phoenix
{
	namespace Graphics
	{
		// ����
		std::shared_ptr<IGraphicsDevice> IGraphicsDevice::Create()
		{
			return std::make_shared<GraphicsDeviceDX11>();
		}

		// ������
		bool GraphicsDeviceDX11::Initialize(OS::IDisplay* display)
		{
			device = Phoenix::Graphics::IDevice::Create();
			if (!device->Initialize())
			{
				return false;
			}

			context = Phoenix::Graphics::IContext::Create();
			if (!context->Initialize(device.get()))
			{
				return false;
			}

			swapChain = Phoenix::Graphics::ISwapChain::Create();
			Phoenix::Graphics::SwapChainDesc desc = {};
			desc.width = display->GetWidth();
			desc.heigth = display->GetHeight();
			desc.windowHandle = reinterpret_cast<void*>(display->GetHandle());

			if (!swapChain->Initialize(device.get(), desc))
			{
				return false;
			}

			return true;
		}

		// �I����
		void GraphicsDeviceDX11::Finalize()
		{
			swapChain->Finalize();
			context->Finalize();
			device->Finalize();
		}

		// �����_�����O���ꂽ�C���[�W��\��
		void GraphicsDeviceDX11::Present(int syncInterval)
		{
			swapChain->Present(syncInterval);
		}

		// �`��J�n
		void GraphicsDeviceDX11::RenderBegin()
		{
			Phoenix::Graphics::IRenderTargetSurface* nullRTS[8] = {};
			Phoenix::Graphics::IDepthStencilSurface* nullDSS[8] = {};
			Phoenix::Graphics::ITexture* nullTexture[8] = {};
			context->SetRenderTargets(8, nullRTS, 0);
			context->SetShaderResources(ShaderType::Vertex, 0, 8, nullTexture);
			context->SetShaderResources(ShaderType::Pixel, 0, 8, nullTexture);

			ContextDX11* contextDX11 = static_cast<ContextDX11*>(context.get());

			ISampler* samplers[] =
			{
				contextDX11->GetSamplerState(SamplerState::LinearWrap)
			};
			context->SetSamplers(ShaderType::Vertex, 0, 1, samplers);
			context->SetSamplers(ShaderType::Pixel, 0, 1, samplers);
			context->SetRasterizer(contextDX11->GetRasterizerState(RasterizerState::SolidCullNone));
			context->SetBlend(contextDX11->GetBlendState(BlendState::AlphaBlend), 0, 0xFFFFFFFF);
			
			float color[] = { 0.5f, 0.5f, 0.5f, 1.0f };
			Phoenix::Graphics::IRenderTargetSurface* rts = swapChain->GetRenderTargerSurface();
			Phoenix::Graphics::IDepthStencilSurface* dss = swapChain->GetDepthStencilSurface();

			context->ClearRenderTargetView(rts, color);
			context->ClearDepthStencilView(dss, 1.0f, 0);

			context->SetRenderTargets(1, &rts, dss);
			context->SetDepthStencil(contextDX11->GetDepthStencilState(DepthState::TestAndWrite), 1);
		}

		// �`��I��
		void GraphicsDeviceDX11::RenderEnd()
		{
			// No Data.
		}
	} // namespace Graphics
} // namespace Phoenix
