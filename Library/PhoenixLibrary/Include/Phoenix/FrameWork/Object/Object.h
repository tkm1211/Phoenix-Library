#pragma once

#include <vector>
#include "Phoenix/Math/PhoenixMath.h"
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/Graphics/Model.h"
#include "Phoenix/Graphics/Animation.h"
#include "Phoenix/OS/Path.h"
#include "Phoenix/FND/STD.h"
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

		class Animator;
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

			std::vector<Math::Matrix> boneTransform;
			u32 boneTransformCount = 0;

		public:
			ModelObject() {}
			~ModelObject() {}

		public:
			// ������
			void Initialize(Graphics::IGraphicsDevice* graphicsDevice);

			// ���f���̓ǂݍ���
			void Load(const char* filename);

			// �A�j���[�V�����̓ǂݍ���
			void LoadAnimation(const char* filename, s32 index);

			// ���[�J���ϊ��s��v�Z
			void CalculateLocalTransform();

			// ���[���h�ϊ��s��v�Z
			void CalculateWorldTransform(const Math::Matrix& worldTransform);

			// �A�j���[�V�����̍Đ�
			void PlayAnimation(u32 bank, u32 clip, f32 fadeTime = 0.0f);

			// �A�j���[�V�����̍X�V
			void UpdateAnimation(f32 elapsedTime);

			// ���f�����\�[�X�̎擾
			Graphics::IModelResource* GetModelResource() { return modelResource.get(); }

			// �m�[�h�̎擾
			std::vector<Node>& GetNodes() { return nodes; }

			// �{�[���g�����X�t�H�[���̎擾
			Math::Matrix* GetBoneTransforms() { return boneTransform.data(); }

			// �{�[���g�����X�t�H�[���̃T�C�Y�擾
			u32 GetBoneTransformCount() { return boneTransformCount; }
		};

		class Animator
		{
		private:
			struct Animation
			{
				std::string filename;
				std::shared_ptr<Graphics::IAnimationResource> resource;
				std::unique_ptr<Graphics::IAnimationPlayer> player;
				std::vector<s16> bindNodeIDs;

				template<class Archive>
				void serialize(Archive& archive, u32 version);
			};

		private:
			std::vector<Animation> animations;
			std::vector<ModelObject::Node>* nodes;
			std::unique_ptr<OS::IFileStream> file;

			Animation* currentAnimation = nullptr;

		public:
			void Initialize(ModelObject* model)
			{
				file = OS::IFileStream::Create();
				file->Initialize(nullptr);

				this->nodes = &model->GetNodes();
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
				// TODO : animation�����܂����[�h�o���ĂȂ�
				LoadResource(resourceManamger, animation);
			}

			void LoadResource(OS::IResourceManager* resourceManamger, Animation& animation)
			{
				animation.resource = resourceManamger->LoadImmediate<Graphics::IAnimationResource>(animation.filename.c_str());
				animation.player = Graphics::IAnimationPlayer::Create();
				animation.player->Initialize(animation.resource);
				BindAnimationNodes(animation);
			}

			// �Đ�
			void Play(u32 bank, u32 clip, f32 fadeTime = 0.0f)
			{
				if (bank < 0 || bank >= animations.size())
				{
					return;
				}

				Animation& animation = animations.at(bank);
				const Graphics::AnimationData& data = animation.resource->GetAnimationData();

				if (clip < 0 || clip >= data.clips.size())
				{
					return;
				}

				currentAnimation = &animation;
				animation.player->Play(clip);
				animation.player->SetBlendTime(fadeTime);
			}

			// �A�j���[�V�����o���N�C���f�b�N�X�擾
			u32 GetAnimationBankIndex(const char* name) const
			{
				for (size_t i = 0; i < animations.size(); ++i)
				{
					if (0 == FND::StrCmp(name, OS::Path::GetFileNameWithoutExtension(animations.at(i).filename.c_str())))
					{
						return i;
					}
				}
				return -1;
			}

			void Update(f32 elapsedTime)
			{
				if (currentAnimation != nullptr)
				{
					std::unique_ptr<Graphics::IAnimationPlayer>& animationPlayer = currentAnimation->player;
					animationPlayer->Update(elapsedTime);

					if (animationPlayer->IsPlaying())
					{
						s32 animationNodeCount = nodes->size();
						for (s32 animationNodeID = 0; animationNodeID < animationNodeCount; ++animationNodeID)
						{
							s16 bindNodeID = currentAnimation->bindNodeIDs.at(animationNodeID);
							if (bindNodeID < 0) continue;

							ModelObject::Node& node = nodes->at(animationNodeID);

							Math::Vector3 scale = node.scale;
							Math::Quaternion rotate = node.rotate;
							Math::Vector3 translate = node.translate;

							animationPlayer->CalculateScale(animationNodeID, scale);
							animationPlayer->CalculateRotate(animationNodeID, rotate);
							animationPlayer->CalculateTranslate(animationNodeID, translate);

							node.scale = scale;
							node.rotate = rotate;
							node.translate = translate;
						}
					}
				}
			}

			// �A�j���[�V�����m�[�h�ƃo�C���h
			void BindAnimationNodes(Animation& animation)
			{
				const Graphics::AnimationData& data = animation.resource->GetAnimationData();
				animation.bindNodeIDs.resize(data.nodeNames.size());

				for (size_t i = 0; i < data.nodeNames.size(); ++i)
				{
					animation.bindNodeIDs.at(i) = -1;

					const std::string& nodeName = data.nodeNames[i];
					for (size_t j = 0; j < nodes->size(); ++j)
					{
						ModelObject::Node& node = nodes->at(j);
						if (nodeName == node.name)
						{
							animation.bindNodeIDs.at(i) = static_cast<s16>(static_cast<s32>(j));
						}
					}
				}
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
	}
}