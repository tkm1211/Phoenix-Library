#pragma once

#include <d3d11.h>
#include "Phoenix/Graphics/RenderState.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11�ŃT���v���[�X�e�[�g����I�u�W�F�N�g
		//****************************************************************************
		class SamplerDX11 final : public ISampler
		{
		private:
			ID3D11SamplerState* samplerState = nullptr;

		public:
			// ������
			bool Initialize(IDevice* device, SamplerState state, bool enableAlways = true, bool enableComparison = false) override;

			// �I����
			void Finalize() override;

			// D3D�T���v���[�X�e�[�g�擾
			ID3D11SamplerState* GetD3DSamplerState() const { return samplerState; }
		};
	} // namespace Graphics
} // namespace Phoenix