#pragma once

#include <d3d11.h>


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// �u�����h�X�e�[�g
		//****************************************************************************
		enum class BlendState
		{
			Opaque,					// �W���u�����h
			AlphaBlend,				// �A���t�@�u�����h
			Additive,				// ���Z�u�����h
			Subtraction,			// ���Z�u�����h
			Multiply,				// ��Z�u�����h
			AlphaToCoverageEnable,	// �A���t�@����

			TypeNum
		};

		//****************************************************************************
		// �[�x�X�e�[�g
		//****************************************************************************
		enum class DepthState
		{
			TestAndWrite,		// �[�x�e�X�g���[�x��������
			TestOnly,			// �[�x�e�X�g�̂�
			WriteOnly,			// �[�x�������݂̂�
			NoTestNoWrite,		// �[�x�e�X�g���[�x�������݂Ȃ�

			TypeNum
		};

		//****************************************************************************
		// ���X�^���C�U�X�e�[�g
		//****************************************************************************
		enum class RasterizerState
		{
			SolidCullNone,		// �O�p�`�h��Ԃ��J�����O�Ȃ�
			SolidCullBack,		// �O�p�`�h��Ԃ����ʃJ�����O
			SolidCullFront,		// �O�p�`�h��Ԃ��\�ʃJ�����O
			WireCullNone,		// ���C���[�t���[���J�����O�Ȃ�
			WireCullBack,		// ���C���[�t���[�����ʃJ�����O
			WireCullFront,		// ���C���[�t���[���\�ʃJ�����O

			TypeNum
		};

		//****************************************************************************
		// �T���v���X�e�[�g
		//****************************************************************************
		enum class SamplerState
		{
			PointWrap,
			PointClamp,
			PointBorder,
			LinearWrap,
			LinearClamp,
			LinearBorder,
			AnisotropicWrap,
			AnisotropicClamp,
			AnisotropicBorder,

			TypeNum
		};

		//****************************************************************************
		// DirectX11�Ń����_�[�X�e�[�g���[�e�B���e�B
		//****************************************************************************
		class RenderStateUtillityDX11
		{
		public:
			// D3D�u�����h�L�q�쐬
			static void MakeD3DBlendDesc(BlendState blendState, D3D11_BLEND_DESC& desc);

			// D3D�[�x�L�q�쐬
			static void MakeD3DDepthDesc(DepthState depthState, D3D11_DEPTH_STENCIL_DESC& desc);

			// D3D���X�^���C�U�L�q�쐬
			static void MakeD3DRasterizerDesc(RasterizerState rasterizerState, D3D11_RASTERIZER_DESC& desc, bool enableCull = true, bool enableDepth = true, bool enableMultisample = false, bool enableScissor = false);

			// D3D�T���v���L�q�쐬
			static void MakeD3DSamplerDesc(SamplerState samplerState, D3D11_SAMPLER_DESC& desc, bool enableAlways = true, bool enableComparison = false);
		};
	} // namespace Graphics
} // namespace Phoenix