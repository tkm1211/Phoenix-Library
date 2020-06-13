#include "pch.h"
#include "Phoenix/FrameWork/Main.h"


namespace Phoenix
{
	namespace FrameWork
	{
		bool Main::Initialize(const wchar_t* name, s32 width, s32 height, uintPtr instance)
		{
			display = Phoenix::OS::IDisplay::Create();
			if (!display->Initialize(name, width, height, instance))
			{
				return false;
			}

			graphicsDevice = Phoenix::Graphics::IDevice::Create();
			if (!graphicsDevice->Initialize())
			{
				return false;
			}

			graphicsContext = Phoenix::Graphics::IContext::Create();
			if (!graphicsContext->Initialize(graphicsDevice.get()))
			{
				return false;
			}

			swapChain = Phoenix::Graphics::ISwapChain::Create();
			Phoenix::Graphics::SwapChainDesc desc = {};
			desc.width = display->GetWidth();
			desc.heigth = display->GetHeight();
			desc.windowHandle = reinterpret_cast<void*>(display->GetHandle());

			if (!swapChain->Initialize(graphicsDevice.get(), desc))
			{
				return false;
			}

			return true;
		}

		void Main::Finalize()
		{
			swapChain->Finalize();
			graphicsContext->Finalize();
			graphicsDevice->Finalize();
			display->Finalize();
		}

		void Main::Run()
		{
			Update();

			Phoenix::Graphics::IRenderTargetSurface* nullRTS[8] = {};
			Phoenix::Graphics::IDepthStencilSurface* nullDSS[8] = {};
			graphicsContext->SetRenderTargets(8, nullRTS, 0);

			float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
			Phoenix::Graphics::IRenderTargetSurface* rts = swapChain->GetRenderTargerSurface();
			Phoenix::Graphics::IDepthStencilSurface* dss = swapChain->GetDepthStencilSurface();

			graphicsContext->ClearRenderTargetView(rts, color);
			graphicsContext->ClearDepthStencilView(dss, 1.0f, 0);
			graphicsContext->SetRenderTargets(1, &rts, dss);

			Render();

			swapChain->Present(1);
		}
	}
}