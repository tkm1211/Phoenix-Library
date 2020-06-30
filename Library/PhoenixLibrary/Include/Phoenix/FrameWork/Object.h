#pragma once

#include <vector>
#include "Phoenix/FND/Flag.h"
#include "Phoenix/Math/AABB.h"
#include "Phoenix/Math/Vector.h"
#include "Phoenix/Math/Matrix.h"
#include "Phoenix/Math/Quaternion.h"


namespace Phoenix
{
	namespace FrameWork
	{
		// �O���錾
		class IGame;

		//****************************************************************************
		// �g�����X�t�H�[���t���O
		//****************************************************************************
		struct TransformationFlag : public FND::Flag32
		{
			static const u32 None = 0;				// �Ȃ�
			static const u32 Translate = (1 << 0);	// �ړ�
			static const u32 Rotate = (1 << 1);		// ��]F
			static const u32 Scale = (1 << 2);		// �X�P�[��
			static const u32 All = 0xFFFFFFFF;

			FLAG32_CONSTRUCT(TransformationFlag)
		};

		//****************************************************************************
		// �g�����X�t�H�[������C���^�[�t�F�[�X
		//****************************************************************************
		class ITransform
		{
		public:
			// �X�P�[���l��ݒ�
			virtual void SetScale(const Math::Vector3& scale) = 0;

			// ��]�l��ݒ�
			virtual void SetRotate(const Math::Quaternion& rotate) = 0;

			// �ړ��l��ݒ�
			virtual void SetTranslate(const Math::Vector3& translate) = 0;

			// �s�{�b�g�l��ݒ�
			//virtual void SetPivot(const math::Vector3& pivot) = 0;

			// �X�P�[���l���擾
			virtual const Math::Vector3& GetScale() = 0;

			// ��]�l���擾
			virtual const Math::Quaternion& GetRotate() = 0;

			// �ړ��l���擾
			virtual const Math::Vector3& GetTranslate() = 0;

			// ���[�J���g�����X�t�H�[�����擾
			virtual const Math::Matrix& GetLocalTransform() = 0;

			// ���[���h�g�����X�t�H�[�����擾
			virtual const Math::Matrix& GetWorldTransform() = 0;

			// �g�����X�t�H�[���t���O���擾
			virtual TransformationFlag GetTransformationFlag() = 0;
		};

		//****************************************************************************
		// �I�u�W�F�N�g����C���^�[�t�F�[�X
		//****************************************************************************
		class IObject : public ITransform
		{
		public:
			// ���O�ݒ�
			virtual void SetName(const char* name) = 0;

			// ���O�擾
			virtual const char* GetName() = 0;

			// �A�N�e�B�u��Ԃ�ݒ�
			virtual void SetActive(bool active) = 0;

			// �A�N�e�B�u��Ԃ��擾
			virtual bool IsActive() = 0;

			// �e�Q�[���I�u�W�F�N�g���擾
			virtual IObject* GetParent() = 0;

			// �q�Q�[���I�u�W�F�N�g���擾
			virtual const std::vector<IObject*>& GetChildren() = 0;

			// �q��ǉ��ł��邩����
			virtual bool CanAddChild(IObject* child) = 0;

			// �q��ǉ�
			virtual bool AddChild(IObject* child) = 0;

			// �o�E���f�B���O�{�b�N�X���擾
			virtual const Math::AABB& GetBounds() = 0;
		};

	}	// namespace fwk
}	// namespace ys
