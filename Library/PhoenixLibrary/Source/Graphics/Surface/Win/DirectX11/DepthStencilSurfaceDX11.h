#pragma once

#include <d3d11.h>
#include "Phoenix/Graphics/Surface.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11�Ő[�x�X�e���V���T�[�t�F�C�X����I�u�W�F�N�g
		//****************************************************************************
		class DepthStencilSurfaceDX11 final : public IDepthStencilSurface
		{
		private:
			ID3D11DepthStencilView* depthStencilView = nullptr;
			ID3D11Texture2D* texture2D = nullptr;

		public:
			DepthStencilSurfaceDX11() {}
			~DepthStencilSurfaceDX11() override { Finalize(); }

		public:
			// ������
			bool Initialize(IDevice* device, const DepthStencilSurfaceDesc& desc) override;

			// �I����
			void Finalize() override;

			// D3D�[�x�X�e���V���r���[�ݒ�
			void SetDepthStencilView(ID3D11DepthStencilView* d3dDepthStencilView) { depthStencilView = d3dDepthStencilView; }
			
			// D3D�[�x�X�e���V���r���[�擾
			ID3D11DepthStencilView* GetD3DDepthStencilView() const { return depthStencilView; }
			
			// D3D�e�N�X�`��2D�擾
			ID3D11Texture2D* GetD3DTexture2D() const { return texture2D; }
		};
	} // namespace Graphics
} // namespace Phoenix