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
			ID3D11ShaderResourceView* shaderResourceView = nullptr;
			ID3D11Texture2D* texture2D = nullptr;

		public:
			RenderTargetSurfaceDX11() {}
			~RenderTargetSurfaceDX11() override { Finalize(); }

		public:
			// ������
			bool Initialize(ID3D11Device* d3dDevice, ID3D11Texture2D* d3dTexture2D, ID3D11ShaderResourceView* d3dShaderResourceView);
			
			// ������
			bool Initialize(IDevice* device, const RenderTargetSurfaceDesc& desc) override;
			
			// �I����
			void Finalize() override;

			// D3D�����_�[�^�[�Q�b�g�r���[�ݒ�
			void SetD3DRenderTargetView(ID3D11RenderTargetView* d3dRenderTargetView) { renderTargetView = d3dRenderTargetView; }

			// D3D�����_�[�^�[�Q�b�g�r���[�擾
			ID3D11RenderTargetView* GetD3DRenderTargetView() const { return renderTargetView; }

			// D3D�V�F�[�_�[���\�[�X�r���[�擾
			ID3D11ShaderResourceView* GetD3DShaderResourceView() const { return shaderResourceView; }

			// D3D�e�N�X�`��2D�擾
			ID3D11Texture2D* GetD3DTexture2D() const { return texture2D; }
		};
	} // namespace Graphics
} // namespace Phoenix