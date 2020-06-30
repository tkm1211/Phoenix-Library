#pragma once

#include "Phoenix/FND/NonCopyable.h"
#include "Phoenix/FND/Flag.h"
#include "Phoenix/Math/Color.h"
#include "Phoenix/Graphics/Material.h"


namespace Phoenix
{
	namespace FrameWork
	{
		//****************************************************************************
		// �\���t���O���[�h
		//****************************************************************************
		enum class DisplayFlagModes
		{
			None,		// �\�����Ȃ�
			Always,		// ��ɕ\��
			Selection,	// �I�����ꂽ�I�u�W�F�N�g�̂ݕ\��
		};

		//****************************************************************************
		// �����_�[�r���[
		//****************************************************************************
		enum class RenderView
		{
			Scene,
			Game
		};

		//****************************************************************************
		// �O���[�o���`��؂�ւ��𔻒肷��t���O
		//****************************************************************************
		struct GlobalRenderFlag : public FND::Flag32
		{
			static const u32 None = 0;						// �Ȃ�
			static const u32 Textured = (1 << 0);			// �e�N�X�`���\����ON/OFF
			static const u32 Lit = (1 << 1);				// ���C�e�B���O������ON/OFF
			static const u32 RenderBackFace = (1 << 2);		// �w�ʕ\����ON/OFF
			static const u32 Solid = (1 << 3);				// �\���b�h�\����ON/OFF
			static const u32 WireFrame = (1 << 4);			// ���C���[�t���[���\����ON/OFF
			static const u32 Shadows = (1 << 5);			// �e�\����ON/OFF
			static const u32 RenderNormals = (1 << 6);		// �@���\����ON/OFF
			static const u32 Collision = (1 << 7);			// �R���W�����\����ON/OFF
			static const u32 All = 0xFFFFFFFF;				// ���\�[�X���}�b�v�\�ɂ��ACPU �����̃��\�[�X�̓��e��ǂݎ���悤�ɂ��܂��B

			FLAG32_CONSTRUCT(GlobalRenderFlag)
		};

		//****************************************************************************
		// �J�����O�}�X�N�t���O
		//****************************************************************************
		struct CullingMaskFlag : public FND::Flag32
		{
			static const u32 Nothing = 0;
			static const u32 Everything = 0xFFFFFFFF;

			FLAG32_CONSTRUCT(CullingMaskFlag)
		};

		//****************************************************************************
		// �����_�[�X�e�[�g����I�u�W�F�N�g
		//****************************************************************************
		class RenderState final : public FND::NonCopyable
		{
		private:
			union
			{
				u32 globalRenderFlags
					= GlobalRenderFlag::Solid
					| GlobalRenderFlag::Textured
					| GlobalRenderFlag::Lit
					| GlobalRenderFlag::Shadows;
				GlobalRenderFlag globalRenderFlag;
			};
			union
			{
				u32 cullingMaskFlags = CullingMaskFlag::Everything;
				CullingMaskFlag cullingMaskFlag;
			};

			RenderView renderView = RenderView::Scene;

			Graphics::BasicMaterial selectionMaterial;
			Graphics::BasicMaterial wireframeMaterial;
			Graphics::BasicMaterial collisionMaterial;
			Graphics::BasicMaterial defaultMaterial;

			DisplayFlagModes displayBounds = DisplayFlagModes::Selection;
			DisplayFlagModes displayBones = DisplayFlagModes::Selection;

		public:
			RenderState()
			{
				SetSelectionColor(Math::Color(0.25f, 1.00f, 0.63f, 1.00f));
				SetWireframeColor(Math::Color(0.00f, 0.00f, 0.55f, 1.00f));
				SetCollisionColor(Math::Color(0.70f, 0.00f, 0.00f, 1.00f));
			}

			// �O���[�o���`��؂�ւ��t���O�擾
			GlobalRenderFlag GetGlobalRenderFlags() const { return globalRenderFlags; }

			// �O���[�o���`��؂�ւ��t���O��ݒ�
			void SetGlobalRenderFlags(GlobalRenderFlag flags) { globalRenderFlags = flags; }

			// �J�����O�}�X�N�t���O�擾
			CullingMaskFlag GetCullingMaskFlags() const { return cullingMaskFlags; }

			// �J�����O�}�X�N�t���O��ݒ�
			void SetCullingMaskFlags(CullingMaskFlag flags) { cullingMaskFlags = flags; }

			// �����_�[�r���[�擾
			RenderView GetRenderView() const { return renderView; }

			// �����_�[�r���[�ݒ�
			void SetRenderView(RenderView render_view) { render_view = render_view; }

			// �I��F��ݒ�
			void SetSelectionColor(const Math::Color& color) { selectionMaterial.color = color; }

			// ���C���[�t���[���F��ݒ�
			void SetWireframeColor(const Math::Color& color) { wireframeMaterial.color = color; }

			// �R���W�����F��ݒ�
			void SetCollisionColor(const Math::Color& color) { collisionMaterial.color = color; }

			// �I���}�e���A�����擾
			const Graphics::BasicMaterial* GetSelectionMaterial() const { return &selectionMaterial; }

			// ���C���[�t���[���}�e���A�����擾
			const Graphics::BasicMaterial* GetWireframeMaterial() const { return &wireframeMaterial; }

			// �R���W�����}�e���A�����擾
			const Graphics::BasicMaterial* GetCollisionMaterial() const { return &collisionMaterial; }

			// �f�t�H���g�}�e���A�����擾
			const Graphics::BasicMaterial* GetDefaultMaterial() const { return &defaultMaterial; }

			// �o�E���f�B���O�{�b�N�X�\�����[�h�擾
			DisplayFlagModes GetDisplayBounds() const { return displayBounds; }

			// �o�E���f�B���O�{�b�N�X�\�����[�h�ݒ�
			void SetDisplayBounds(DisplayFlagModes mode) { displayBounds = mode; }

			// �{�[���\�����[�h�擾
			DisplayFlagModes GetDisplayBones() const { return displayBones; }

			// �{�[���\�����[�h�ݒ�
			void SetDisplayBones(DisplayFlagModes mode) { displayBones = mode; }

		public:
			template<class Archive>
			void serialize(Archive& archive, u32 version);
		};
	} // namespace FrameWork
} // namespace Phoenix