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
			std::unique_ptr<ITexture> texture = nullptr;

		public:
			DepthStencilSurfaceDX11() {}
			~DepthStencilSurfaceDX11() override { Finalize(); }

		public:
			// ������
			bool Initialize(ID3D11Device* d3dDevice, const D3D11_TEXTURE2D_DESC& texDesc, const D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc, const D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc);

			// ������
			bool Initialize(IDevice* device, const DepthStencilSurfaceDesc& desc) override;

			// �I����
			void Finalize() override;

			// �e�N�X�`���擾
			ITexture* GetTexture() override { return texture.get(); }

			// D3D�[�x�X�e���V���r���[�ݒ�
			void SetDepthStencilView(ID3D11DepthStencilView* d3dDepthStencilView) { depthStencilView = d3dDepthStencilView; }
			
			// D3D�[�x�X�e���V���r���[�擾
			ID3D11DepthStencilView* GetD3DDepthStencilView() const { return depthStencilView; }
		};
	} // namespace Graphics
} // namespace Phoenix