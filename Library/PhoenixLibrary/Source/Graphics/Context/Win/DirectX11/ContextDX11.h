#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d11.h>
#include <assert.h>

#include "Phoenix/Graphics/Context.h"
#include "Phoenix/Graphics/Surface.h"
#include "Phoenix/Graphics/Viewport.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11�ŃR���e�L�X�g����I�u�W�F�N�g
		//****************************************************************************
		class ContextDX11 final : public IContext
		{
		private:
			ID3D11DeviceContext* deviceContext = nullptr;

		public:
			ContextDX11() {}
			~ContextDX11() override { Finalize(); }

		public:
			// ������
			bool Initialize(IDevice* device) override;

			// �I����
			void Finalize() override;

			// �����_�[�^�[�Q�b�g�r���[�N���A
			void ClearRenderTargetView(IRenderTargetSurface* renderTargetSurface, const f32* color) override;
			
			// �[�x�X�e���V���r���[�N���A
			void ClearDepthStencilView(IDepthStencilSurface* depthStencilSurface, f32 depth, u8 stencil) override;
			
			// �����_�[�^�[�Q�b�g�ݒ�
			void SetRenderTargets(u32 count, IRenderTargetSurface* renderTargetSurface[], IDepthStencilSurface* depthStencilSurface) override;
			
			// �����_�[�^�[�Q�b�g�擾
			void GetRenderTargets(u32 count, IRenderTargetSurface* renderTargetSurface[], IDepthStencilSurface* depthStencilSurface) override;
			
			// �r���[�|�[�g�ݒ�
			void SetViewports(u32 count, const Viewport viewports[]) override;
			
			// �r���[�|�[�g�擾
			void GetViewports(u32 count, Viewport* viewports[]) override;
		};
	} // namespace Graphics
} // namespace Phoenix