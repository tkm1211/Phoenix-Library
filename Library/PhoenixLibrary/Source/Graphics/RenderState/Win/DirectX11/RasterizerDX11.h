#pragma once

#include <d3d11.h>
#include "Phoenix/Graphics/RenderState.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11�Ń��X�^���C�U�X�e�[�g����I�u�W�F�N�g
		//****************************************************************************
		class RasterizerDX11 final : public IRasterizer
		{
		private:
			ID3D11RasterizerState* rasterizerState = nullptr;

		public:
			// ������
			bool Initialize(IDevice* device, RasterizerState state) override;

			// �I����
			void Finalize() override;

			// D3D���X�^���C�U�X�e�[�g�擾
			ID3D11RasterizerState* GetD3DRasterizerState() const { return rasterizerState; }
		};
	} // namespace Graphics
} // namespace Phoenix