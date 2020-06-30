#pragma once

#include <memory>
#include <string>
#include "Phoenix/Math/PhoenixMath.h"
#include "Phoenix/FrameWork/Object.h"
#include "Phoenix/FrameWork/Component.h"
#include "Phoenix/FrameWork/FrameTime.h"


namespace Phoenix
{
	namespace FrameWork
	{
		class GameObject : public std::enable_shared_from_this<GameObject>,
			public IObject
		{
		private:
			using super = FND::Base;

			std::weak_ptr<GameObject> parent;
			std::vector<std::shared_ptr<GameObject>> children;
			std::vector<std::shared_ptr<Component>> components;

			std::vector<IObject*> objects;	// ���[�N�p

			std::string name;

			Math::Vector3 translate = Math::Vector3::Zero;
			Math::Quaternion rotate = Math::Quaternion::Zero;
			Math::Vector3 scale = Math::Vector3::OneAll;
			Math::Vector3 pivot = Math::Vector3::Zero;
			Math::Matrix localTransform = Math::Matrix::One;
			Math::Matrix worldTransform = Math::Matrix::One;
			Math::AABB localBounds = Math::AABB::Zero;
			Math::AABB worldBounds = Math::AABB::Zero;

			u32 layer = 0;

			bool active = true;
			bool boundsDirty = false;
			bool boundsUpdated = false;
			bool localTransformDirty = false;
			bool worldTransformDirty = false;
			bool worldTransformUpdated = false;

		public:
			GameObject();
			virtual ~GameObject();

		private:
			GameObject(const GameObject&);
			const GameObject& operator = (const GameObject&);

		public:
			static std::shared_ptr<GameObject> Create();

			// ���O��ݒ�
			void SetName(const char* name) override { this->name = name; }

			// ���O���擾
			const char* GetName() override { return this->name.c_str(); }

			// �A�N�e�B�u��Ԃ�ݒ�
			void SetActive(bool active) override { this->active = active; }

			// �A�N�e�B�u��Ԃ��擾
			bool IsActive() override { return this->active; }

			// �e�Q�[���I�u�W�F�N�g���擾
			IObject* GetParent() override { return this->parent.lock().get(); }

			// �q�Q�[���I�u�W�F�N�g���擾
			const std::vector<IObject*>& GetChildren() override;

			// �q��ǉ��ł��邩����
			bool CanAddChild(IObject* child) override;

			// �q��ǉ�
			bool AddChild(IObject* child) override;

			// �o�E���f�B���O�{�b�N�X���擾
			const Math::AABB& GetBounds() override { return GetWorldBounds(); }

			// �X�P�[���l��ݒ�
			void SetScale(const Math::Vector3& scale) override { this->scale = scale; InvalidateLocalTransform(); }

			// ��]�l��ݒ�
			void SetRotate(const Math::Quaternion& rotate) override { this->rotate = rotate; InvalidateLocalTransform(); }

			// �ړ��l��ݒ�
			void SetTranslate(const Math::Vector3& translate) override { this->translate = translate; InvalidateLocalTransform(); }

			// �X�P�[���l���擾
			const Math::Vector3& GetScale() override { return scale; }

			// ��]�l���擾
			const Math::Quaternion& GetRotate() override { return rotate; }

			// �ړ��l���擾
			const Math::Vector3& GetTranslate() override { return translate; }

			// ���[�J���g�����X�t�H�[�����擾
			const Math::Matrix& GetLocalTransform() override { return localTransform; }

			// ���[���h�g�����X�t�H�[�����擾
			const Math::Matrix& GetWorldTransform() override { return worldTransform; }

			// �g�����X�t�H�[���t���O���擾
			TransformationFlag GetTransformationFlag() override { return TransformationFlag::All; }

			// �`��m�[�h�����W
			void GetRenderables(IGame* game, RenderableCollection& collection);

			// �s��X�V����
			void UpdateTransform(IGame* game);

			// �t���[�����̍X�V����
			void Update(IGame* game, UpdateType update_type);

			// ���C���[���擾
			u32 GetLayer() const { return layer; }

			// ���[�J���o�E���f�B���O�{�b�N�X���擾
			const Math::AABB& GetLocalBounds() const { return localBounds; }

			// ���[���h�o�E���f�B���O�{�b�N�X���擾
			const Math::AABB& GetWorldBounds() const { return worldBounds; }

			// ���[�J���g�����X�t�H�[����ݒ�
			void SetLocalTransform(const Math::Matrix& transform);

			// ���[�J���o�E���f�B���O�{�b�N�X���g��
			void ExtendLocalBounds(const Math::AABB& bounds);

			// ���[���h�o�E���f�B���O�{�b�N�X���v�Z
			void ComputeWorldBounds(Math::AABB& bounds);

			// �e�Q�[���I�u�W�F�N�g��ݒ�
			void SetParent(std::shared_ptr<GameObject> parent);

			// �q�Q�[���I�u�W�F�N�g��ǉ�
			void AddChild(std::shared_ptr<GameObject> child, s32 index = -1);

			// �q�Q�[���I�u�W�F�N�g���폜
			void RemoveChild(std::shared_ptr<GameObject> child);

			// �e����Q�[���I�u�W�F�N�g���폜
			void RemoveFromParent();

			// �q�Q�[���I�u�W�F�N�g���N���A
			void ClearChildren();

			// �R���|�[�l���g��ǉ�
			void AddComponent(std::shared_ptr<Component> component, IGame* game, s32 index = -1);

			// �R���|�[�l���g���폜
			void RemoveComponent(std::shared_ptr<Component> component, IGame* game);

			// �R���|�[�l���g���N���A
			void ClearComponents();

			// ���[�J���g�����X�t�H�[�����X�V
			void UpdateLocalTransform();

			// ���[���h�g�����X�t�H�[�����X�V
			void UpdateWorldTransform();

			// ���[���h�o�E���f�B���O�{�b�N�X
			void UpdateWorldBounds();

			// ���[�J���g�����X�t�H�[���̍X�V��v��
			void InvalidateLocalTransform();

			// ���[���h�g�����X�t�H�[���̍X�V��v��
			void InvalidateWorldTransform();

			// ���[���h�o�E���f�B���O�{�b�N�X�̍X�V��v��
			void InvalidateWorldBounds();

			// �Z�b�g�A�b�v
			virtual void Setup(IGame* game);

			// �C���X�^���X������̉�������
			virtual void Resolve();

			// �j��
			virtual void Destroy(IGame* game);

			// �w��̌^�̃R���|�[�l���g���擾
			template<class T>
			std::shared_ptr<T> GetComponent()
			{
				for (auto&& component : components)
				{
					auto obj = std::dynamic_pointer_cast<T>(component);
					if (obj != nullptr)
					{
						return obj;
					}
				}
				return nullptr;
			}

			// �w��̌^�̃R���|�[�l���g��S�Ď擾
			template<class T>
			void GetComponents(std::vector<std::shared_ptr<T>>& components)
			{
				for (auto&& component : components)
				{
					auto obj = std::dynamic_pointer_cast<T>(component);
					if (obj != nullptr)
					{
						components.emplace_back(obj);
					}
				}
			}

			// �q�Q�[���I�u�W�F�N�g����w��̌^�̃R���|�[�l���g���擾
			template<class T>
			std::shared_ptr<T> GetComponentInChildren()
			{
				for (auto&& child : children)
				{
					auto component = child->GetComponent<T>();
					if (component != nullptr)
					{
						return component;
					}
				}

				for (auto&& child : children)
				{
					auto component = child->GetComponentInChildren<T>();
					if (component != nullptr)
					{
						return component;
					}
				}

				return nullptr;
			}


			// �w��̌^�̃R���|�[�l���g��ǉ�
			template<class T, class... Args>
			std::shared_ptr<T> AddComponent(IGame* game, Args... args)
			{
				std::shared_ptr<T> component = std::make_shared<T>(args...);
				AddComponent(component, game, -1);
				return component;
			}
		};
	}
}