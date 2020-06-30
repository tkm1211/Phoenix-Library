#pragma once

#include <vector>
#include "Phoenix/Math/PhoenixMath.h"
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/Graphics/Model.h"
#include "Phoenix/Graphics/Animation.h"
#include "Phoenix/OS/Path.h"
#include "../Source/Loader/Loader.h"


namespace Phoenix
{
	namespace FrameWork
	{
		/*class ITransform
		{
		public:
			// �X�P�[���l��ݒ�
			virtual void SetScale(const Math::Vector3& scale) = 0;

			// ��]�l��ݒ�
			virtual void SetRotate(const Math::Quaternion& rotate) = 0;

			// �ړ��l��ݒ�
			virtual void SetTranslate(const Math::Vector3& translate) = 0;

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
		};

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
		};*/

		class Animator
		{
		private:
			struct Animation
			{
				std::string filename;
				std::shared_ptr<Graphics::IAnimationResource> resource;
				std::unique_ptr<Graphics::IAnimationPlayer> player;

				template<class Archive>
				void serialize(Archive& archive, u32 version);
			};

		private:
			std::vector<Animation> animations;
			std::unique_ptr<OS::IFileStream> file;

		public:
			void Initialize()
			{
				file = OS::IFileStream::Create();
				file->Initialize(nullptr);
			}

			// �A�j���[�V�������\�[�X�ǂݍ���
			void LoadResource(OS::IResourceManager* resourceManamger, const char* filename, s32 index)
			{
				if (index < 0)
				{
					index = static_cast<s32>(animations.size());
					animations.emplace_back(Animation());
				}

				Animation& animation = animations.at(index);

				animation.filename = OS::Path::ChangeFileExtension(filename, "ani");

				if (OS::Path::CheckFileExtension(filename, "fbx") && !file->Exists(animation.filename.c_str()))
				{
					std::unique_ptr<Loader::ILoader> loader = Loader::ILoader::Create();
					if (!loader->Initialize(filename))
					{
						return;
					}
					Graphics::AnimationData data;
					if (!loader->Load(data))
					{
						return;
					}
					Graphics::AnimationData::Serialize(data, animation.filename.c_str());
				}

				LoadResource(resourceManamger, animation);
			}

			void LoadResource(OS::IResourceManager* resourceManamger, Animation& animation)
			{
				animation.resource = resourceManamger->LoadAsync<Graphics::IAnimationResource>(animation.filename.c_str());
			}

			// �A�j���[�V�����N���b�v������
			const Graphics::AnimationData::Clip* FindAnimationClip(const char* name)
			{
				for (Animation& animation : animations)
				{
					const Graphics::AnimationData& data = animation.resource->GetAnimationData();
					for (const Graphics::AnimationData::Clip& clip : data.clips)
					{
						if (clip.name == name)
						{
							return &clip;
						}
					}
				}
				return nullptr;
			}

		public:
			template<class Archive>
			void serialize(Archive& archive, u32 version);
		};

		class ModelObject
		{
		public:
			struct Node
			{
				const char* name;
				Node* parent;
				Math::Vector3 scale;
				Math::Quaternion rotate;
				Math::Vector3 translate;
				Math::Matrix localTransform;
				Math::Matrix worldTransform;
			};

		private:
			std::shared_ptr<Graphics::IModelResource> modelResource;
			std::unique_ptr<Animator> animator;
			std::vector<Node> nodes;
			std::unique_ptr<OS::IResourceManager> resourceManamger;
			std::unique_ptr<OS::IFileStream> file;

		public:
			ModelObject() {}
			~ModelObject() {}

		public:
			// ������
			void Initialize(Graphics::IGraphicsDevice* graphicsDevice)
			{
				resourceManamger = Phoenix::OS::IResourceManager::Create();
				resourceManamger->Initialize(nullptr);
				resourceManamger->RegisterFactory("ani", Phoenix::Graphics::IAnimationResourceFactory::Create());
				resourceManamger->RegisterFactory("mdl", Phoenix::Graphics::IModelResourceFactory::Create(graphicsDevice->GetDevice()));

				file = OS::IFileStream::Create();
				file->Initialize(nullptr);
			}

			// ���f���̓ǂݍ���
			void Load(const char* filename)
			{
				std::string modelFilename;
				modelFilename = OS::Path::ChangeFileExtension(filename, "mdl");

				if (OS::Path::CheckFileExtension(filename, "fbx") && !file->Exists(modelFilename.c_str()))
				{
					std::unique_ptr<Loader::ILoader> loader = Loader::ILoader::Create();
					if (!loader->Initialize(filename))
					{
						return;
					}
					Graphics::ModelData data;
					if (!loader->Load(data))
					{
						return;
					}
					Graphics::ModelData::Serialize(data, modelFilename.c_str());
				}

				modelResource = resourceManamger->LoadImmediate<Graphics::IModelResource>(modelFilename.c_str());
				if (modelResource == nullptr)
				{
					return;
				}

				animator = std::make_unique<Animator>();
				animator->Initialize();
				LoadAnimation(filename, -1);

				const std::vector<Graphics::ModelData::Node>& resourceNodes = modelResource->GetModelData().nodes;
				nodes.resize(resourceNodes.size());

				for (size_t nodeIndex = 0; nodeIndex < nodes.size(); nodeIndex++)
				{
					auto&& src = resourceNodes.at(nodeIndex);
					auto&& dst = nodes.at(nodeIndex);

					dst.name = src.name.c_str();
					dst.parent = src.parentIndex >= 0 ? &nodes.at(src.parentIndex) : nullptr;
					dst.scale = src.scale;
					dst.rotate = src.rotate;
					dst.translate = src.translate;
				}
			}

			// �A�j���[�V�����̓ǂݍ���
			void LoadAnimation(const char* filename, s32 index)
			{
				animator->LoadResource(resourceManamger.get(), filename, index);
			}

			// ���[�J���ϊ��s��v�Z
			void CalculateLocalTransform()
			{
				for (Node& node : nodes)
				{
					Math::Matrix scale, rotate, translate;
					scale = Math::MatrixScaling(node.scale.x, node.scale.y, node.scale.z);
					rotate = Math::MatrixRotationQuaternion(&Math::Quaternion(node.rotate.x, node.rotate.y, node.rotate.z, node.rotate.w));
					translate = Math::MatrixTranslation(node.translate.x, node.translate.y, node.translate.z);

					node.localTransform = scale * rotate * translate;
				}
			}

			// ���[���h�ϊ��s��v�Z
			void CalculateWorldTransform(const Math::Matrix& worldTransform)
			{
				for (Node& node : nodes)
				{
					if (node.parent != nullptr)
					{
						node.worldTransform = node.localTransform * node.parent->worldTransform;
					}
					else
					{
						node.worldTransform = node.localTransform * worldTransform;
					}
				}
			}

			// ���f�����\�[�X�̎擾
			Graphics::IModelResource* GetModelResource() { return modelResource.get(); }

			// �m�[�h�̎擾
			const std::vector<Node>& GetNodes() { return nodes; }
		};
	}
}