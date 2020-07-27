#pragma once

#include <d3d11.h>
#include "Phoenix/Graphics/RenderState.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11�Ő[�x�X�e���V���X�e�[�g����I�u�W�F�N�g
		//****************************************************************************
		class DepthStencilDX11 final : public IDepthStencil
		{
		private:
			ID3D11DepthStencilState* depthStencilState = nullptr;

		public:
			// ������
			bool Initialize(IDevice* device, DepthState state) override;

			// �I����
			void Finalize() override;

			// D3D�u�����h�X�e�[�g�ݒ�
			void SetD3DDepthStencilState(ID3D11DepthStencilState* depthStencilState) { this->depthStencilState = depthStencilState; }

			// D3D�u�����h�X�e�[�g�擾
			ID3D11DepthStencilState* GetD3DDepthStencilState() const { return depthStencilState; }
		};
	} // namespace Graphics
} // namespace Phoenix