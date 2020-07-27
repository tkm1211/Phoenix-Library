#pragma once

#include <d3d11.h>
#include "Phoenix/Graphics/RenderState.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11�Ńu�����h����I�u�W�F�N�g
		//****************************************************************************
		class BlendDX11 final : public IBlend
		{
		private:
			ID3D11BlendState* blendState = nullptr;

		public:
			// ������
			bool Initialize(IDevice* device, BlendState state) override;

			// �I����
			void Finalize() override;

			// D3D�u�����h�X�e�[�g�擾
			ID3D11BlendState* GetD3DBlendState() { return blendState; }
		};
	} // namespace Graphics
} // namespace Phoenix