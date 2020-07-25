#pragma once

#include <d3d11.h>
#include "Phoenix/Graphics/Surface.h"


namespace Phoenix
{
	namespace Graphics
	{
		class RenderTargetSurfaceDX11 final : public IRenderTargetSurface
		{
		private:
			ID3D11RenderTargetView* renderTargetView = nullptr;
			std::unique_ptr<ITexture> texture = nullptr;

		public:
			RenderTargetSurfaceDX11() {}
			~RenderTargetSurfaceDX11() override { Finalize(); }

		public:
			// ������
			bool Initialize(ID3D11Device* d3dDevice, ID3D11Texture2D* d3dTexture2D, ID3D11ShaderResourceView* d3dShaderResourceView);

			// ������
			bool Initialize(ID3D11Device* d3dDevice, const D3D11_TEXTURE2D_DESC& texDesc, const D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, const D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc);
			
			// ������
			bool Initialize(IDevice* device, const RenderTargetSurfaceDesc& desc) override;
			
			// �I����
			void Finalize() override;

			// �e�N�X�`���擾
			ITexture* GetTexture() override { return texture.get(); }

			// D3D�����_�[�^�[�Q�b�g�r���[�ݒ�
			void SetD3DRenderTargetView(ID3D11RenderTargetView* d3dRenderTargetView) { renderTargetView = d3dRenderTargetView; }

			// D3D�����_�[�^�[�Q�b�g�r���[�擾
			ID3D11RenderTargetView* GetD3DRenderTargetView() const { return renderTargetView; }
		};
	} // namespace Graphics
} // namespace Phoenix