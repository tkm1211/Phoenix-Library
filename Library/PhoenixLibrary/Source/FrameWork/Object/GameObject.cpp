#include "pch.h"
#include "Phoenix/FrameWork/Object/GameObject.h"


namespace Phoenix
{
	namespace FrameWork
	{
		GameObject::GameObject()
		{
		}

		GameObject::~GameObject()
		{
			ClearComponents();
			ClearChildren();
		}

		// ����
		std::shared_ptr<GameObject> GameObject::Create()
		{
			return std::make_shared<GameObject>();
		}

		// �q�Q�[���I�u�W�F�N�g���擾
		const std::vector<IObject*>& GameObject::GetChildren()
		{
			objects.clear();
			for (std::shared_ptr<GameObject>& child : children)
			{
				objects.push_back(child.get());
			}

			for (std::shared_ptr<Component>& component : components)
			{
				component->GetChildren(objects);
			}

			return objects;
		}

		// �q��ǉ��ł��邩����
		bool GameObject::CanAddChild(IObject* child)
		{
			GameObject* gameObject = dynamic_cast<GameObject*>(child);
			return gameObject != nullptr;
		}

		// �q��ǉ�
		bool GameObject::AddChild(IObject* child)
		{
			GameObject* gameObject = dynamic_cast<GameObject*>(child);
			if (gameObject != nullptr)
			{
				AddChild(gameObject->shared_from_this(), -1);
				return true;
			}

			return false;
		}

		// �`��m�[�h���W
		void GameObject::GetRenderables(IGame* game, RenderableCollection& collection)
		{
			// TODO : Draw
			/*if (!IsActive())
			{
				return;
			}

			if (game->GetRenderState()->GetCullingMaskFlags().Test(1 << layer))
			{
				for (auto&& component : m_components)
				{
					if (component->IsEnable())
					{
						component->GetRenderables(game, collection, false);
					}
				}
			}

			for (auto&& child : m_children)
			{
				child->GetRenderables(game, collection);
			}*/
		}

		// �s��X�V����
		void GameObject::UpdateTransform(IGame* game)
		{
			if (!IsActive())
			{
				return;
			}

			worldTransformUpdated = false;
			boundsUpdated = false;

			Math::AABB localBounds;
			localBounds.Reset();
			this->localBounds = localBounds;

			// �g�����X�t�H�[���X�V����
			UpdateLocalTransform();
			UpdateWorldTransform();

			// �R���|�[�l���g�X�V����
			for (auto&& component : components)
			{
				if (component->IsEnable())
				{
					component->UpdateTransform(game);
				}
			}

			if (this->localBounds.min == localBounds.min &&
				this->localBounds.max == localBounds.max)
			{
				this->localBounds = Math::AABB::Zero;
			}

			// �o�E���f�B���O�{�b�N�X�X�V����
			UpdateWorldBounds();

			// �q�Q�[���I�u�W�F�N�g�X�V����
			for (auto&& child : children)
			{
				child->UpdateTransform(game);
			}
		}

		// �X�V
		void GameObject::Update(IGame* game, UpdateType update_type)
		{
			if (!IsActive())
			{
				return;
			}

			// �R���|�[�l���g�X�V����
			for (auto&& component : components)
			{
				if (component->IsEnable())
				{
					if (!component->IsStarted())
					{
						component->SetStart();
						component->Start(game);
					}
					component->Update(game);
				}
			}

			// �q�Q�[���I�u�W�F�N�g�X�V����
			for (auto&& child : children)
			{
				child->Update(game, update_type);
			}
		}

		// ���[�J���g�����X�t�H�[���ݒ�
		void GameObject::SetLocalTransform(const Math::Matrix& transform)
		{
			localTransform = transform;
			InvalidateWorldTransform();
			InvalidateWorldBounds();
		}

		// ���[�J���o�E���f�B���O�{�b�N�X�g��
		void GameObject::ExtendLocalBounds(const Math::AABB& bounds)
		{
			localBounds.Extend(bounds);
			InvalidateWorldBounds();
		}

		// ���[���h�o�E���f�B���O�{�b�N�X���v�Z
		void GameObject::ComputeWorldBounds(Math::AABB& bounds)
		{
			bounds.Extend(worldBounds);

			for (auto&& child : children)
			{
				child->ComputeWorldBounds(bounds);
			}
		}

		// �e�Q�[���I�u�W�F�N�g�ݒ�
		void GameObject::SetParent(std::shared_ptr<GameObject> parent)
		{
			InvalidateWorldBounds();
			this->parent = parent;
			InvalidateWorldBounds();
			InvalidateWorldTransform();
		}

		// �q�Q�[���I�u�W�F�N�g�ǉ�
		void GameObject::AddChild(std::shared_ptr<GameObject> child, s32 index)
		{
			child->RemoveFromParent();
			child->SetParent(shared_from_this());

			if (index < 0)
			{
				children.emplace_back(child);
			}
			else
			{
				children.insert(children.begin() + index, child);
			}
		}

		// �q�Q�[���I�u�W�F�N�g�폜
		void GameObject::RemoveChild(std::shared_ptr<GameObject> child)
		{
			child->SetParent(nullptr);

			auto it = std::find(children.begin(), children.end(), child);
			if (it != children.end())
			{
				children.erase(it);
			}
		}

		// �e����Q�[���I�u�W�F�N�g���폜
		void GameObject::RemoveFromParent()
		{
			std::shared_ptr<GameObject> parent = this->parent.lock();
			if (parent != nullptr)
			{
				parent->RemoveChild(shared_from_this());
			}
		}

		// �q�Q�[���I�u�W�F�N�g���N���A
		void GameObject::ClearChildren()
		{
			children.clear();
		}

		// �R���|�[�l���g�ǉ�
		void GameObject::AddComponent(std::shared_ptr<Component> component, IGame* game, s32 index)
		{
			component->SetOwner(shared_from_this());
			component->Awake(game);

			if (index < 0)
			{
				components.emplace_back(component);
			}
			else
			{
				components.insert(components.begin() + index, component);
			}
		}

		// �R���|�[�l���g�폜
		void GameObject::RemoveComponent(std::shared_ptr<Component> component, IGame* game)
		{
			component->SetOwner(nullptr);
			component->OnDestroy(game);

			auto&& it = std::find(components.begin(), components.end(), component);
			if (it != components.end())
			{
				components.erase(it);
			}
		}

		// �R���|�[�l���g���N���A
		void GameObject::ClearComponents()
		{
			components.clear();
		}

		// ���[�J���g�����X�t�H�[���X�V
		void GameObject::UpdateLocalTransform()
		{
			if (localTransformDirty)
			{
				localTransformDirty = false;
#if 1
				Math::Matrix pivotM, scaleM, rotateM, translateM;
				pivotM = Math::MatrixTranslation(-pivot.x, -pivot.y, -pivot.z);
				scaleM = Math::MatrixScaling(scale.x, scale.y, scale.z);
				rotateM = Math::MatrixRotationQuaternion(&rotate);
				translateM = Math::MatrixTranslation(pivot.x + translate.x, pivot.y + translate.y, pivot.z + translate.z);

				Math::Matrix pivotScale, pivotScaleRotate;
				pivotScale = Math::MatrixMultiply(pivotM, scaleM);
				pivotScaleRotate = Math::MatrixMultiply(pivotScale, rotateM);
				localTransform = Math::MatrixMultiply(pivotScaleRotate, translateM);
#else
				math::MatrixRotationQuaternion(&m_local_transform, &m_rotate);
				math::Vec4Scale(&m_local_transform.x, &m_local_transform.x, m_scale.x);
				math::Vec4Scale(&m_local_transform.y, &m_local_transform.y, m_scale.y);
				math::Vec4Scale(&m_local_transform.z, &m_local_transform.z, m_scale.z);
				fnd::MemCpy(&m_local_transform.w, &m_translate, sizeof(m_translate));
#endif
				InvalidateWorldTransform();
				InvalidateWorldBounds();
			}
		}

		// ���[���h�g�����X�t�H�[���X�V
		void GameObject::UpdateWorldTransform()
		{
			if (worldTransformDirty)
			{
				std::shared_ptr<GameObject> parent = this->parent.lock();
				if (parent != nullptr)
				{
					worldTransform = Math::MatrixMultiply(localTransform, parent->worldTransform);
				}
				else
				{
					worldTransform = localTransform;
				}

				worldTransformDirty = false;
				worldTransformUpdated = true;
			}
		}

		// ���[���h�o�E���f�B���O�{�b�N�X�X�V
		void GameObject::UpdateWorldBounds()
		{
			if (boundsDirty)
			{
				worldBounds = localBounds;
				worldBounds.Transform(worldTransform);

				boundsDirty = false;
				boundsUpdated = true;
			}
		}

		// ���[�J���g�����X�t�H�[���X�V�ʒm
		void GameObject::InvalidateLocalTransform()
		{
			localTransformDirty = true;
		}

		// ���[���h�g�����X�t�H�[���X�V�ʒm
		void GameObject::InvalidateWorldTransform()
		{
			worldTransformDirty = true;
			InvalidateWorldBounds();

			for (auto&& child : children)
			{
				child->InvalidateWorldTransform();
			}
		}

		// ���[���h�o�E���f�B���O�{�b�N�X�X�V�ʒm
		void GameObject::InvalidateWorldBounds()
		{
			boundsDirty = true;

			std::shared_ptr<GameObject> parent = this->parent.lock();
			if (parent != nullptr)
			{
				parent->InvalidateWorldBounds();
			}
		}

		// �Z�b�g�A�b�v
		void GameObject::Setup(IGame* game)
		{
			for (auto&& component : components)
			{
				component->Awake(game);
			}

			boundsDirty = true;
			localTransformDirty = true;
			boundsDirty = true;
			UpdateLocalTransform();
			UpdateWorldTransform();
			UpdateWorldBounds();

			for (auto&& child : children)
			{
				child->Setup(game);
			}

		}

		// �C���X�^���X������̉�������
		void GameObject::Resolve()
		{
			for (auto&& child : children)
			{
				child->Resolve();
			}
		}

		// �j��
		void GameObject::Destroy(IGame* game)
		{
			for (auto&& component : components)
			{
				component->OnDestroy(game);
			}

			for (auto&& child : children)
			{
				child->Destroy(game);
			}
		}
	}
}