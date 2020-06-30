#pragma once

#include "Phoenix/FND/Base.h"
#include "Phoenix/FrameWork/RenderableCollection.h"
#include "Phoenix/FrameWork/Object.h"


namespace Phoenix
{
	namespace FrameWork
	{
		// ��s�錾
		class IGame;
		class GameObject;

		//****************************************************************************
		// �R���|�[�l���g
		//****************************************************************************
		class Component : public FND::Base
		{
		private:
			friend class GameObject;

			std::weak_ptr<GameObject> owner;
			bool enable = true;
			bool isStarted = false;

		public:
			Component() {}
			~Component() override {}

			// ���O�擾
			virtual const char* GetName() = 0;

			// �q�Q�[���I�u�W�F�N�g���擾
			virtual void  GetChildren(std::vector<IObject*>& objects) {}

			// �������Ɉ�x�����Ă΂��
			virtual void Awake(IGame* game) {}

			// �`��m�[�h�����W
			virtual void GetRenderables(IGame* game, RenderableCollection& collection, bool force) {}

			// �s��X�V����
			virtual void UpdateTransform(IGame* game) {}

			// �J�n����
			virtual void Start(IGame* game) {}

			// �t���[�����̍X�V����
			virtual void Update(IGame* game) {}

			// �L����Ԃ�ݒ�
			void SetEnable(bool enable) { this->enable = enable; }

			// �L����Ԃ��擾
			bool IsEnable() const { return enable; }

			// �I�[�i�[���擾
			std::shared_ptr<GameObject> GetOwner() { return owner.lock(); }

			// �j�����ɌĂ΂��
			virtual void OnDestroy(IGame* game) {}

			// �V�[��GUI�\��
			virtual void OnSceneGUI(IGame* game) {}

			// �C���X�y�N�^�[GUI�\��
			//virtual void OnInspectorGUI(IGame* game);

		protected:
			// �\������
			bool IsVisible(IGame* game);

		private:
			// �I�[�i�[�ݒ�
			void SetOwner(std::shared_ptr<GameObject> gameObject) { owner = gameObject; }

			// �X�^�[�g�ݒ�
			bool IsStarted() const { return isStarted; }
			void SetStart() { isStarted = true; }

		public:
			template<class Archive>
			void serialize(Archive& archive, u32 version);
		};
	} // namespace FrameWork
} // namespace Phoenix