#pragma once

#include <memory>

#include "Phoenix/Types.h"
#include "Phoenix/FND/Base.h"
#include "Phoenix/Graphics/Device.h"
#include "Phoenix/Graphics/Surface.h"
#include "Phoenix/Graphics/Viewport.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// �R���e�L�X�g����C���^�[�t�F�[�X
		//****************************************************************************
		class IContext : public FND::Base
		{
		public:
			// ����
			static std::unique_ptr<IContext> Create();

			// ������
			virtual bool Initialize(IDevice* device) = 0;

			// �I����
			virtual void Finalize() = 0;

			// �����_�[�^�[�Q�b�g�r���[�N���A
			virtual void ClearRenderTargetView(IRenderTargetSurface* renderTargetSurface, const f32* color) = 0;
			
			// �[�x�X�e���V���r���[�N���A
			virtual void ClearDepthStencilView(IDepthStencilSurface* depthStencilSurface, f32 depth, u8 stencil) = 0;
			
			// �����_�[�^�[�Q�b�g�ݒ�
			virtual void SetRenderTargets(u32 count, IRenderTargetSurface* renderTargetSurface[], IDepthStencilSurface* depthStencilSurface) = 0;
			
			// �����_�[�^�[�Q�b�g�擾
			virtual void GetRenderTargets(u32 count, IRenderTargetSurface* renderTargetSurface[], IDepthStencilSurface* depthStencilSurface) = 0;
			
			// �r���[�|�[�g�ݒ�
			virtual void SetViewports(u32 numViews, const Viewport* viewports) = 0;
			
			// �r���[�|�[�g�擾
			virtual void GetViewports(u32 count, Viewport* viewports[]) = 0;
		};
	} // namespace Graphics
} // namespace Phoenix