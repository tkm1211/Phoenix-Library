#pragma once
// TODO : Add HIT 
#include <vector>
#include "Phoenix/FND/NonCopyable.h"
//#include "Phoenix/HIT/Manager.h"


namespace Phoenix
{
	namespace FrameWork
	{
		// �O���錾
		class MeshCollider;
		class SphereCollider;

		//****************************************************************************
		// �R���W�����}�l�[�W���[
		//****************************************************************************
		class CollisionManager final : public FND::NonCopyable
		{
		private:
			//std::unique_ptr<hit::IManager> manager;
			std::vector<SphereCollider*> sphereColliders;
			std::vector<MeshCollider*> meshColliders;

		public:
			// ������
			bool Initialize();

			// �I����
			void Finalize();

			// �X�t�B�A�R���C�_�[�o�^
			void RegisterSphereCollider(SphereCollider* collider);

			// �X�t�B�A�R���C�_�[�o�^����
			void UnregisterSphereCollider(SphereCollider* collider);

			// ���b�V���R���C�_�[�o�^
			void RegisterMeshCollider(MeshCollider* collider);

			// ���b�V���R���C�_�[�o�^����
			void UnregisterMeshCollider(MeshCollider* collider);

			// �R���C�_�[�o�^����
			void Clear();

			// �X�t�B�A�R���C�_�[���X�g�擾
			const std::vector<SphereCollider*>& GetSphereColliders() const { return sphereColliders; }

			// ���b�V���R���C�_�[���X�g�擾
			const std::vector<MeshCollider*>& GetMeshColliders() const { return meshColliders; }

			// �X�V
			void Update();
		};
	} // namespace FrameWork
} // namespace Phoenix