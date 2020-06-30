#pragma once

#include "Phoenix/FND/Flag.h"
#include "Phoenix/Math/AABB.h"
#include "Phoenix/Math/Color.h"
#include "Phoenix/Math/Matrix.h"
#include "Phoenix/Graphics/Mesh.h"
#include "Phoenix/Graphics/Material.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// �`��m�[�h����t���O
		//****************************************************************************
		struct RenderableNodeFlag : public FND::Flag32
		{
			static const u32 None = 0;
			static const u32 ShadowCaster = (1 << 0);		// �e�𗎂Ƃ��B
			static const u32 TestAgainstBBoxOnly = (1 << 1);	// �q�b�g�e�X�g���o�E���f�B���O�{�b�N�X�݂̂ɂ���B
			static const u32 NotPickable = (1 << 2);			// ���̃m�[�h�̓s�b�N�̑Ώۂɂ��Ȃ��B
			static const u32 Foreground = (1 << 3);			// �őO�ʂɕ\������B
			static const u32 Skinning = (1 << 4);			// �X�L�j���O����B

			FLAG32_CONSTRUCT(RenderableNodeFlag)
		};

		//****************************************************************************
		// �`��m�[�h
		//****************************************************************************
		struct RenderableNode
		{
			IMesh* mesh = nullptr;								// ���b�V��
			const Material* material = nullptr;					// �}�e���A��
			u32 startVertex = 0;								// �J�n���_
			u32 vertexCount = 0;								// ���_��
			Math::Matrix worldTransform = Math::Matrix::One;	// ���[���h�ϊ��s��
			const Math::Matrix* boneTransforms = nullptr;		// �{�[���ϊ��s��
			u32 boneTransformCount = 0;							// �{�[���ϊ��s��
			Math::AABB bounds = Math::AABB::Zero;				// �o�E���f�B���O�{�b�N�X
			RenderableNodeFlag flags = 0;						// �`��m�[�h����t���O
		};

		//****************************************************************************
		// �`��؂�ւ�����t���O
		//****************************************************************************
		struct RenderFlag : public FND::Flag32
		{
		public:
			static const u32 None = 0;
			static const u32 Textured = (1 << 0);			// �e�N�X�`���\�����܂��B
			static const u32 Lit = (1 << 1);				// ���C�e�B���O������ON/OFF
			static const u32 RenderBackFace = (1 << 2);		// �w�ʕ\�����܂��B
			static const u32 DisableDepthTest = (1 << 3);	// �[�x�e�X�g�����܂���B
			static const u32 DisableDepthWrite = (1 << 4);	// �[�x�������݂����܂���B
			static const u32 AlphaBlend = (1 << 5);			// �A���t�@�u�����h���܂��B
			static const u32 Skinning = (1 << 6);			// �X�L�j���O���܂��B
			static const u32 Foreground = (1 << 7);			// �őO�ʂɕ`�悵�܂��B
			static const u32 All = 0xFFFFFFFF;				// ���\�[�X���}�b�v�\�ɂ��ACPU �����̃��\�[�X�̓��e��ǂݎ���悤�ɂ��܂��B
							 
			FLAG32_CONSTRUCT(RenderFlag)
		};
	} // namespace Graphics
} // namespace Phoenix