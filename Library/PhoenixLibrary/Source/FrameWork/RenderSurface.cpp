#include "pch.h"
#include "Phoenix/FrameWork/RenderSurface.h"


namespace Phoenix
{
	namespace FrameWork
	{
		//****************************************************************************
		// �����_�[�T�[�t�F�C�X
		//****************************************************************************
		// ������
		bool RenderSurface::Initialize(Graphics::IDevice* device, u32 width, u32 height)
		{
			this->width = width;
			this->height = height;

			// �����_�[�^�[�Q�b�g
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

			// �f�v�X�X�e���V��
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

		// �I����
		void RenderSurface::Finalize()
		{
			rts.reset();
			dss.reset();
		}

		// �I����
		void RenderSurface::Resize(Graphics::IDevice* device, u32 width, u32 height)
		{
			Finalize();
			Initialize(device, width, height);
		}
	} // namespace FrameWork
} // namespace Phoenix