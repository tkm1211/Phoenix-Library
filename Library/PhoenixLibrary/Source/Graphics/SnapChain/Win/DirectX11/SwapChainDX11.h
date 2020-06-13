#pragma once

#include <d3d11.h>
#include <vector>

#include "Phoenix/Graphics/SwapChain.h"
#include "Phoenix/Graphics/Surface.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11�ŃX���b�v�`�F�[������I�u�W�F�N�g
		//****************************************************************************
		class SwapChainDX11 final : public ISwapChain
		{
		private:
			IDXGISwapChain* swapChain = nullptr;
			std::unique_ptr<IRenderTargetSurface> renderTargerSurface;
			std::unique_ptr<IDepthStencilSurface> depthStencilSurface;

		public:
			SwapChainDX11() {}
			~SwapChainDX11() override { Finalize(); }

		public:
			// ������
			bool Initialize(IDevice* device, const SwapChainDesc& desc) override;

			// �I����
			void Finalize() override;

			// ��ʕ\��
			void Present(int syncInterval) override;

			// �����_�[�^�[�Q�b�g�T�[�t�F�C�X�擾
			IRenderTargetSurface* GetRenderTargerSurface() override { return renderTargerSurface.get(); }

			// �[�x�X�e���V���T�[�t�F�C�X�擾
			IDepthStencilSurface* GetDepthStencilSurface() override { return depthStencilSurface.get(); }

			// �X���b�v�`�F�[���擾
			IDXGISwapChain* GetDXGISwapChain() const { return swapChain; }

		private:
			// �X���b�v�`�F�[���쐬
			bool CreateSwapChain(ID3D11Device* device, IDXGIFactory* dxgiFactory, HWND hwnd, unsigned int width, unsigned int height, bool enableMsaa = false, int subSamples = 1);
			
			// �o�b�N�o�b�t�@�쐬
			bool CreateBackBuffer(IDevice* device, unsigned int width, unsigned int height);
		};
	} // namespace Graphics
} // namespace Phoenix