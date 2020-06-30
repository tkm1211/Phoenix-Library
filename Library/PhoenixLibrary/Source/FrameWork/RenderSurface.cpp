#include "pch.h"
#include "Phoenix/FrameWork/RenderSurface.h"


namespace Phoenix
{
	namespace FrameWork
	{
		//****************************************************************************
		// レンダーサーフェイス
		//****************************************************************************
		// 初期化
		bool RenderSurface::Initialize(Graphics::IDevice* device, u32 width, u32 height)
		{
			this->width = width;
			this->height = height;

			// レンダーターゲット
			{
				Graphics::RenderTargetSurfaceDesc desc;
				desc.width = width;
				desc.height = height;

				rts = Graphics::IRenderTargetSurface::Create();
				if (!rts->Initialize(device, desc))
				{
					return false;
				}
			}

			// デプスステンシル
			{
				Graphics::DepthStencilSurfaceDesc desc;
				desc.width = width;
				desc.height = height;

				dss = Graphics::IDepthStencilSurface::Create();
				if (!dss->Initialize(device, desc))
				{
					return false;
				}
			}

			return true;
		}

		// 終了化
		void RenderSurface::Finalize()
		{
			rts.reset();
			dss.reset();
		}

		// 終了化
		void RenderSurface::Resize(Graphics::IDevice* device, u32 width, u32 height)
		{
			Finalize();
			Initialize(device, width, height);
		}
	} // namespace FrameWork
} // namespace Phoenix