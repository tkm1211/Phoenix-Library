#pragma once

#include "Phoenix/Graphics/Mesh.h"


namespace Phoenix
{
	namespace Graphics
	{
		//****************************************************************************
		// ���b�V������I�u�W�F�N�g���N���X
		//****************************************************************************
		class Mesh : public IMesh
		{
		private:
			MeshDesc desc;
			Math::AABB bounds;

		protected:
			Mesh() {}
			~Mesh() override {}

		public:
			// �o�E���f�B���O�{�b�N�X�擾
			const Math::AABB& GetBounds() override { return bounds; }

			// ���b�V���ݒ�L�q�擾
			MeshDesc& GetDesc() override { return desc; }

			// �o�E���f�B���O�{�b�N�X���v�Z
			void ComputeBounds() override;

			// �o�E���f�B���O�{�b�N�X�v�Z
			void ComputeBounds(Math::AABB& bounds, const Math::Matrix boneTransforms[]) override;

			// �X�L�j���O���_�v�Z
			void ComputeSkinnedVertices(Math::Vector3 vertices[], const Math::Matrix boneTransforms[]) override;

		protected:
			// ��ꏉ����
			void BaseInitialize(const MeshDesc& desc)
			{
				this->desc = desc;
				ComputeBounds();
			}
		};

	}	// namespace grp
}	// namespace ys