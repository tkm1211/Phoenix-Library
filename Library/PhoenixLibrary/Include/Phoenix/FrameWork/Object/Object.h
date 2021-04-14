//#pragma once
//
//#include <vector>
//#include "Phoenix/Math/PhoenixMath.h"
//#include "Phoenix/Graphics/GraphicsDevice.h"
//#include "Phoenix/Graphics/Model.h"
//#include "Phoenix/Graphics/Animation.h"
//#include "Phoenix/Graphics/Texture.h"
//#include "Phoenix/OS/Path.h"
//#include "Phoenix/FND/STD.h"
//#include "../Source/Loader/Loader.h"
//
//#define USE_BEFORE 0
//
//
//namespace Phoenix
//{
//	namespace FrameWork
//	{
//		/*class ITransform
//		{
//		public:
//			// �X�P�[���l��ݒ�
//			virtual void SetScale(const Math::Vector3& scale) = 0;
//
//			// ��]�l��ݒ�
//			virtual void SetRotate(const Math::Quaternion& rotate) = 0;
//
//			// �ړ��l��ݒ�
//			virtual void SetTranslate(const Math::Vector3& translate) = 0;
//
//			// �X�P�[���l���擾
//			virtual const Math::Vector3& GetScale() = 0;
//
//			// ��]�l���擾
//			virtual const Math::Quaternion& GetRotate() = 0;
//
//			// �ړ��l���擾
//			virtual const Math::Vector3& GetTranslate() = 0;
//
//			// ���[�J���g�����X�t�H�[�����擾
//			virtual const Math::Matrix& GetLocalTransform() = 0;
//
//			// ���[���h�g�����X�t�H�[�����擾
//			virtual const Math::Matrix& GetWorldTransform() = 0;
//		};
//
//		class IObject : public ITransform
//		{
//		public:
//			// ���O�ݒ�
//			virtual void SetName(const char* name) = 0;
//
//			// ���O�擾
//			virtual const char* GetName() = 0;
//
//			// �A�N�e�B�u��Ԃ�ݒ�
//			virtual void SetActive(bool active) = 0;
//
//			// �A�N�e�B�u��Ԃ��擾
//			virtual bool IsActive() = 0;
//
//			// �e�Q�[���I�u�W�F�N�g���擾
//			virtual IObject* GetParent() = 0;
//
//			// �q�Q�[���I�u�W�F�N�g���擾
//			virtual const std::vector<IObject*>& GetChildren() = 0;
//
//			// �q��ǉ��ł��邩����
//			virtual bool CanAddChild(IObject* child) = 0;
//
//			// �q��ǉ�
//			virtual bool AddChild(IObject* child) = 0;
//
//			// �o�E���f�B���O�{�b�N�X���擾
//			virtual const Math::AABB& GetBounds() = 0;
//		};*/
//
//		struct CollisionData
//		{
//			Phoenix::Math::Vector3 pos;
//			Phoenix::f32 radius = 0.0f;
//			Phoenix::s32 boneIndex = 0;
//		};
//
//		class Animator;
//		class ModelObject
//		{
//		public:
//			struct Node
//			{
//				const char* name;
//				Node* parent;
//				Math::Vector3 scale;
//				Math::Quaternion rotate;
//				Math::Vector3 translate;
//				Math::Matrix localTransform;
//				Math::Matrix worldTransform;
//			};
//
//			struct MeshNode
//			{
//				std::vector<Math::Matrix> boneTransform;
//				u32 boneTransformCount = 0;
//			};
//
//			struct Material
//			{
//				std::string name;
//				std::vector<std::unique_ptr<Graphics::ITexture>> textures;
//				std::vector<Math::Color> colors;
//			};
//
//		private:
//			std::shared_ptr<Graphics::IModelResource> modelResource;
//			std::unique_ptr<Animator> animator;
//			std::vector<Node> nodes;
//			std::vector<MeshNode> meshNodes;
//			std::vector<Material> materials;
//			std::unique_ptr<OS::IResourceManager> resourceManamger;
//			std::unique_ptr<OS::IFileStream> file;
//
//			// �{�[�����񋓗p
//			std::vector<const char*> boneNames;
//
//		public:
//			ModelObject() {}
//			~ModelObject() {}
//
//		public:
//			// ����
//			static std::unique_ptr<ModelObject> Create();
//
//			// ������
//			void Initialize(Graphics::IGraphicsDevice* graphicsDevice);
//
//			// ���f���̓ǂݍ���
//			void Load(Graphics::IGraphicsDevice* graphicsDevice, const char* filename);
//
//			// �A�j���[�V�����̓ǂݍ���
//			void LoadAnimation(const char* filename, s32 index);
//
//			// �A�j���[�V�����̃��C���[�ǉ�
//			void SetAnimationLayer(u32 baseLayerID, u32 addLayerID, u32 parentNodeID, Phoenix::Math::Vector3 pos);
//
//			// �s����X�V
//			void UpdateTransform(f32 elapsedTime);
//
//			// ���[�J���ϊ��s����X�V
//			void UpdateLocalTransform();
//
//			// ���[���h�ϊ��s����X�V
//			void UpdateWorldTransform();
//
//			// �{�[���ϊ��s����X�V
//			void UpdateBoneTransform();
//
//			// �A�j���[�V�����̍Đ�
//			void PlayAnimation(u32 bank, u32 clip, f32 fadeTime = 0.0f);
//
//			// �A�j���[�V�����̓����Đ�
//			void PlayBlendAnimation(u32 bank, u32 clip, f32 fadeTime = 0.0f);
//
//			// �ꎞ��~/�ĊJ
//			void PauseAnimation(bool pause);
//
//			// ��~
//			void StopAnimation();
//
//			// ���[�v�Đ��ݒ�
//			void SetLoopAnimation(bool loop);
//
//			// ���[�v�����Đ��ݒ�
//			void SetBlendLoopAnimation(bool loop);
//
//			// �Đ���
//			bool IsPlaying();
//
//			// ���[�v�Đ���
//			bool IsLoopAnimation();
//
//			// �A�j���[�V�����̍X�V
//			void UpdateAnimation(f32 elapsedTime);
//
//			// �Đ����Ԓ�(�b)�擾
//			f32 GetLength();
//
//			// �Đ����x�{���ݒ�
//			void SetSpeed(f32 speed);
//
//			// �Đ����x�{���擾
//			f32 GetSpeed();
//
//			// �u�����h����
//			bool IsBlend();
//
//			// �Đ��ʒu�ݒ�
//			void SetCurrentTime(f32 seconds);
//
//			// �Đ��ʒu�擾
//			f32 GetCurrentTime();
//
//			// �c��Đ����Ԏ擾
//			f32 GetLastTime();
//
//			// �Đ��J�n�ʒu�ݒ�
//			void SetBeginTime(f32 seconds);
//
//			// �Đ��J�n�ʒu�擾
//			f32 GetBeginTime();
//
//			// �Đ��I���ʒu�ݒ�
//			void SetEndTime(f32 seconds);
//
//			// �Đ��I���ʒu�擾
//			f32 GetEndTime();
//
//			// ���[�v�Đ��J�n�ʒu�ݒ�
//			void SetLoopBeginTime(f32 seconds);
//
//			// ���[�v�Đ��J�n�ʒu�擾
//			f32 GetLoopBeginTime();
//
//			// ���[�v�Đ��I���ʒu�ݒ�
//			void SetLoopEndTime(f32 seconds);
//
//			// ���[�v�Đ��I���ʒu�擾
//			f32 GetLoopEndTime();
//
//			// 
//			void SetBlendRate(f32 rate);
//
//			// ���f�����\�[�X�̎擾
//			Graphics::IModelResource* GetModelResource() { return modelResource.get(); }
//
//			// �m�[�h�̎擾
//			std::vector<Node>* GetNodes() { return &nodes; }
//
//			// �{�[�����擾
//			std::vector<const char*> GetBoneNames() { return boneNames; }
//
//			// �{�[���g�����X�t�H�[���̎擾
//			Math::Matrix* GetBoneTransforms(u32 meshIndex) { return meshNodes.at(meshIndex).boneTransform.data(); }
//			Math::Matrix GetBoneTransforms(u32 meshIndex, u32 boneIndex) { return meshNodes.at(meshIndex).boneTransform.at(boneIndex); }
//
//			// �{�[���C���f�b�N�X�̎擾
//			u32 GetBoneIndex(const char* name)
//			{
//				for (u32 i = 0; i < nodes.size(); ++i)
//				{
//					if (FND::StrCmp(nodes[i].name, name) == 0)
//					{
//						return i;
//					}
//				}
//
//				return -1;
//			}
//
//			// ���b�V���m�[�h�̃T�C�Y�擾
//			sizeT GetMeshNodes() { return meshNodes.size(); }
//
//			// �{�[���g�����X�t�H�[���̃T�C�Y�擾
//			u32 GetBoneTransformCount(u32 meshIndex) { return meshNodes.at(meshIndex).boneTransformCount; }
//
//			// �}�e���A���̃e�N�X�`���擾
//			Graphics::ITexture* GetTexture(u32 index, u32 texIndex)
//			{
//				return materials.at(index).textures.at(texIndex).get();
//			}
//
//			// �}�e���A���̃T�C�Y�擾
//			sizeT GetMaterialSize() { return materials.size(); }
//
//			// �}�e���A���̃e�N�X�`���T�C�Y�擾
//			sizeT GetTextureSize(u32 index) { return materials.at(index).textures.size(); }
//		};
//
//		class Animator
//		{
//		private:
//			struct Animation
//			{
//				std::string filename;
//				std::shared_ptr<Graphics::IAnimationResource> resource;
//				std::unique_ptr<Graphics::IAnimationPlayer> player;
//				std::vector<s16> bindNodeIDs;
//
//				template<class Archive>
//				void serialize(Archive& archive, u32 version);
//			};
//
//			struct AnimationBlend
//			{
//				s32 blendPoseID = -1;
//				Phoenix::Math::Vector3 pos;
//			};
//
//			struct AnimationLayer
//			{
//				std::vector<ModelObject::Node> blendedPose;
//				std::vector<AnimationBlend> blendPoses;
//				s32 parentNodeID = -1;
//			};
//
//			struct AnimaionMontage
//			{
//				s32 basePoseID = -1;
//				std::vector<AnimationLayer> layerPoses;
//			};
//
//		private:
//			std::vector<Animation> animations;
//			std::vector<AnimaionMontage> animationMontages;
//			std::vector<ModelObject::Node>* nodes;
//			std::unique_ptr<OS::IFileStream> file;
//
//#if USE_BEFORE
//			Animation* currentAnimation = nullptr;
//			Animation* blendCurrentAnimation = nullptr; // �����Đ��p
//#else
//			AnimaionMontage* currentMontage = nullptr;
//			AnimaionMontage* blendCurrentMontage = nullptr; // �����Đ��p
//#endif
//
//			f32 blendRate = 0.0f;
//
//		public:
//			void Initialize(ModelObject* model)
//			{
//				file = OS::IFileStream::Create();
//				file->Initialize(nullptr);
//
//				nodes = model->GetNodes();
//			}
//
//			// �A�j���[�V�������\�[�X�ǂݍ���
//			void LoadResource(OS::IResourceManager* resourceManamger, const char* filename, s32 index)
//			{
//				if (index < 0)
//				{
//					index = static_cast<s32>(animations.size());
//					animations.emplace_back(Animation());
//					animationMontages.emplace_back(AnimaionMontage());
//				}
//
//				Animation& animation = animations.at(index);
//				animationMontages.at(index).basePoseID = index;
//
//				animation.filename = OS::Path::ChangeFileExtension(filename, "ani");
//
//				if (OS::Path::CheckFileExtension(filename, "fbx") && !file->Exists(animation.filename.c_str()))
//				{
//					std::unique_ptr<Loader::ILoader> loader = Loader::ILoader::Create();
//					if (!loader->Initialize(filename))
//					{
//						return;
//					}
//					Graphics::AnimationData data;
//					if (!loader->Load(data))
//					{
//						return;
//					}
//					Graphics::AnimationData::Serialize(data, animation.filename.c_str());
//				}
//				LoadResource(resourceManamger, animation);
//			}
//
//			void LoadResource(OS::IResourceManager* resourceManamger, Animation& animation)
//			{
//				animation.resource = resourceManamger->LoadImmediate<Graphics::IAnimationResource>(animation.filename.c_str());
//
//				if (animation.resource)
//				{
//					animation.player = Graphics::IAnimationPlayer::Create();
//					animation.player->Initialize(animation.resource);
//					BindAnimationNodes(animation);
//				}
//			}
//
//			/// <summary>
//			/// �x�[�X�A�j���[�V�����Ƀu�����h����A�j���[�V�����ǉ�
//			/// </summary>
//			/// <param name="baseLayerID"> �x�[�X�ɂ���A�j���[�V�������Ǘ����Ă���z���Index </param>
//			/// <param name="addLayerID"> �ǉ�����A�j���[�V�������Ǘ����Ă���z���Index </param>
//			/// <param name="parentNodeID"> �u�����h��K�p����{�[���z���Index </param>
//			/// <param name="pos"> 3�����ɔz�u������W </param>
//			void SetAnimationLayer(u32 baseLayerID, u32 addLayerID, u32 parentNodeID, Phoenix::Math::Vector3 pos)
//			{
//				if (baseLayerID < 0 || animationMontages.size() <= baseLayerID)
//				{
//					return;
//				}
//				if (addLayerID < 0 || animationMontages.size() <= addLayerID)
//				{
//					return;
//				}
//				if (parentNodeID < 0 || nodes->size() <= parentNodeID)
//				{
//					return;
//				}
//
//				s32 layerIndex = static_cast<s32>(animationMontages.at(baseLayerID).layerPoses.size());
//				animationMontages.at(baseLayerID).layerPoses.emplace_back(AnimationLayer());
//
//				s32 blendIndex = static_cast<s32>(animationMontages.at(baseLayerID).layerPoses.at(layerIndex).blendPoses.size());
//				animationMontages.at(baseLayerID).layerPoses.at(layerIndex).parentNodeID = parentNodeID;
//				animationMontages.at(baseLayerID).layerPoses.at(layerIndex).blendPoses.emplace_back(AnimationBlend());
//				animationMontages.at(baseLayerID).layerPoses.at(layerIndex).blendPoses.at(blendIndex).blendPoseID = addLayerID;
//				animationMontages.at(baseLayerID).layerPoses.at(layerIndex).blendPoses.at(blendIndex).pos = pos;
//
//				if (animationMontages.at(baseLayerID).layerPoses.at(layerIndex).blendedPose.size() == 0)
//				{
//					s32 animationNodeCount = static_cast<s32>(nodes->size());
//					for (s32 animationNodeID = 0; animationNodeID < animationNodeCount; ++animationNodeID)
//					{
//						animationMontages.at(baseLayerID).layerPoses.at(layerIndex).blendedPose.emplace_back(nodes->at(animationNodeID));
//					}
//				}
//			}
//
//			// �Đ�
//			void Play(u32 bank, u32 clip, f32 fadeTime = 0.0f)
//			{
//#if USE_BEFORE
//				if (bank < 0 || bank >= animations.size())
//				{
//					return;
//				}
//
//				Animation& animation = animations.at(bank);
//				if (!animation.resource)
//				{
//					return;
//				}
//				const Graphics::AnimationData& data = animation.resource->GetAnimationData();
//
//				if (clip < 0 || clip >= data.clips.size())
//				{
//					return;
//				}
//
//				currentAnimation = &animation;
//				blendCurrentAnimation = nullptr;
//				animation.player->Play(clip);
//				animation.player->SetBlendTime(fadeTime);
//#else
//				if (bank < 0 || bank >= animationMontages.size())
//				{
//					return;
//				}
//
//				AnimaionMontage& animaionMontage = animationMontages.at(bank);
//				if (!animations.at(animaionMontage.basePoseID).resource)
//				{
//					return;
//				}
//				const Graphics::AnimationData& data = animations.at(animaionMontage.basePoseID).resource->GetAnimationData();
//
//				if (clip < 0 || clip >= data.clips.size())
//				{
//					return;
//				}
//
//				currentMontage = &animaionMontage;
//				blendCurrentMontage = nullptr;
//				animations.at(currentMontage->basePoseID).player->Play(clip);
//				animations.at(currentMontage->basePoseID).player->SetBlendTime(fadeTime);
//
//				s32 layerCount = static_cast<s32>(currentMontage->layerPoses.size());
//				if (layerCount == 0)
//				{
//					return;
//				}
//
//				for (s32 layerIndex = 0; layerIndex < layerCount; ++layerIndex)
//				{
//					s32 blendCount = static_cast<s32>(currentMontage->layerPoses.at(layerIndex).blendPoses.size());
//					if (blendCount == 0)
//					{
//						return;
//					}
//
//					for (s32 blendIndex = 0; blendIndex < blendCount; ++blendIndex)
//					{
//						animations.at(currentMontage->layerPoses.at(layerIndex).blendPoses.at(blendIndex).blendPoseID).player->Play(clip);
//					}
//				}
//#endif
//			}
//
//			// �����Đ�
//			void BlendPlay(u32 bank, u32 clip, f32 fadeTime = 0.0f)
//			{
//#if USE_BEFORE
//				if (bank < 0 || bank >= animations.size())
//				{
//					return;
//				}
//
//				Animation& animation = animations.at(bank);
//				if (!animation.resource)
//				{
//					return;
//				}
//				const Graphics::AnimationData& data = animation.resource->GetAnimationData();
//
//				if (clip < 0 || clip >= data.clips.size())
//				{
//					return;
//				}
//
//				blendRate = 0.0f;
//				blendCurrentAnimation = &animation;
//				animation.player->Play(clip);
//				animation.player->SetBlendTime(fadeTime);
//#else
//				if (bank < 0 || bank >= animationMontages.size())
//				{
//					return;
//				}
//
//				AnimaionMontage& animaionMontage = animationMontages.at(bank);
//				if (!animations.at(animaionMontage.basePoseID).resource)
//				{
//					return;
//				}
//				const Graphics::AnimationData& data = animations.at(animaionMontage.basePoseID).resource->GetAnimationData();
//
//				if (clip < 0 || clip >= data.clips.size())
//				{
//					return;
//				}
//
//				blendRate = 0.0f;
//				blendCurrentMontage = &animaionMontage;
//				animations.at(blendCurrentMontage->basePoseID).player->Play(clip);
//				animations.at(blendCurrentMontage->basePoseID).player->SetBlendTime(fadeTime);
//
//				s32 layerCount = static_cast<s32>(currentMontage->layerPoses.size());
//				if (layerCount == 0)
//				{
//					return;
//				}
//
//				for (s32 layerIndex = 0; layerIndex < layerCount; ++layerIndex)
//				{
//					s32 blendCount = static_cast<s32>(currentMontage->layerPoses.at(layerIndex).blendPoses.size());
//					if (blendCount == 0)
//					{
//						return;
//					}
//
//					for (s32 blendIndex = 0; blendIndex < blendCount; ++blendIndex)
//					{
//						animations.at(currentMontage->layerPoses.at(layerIndex).blendPoses.at(blendIndex).blendPoseID).player->Play(clip);
//					}
//				}
//#endif
//			}
//
//			// �ꎞ��~/�ĊJ
//			void Pause(bool pause)
//			{
//#if USE_BEFORE
//				currentAnimation->player->Pause(pause);
//#else
//				animations.at(currentMontage->basePoseID).player->Pause(pause);
//#endif
//			}
//
//			// ��~
//			void Stop()
//			{
//#if USE_BEFORE
//				currentAnimation->player->Stop();
//#else
//				animations.at(currentMontage->basePoseID).player->Stop();
//#endif
//			}
//
//			// ���[�v�Đ��ݒ�
//			void SetLoop(bool loop)
//			{
//#if USE_BEFORE
//				currentAnimation->player->SetLoop(loop);
//#else
//				animations.at(currentMontage->basePoseID).player->SetLoop(loop);
//#endif
//			}
//
//			// ���[�v�����Đ��ݒ�
//			void SetBlendLoop(bool loop)
//			{
//#if USE_BEFORE
//				blendCurrentAnimation->player->SetLoop(loop);
//#else
//				animations.at(blendCurrentMontage->basePoseID).player->SetLoop(loop);
//#endif
//			}
//
//			// �Đ���
//			bool IsPlaying()
//			{
//#if USE_BEFORE
//				return currentAnimation->player->IsPlaying();
//#else
//				return animations.at(currentMontage->basePoseID).player->IsPlaying();
//#endif
//			}
//
//			// ���[�v�Đ���
//			bool IsLoop()
//			{
//#if USE_BEFORE
//				return currentAnimation->player->IsLoop();
//#else
//				return animations.at(currentMontage->basePoseID).player->IsPlaying();
//#endif
//			}
//
//			// �Đ����Ԓ�(�b)�擾
//			f32 GetLength()
//			{
//#if USE_BEFORE
//				return currentAnimation->player->GetLength();
//#else
//				return animations.at(currentMontage->basePoseID).player->GetLength();
//#endif
//			}
//
//			// �Đ����x�{���ݒ�
//			void SetSpeed(f32 speed)
//			{
//#if USE_BEFORE
//				currentAnimation->player->SetSpeed(speed);
//#else
//				animations.at(currentMontage->basePoseID).player->SetSpeed(speed);
//#endif
//			}
//
//			// �Đ����x�{���擾
//			f32 GetSpeed()
//			{
//#if USE_BEFORE
//				return currentAnimation->player->GetSpeed();
//#else
//				return animations.at(currentMontage->basePoseID).player->GetSpeed();
//#endif
//			}
//
//			// �u�����h����
//			bool IsBlend()
//			{
//#if USE_BEFORE
//				return currentAnimation->player->IsBlend();
//#else
//				return animations.at(currentMontage->basePoseID).player->IsBlend();
//#endif
//			}
//			// �Đ��ʒu�ݒ�
//			void SetCurrentTime(f32 seconds)
//			{
//#if USE_BEFORE
//				currentAnimation->player->SetCurrentTime(seconds);
//#else
//				return animations.at(currentMontage->basePoseID).player->SetCurrentTime(seconds);
//#endif
//			}
//
//			// �Đ��ʒu�擾
//			f32 GetCurrentTime()
//			{
//#if USE_BEFORE
//				return currentAnimation->player->GetCurrentTime();
//#else
//				return animations.at(currentMontage->basePoseID).player->GetCurrentTime();
//#endif
//			}
//
//			// �c��Đ����Ԏ擾
//			f32 GetLastTime()
//			{
//#if USE_BEFORE
//				return currentAnimation->player->GetLastTime();
//#else
//				return animations.at(currentMontage->basePoseID).player->GetLastTime();
//#endif
//			}
//
//			// �Đ��J�n�ʒu�ݒ�
//			void SetBeginTime(f32 seconds)
//			{
//#if USE_BEFORE
//				currentAnimation->player->SetBeginTime(seconds);
//#else
//				animations.at(currentMontage->basePoseID).player->SetBeginTime(seconds);
//#endif
//			}
//
//			// �Đ��J�n�ʒu�擾
//			f32 GetBeginTime()
//			{
//#if USE_BEFORE
//				return currentAnimation->player->GetBeginTime();
//#else
//				return animations.at(currentMontage->basePoseID).player->GetBeginTime();
//#endif
//			}
//
//			// �Đ��I���ʒu�ݒ�
//			void SetEndTime(f32 seconds)
//			{
//#if USE_BEFORE
//				currentAnimation->player->SetEndTime(seconds);
//#else
//				animations.at(currentMontage->basePoseID).player->SetEndTime(seconds);
//#endif
//			}
//
//			// �Đ��I���ʒu�擾
//			f32 GetEndTime()
//			{
//#if USE_BEFORE
//				return currentAnimation->player->GetEndTime();
//#else
//				return animations.at(currentMontage->basePoseID).player->GetEndTime();
//#endif
//			}
//
//			// ���[�v�Đ��J�n�ʒu�ݒ�
//			void SetLoopBeginTime(f32 seconds)
//			{
//#if USE_BEFORE
//				currentAnimation->player->SetLoopBeginTime(seconds);
//#else
//				animations.at(currentMontage->basePoseID).player->SetLoopBeginTime(seconds);
//#endif
//			}
//
//			// ���[�v�Đ��J�n�ʒu�擾
//			f32 GetLoopBeginTime()
//			{
//#if USE_BEFORE
//				return currentAnimation->player->GetLoopBeginTime();
//#else
//				return animations.at(currentMontage->basePoseID).player->GetLoopBeginTime();
//#endif
//			}
//
//			// ���[�v�Đ��I���ʒu�ݒ�
//			void SetLoopEndTime(f32 seconds)
//			{
//#if USE_BEFORE
//				currentAnimation->player->SetLoopEndTime(seconds);
//#else
//				animations.at(currentMontage->basePoseID).player->SetLoopEndTime(seconds);
//#endif
//			}
//
//			// ���[�v�Đ��I���ʒu�擾
//			f32 GetLoopEndTime()
//			{
//#if USE_BEFORE
//				return currentAnimation->player->GetLoopEndTime();
//#else
//				return animations.at(currentMontage->basePoseID).player->GetLoopEndTime();
//#endif
//			}
//
//			// �u�����h���[�g�̐ݒ�
//			void SetBlendRate(f32 rate)
//			{
//				blendRate = rate;
//			}
//
//			// �A�j���[�V�����o���N�C���f�b�N�X�擾
//			u32 GetAnimationBankIndex(const char* name) const
//			{
//				for (size_t i = 0; i < animations.size(); ++i)
//				{
//					if (0 == FND::StrCmp(name, OS::Path::GetFileNameWithoutExtension(animations.at(i).filename.c_str())))
//					{
//						return static_cast<u32>(i);
//					}
//				}
//				return -1;
//			}
//
//			void Update(f32 elapsedTime)
//			{
//#if USE_BEFORE
//				if (currentAnimation != nullptr)
//				{
//					if (blendCurrentAnimation != nullptr)
//					{
//						std::unique_ptr<Graphics::IAnimationPlayer>& animationPlayer = currentAnimation->player;
//						std::unique_ptr<Graphics::IAnimationPlayer>& blendAnimationPlayer = blendCurrentAnimation->player;
//
//						animationPlayer->Update(elapsedTime);
//						blendAnimationPlayer->Update(elapsedTime);
//
//						if (animationPlayer->IsPlaying())
//						{
//							s32 animationNodeCount = static_cast<s32>(nodes->size());
//							for (s32 animationNodeID = 0; animationNodeID < animationNodeCount; ++animationNodeID)
//							{
//								s16 bindNodeID = currentAnimation->bindNodeIDs.at(animationNodeID);
//								if (bindNodeID < 0) continue;
//
//								s16 blendBindNodeID = blendCurrentAnimation->bindNodeIDs.at(animationNodeID);
//								if (blendBindNodeID < 0) continue;
//
//								ModelObject::Node& node = nodes->at(animationNodeID);
//
//								Math::Vector3 scale = node.scale;
//								Math::Quaternion rotate = node.rotate;
//								Math::Vector3 translate = node.translate;
//
//								Math::Vector3 blendScale = node.scale;
//								Math::Quaternion blendRotate = node.rotate;
//								Math::Vector3 blendTranslate = node.translate;
//
//								animationPlayer->CalculateScale(animationNodeID, scale);
//								animationPlayer->CalculateRotate(animationNodeID, rotate);
//								animationPlayer->CalculateTranslate(animationNodeID, translate);
//
//								blendAnimationPlayer->CalculateScale(animationNodeID, blendScale);
//								blendAnimationPlayer->CalculateRotate(animationNodeID, blendRotate);
//								blendAnimationPlayer->CalculateTranslate(animationNodeID, blendTranslate);
//
//								node.scale = Math::Vector3Lerp(scale, blendScale, blendRate);
//								node.rotate = Math::QuaternionSlerp(rotate, blendRotate, blendRate);
//								node.translate = Math::Vector3Lerp(translate, blendTranslate, blendRate);
//							}
//						}
//					}
//					else
//					{
//						std::unique_ptr<Graphics::IAnimationPlayer>& animationPlayer = currentAnimation->player;
//						animationPlayer->Update(elapsedTime);
//
//						if (animationPlayer->IsPlaying())
//						{
//							s32 animationNodeCount = static_cast<s32>(nodes->size());
//							for (s32 animationNodeID = 0; animationNodeID < animationNodeCount; ++animationNodeID)
//							{
//								s16 bindNodeID = currentAnimation->bindNodeIDs.at(animationNodeID);
//								if (bindNodeID < 0) continue;
//
//								ModelObject::Node& node = nodes->at(animationNodeID);
//
//								Math::Vector3 scale = node.scale;
//								Math::Quaternion rotate = node.rotate;
//								Math::Vector3 translate = node.translate;
//
//								animationPlayer->CalculateScale(animationNodeID, scale);
//								animationPlayer->CalculateRotate(animationNodeID, rotate);
//								animationPlayer->CalculateTranslate(animationNodeID, translate);
//
//								node.scale = scale;
//								node.rotate = rotate;
//								node.translate = translate;
//							}
//						}
//					}
//				}
//#else
//				if (currentMontage != nullptr)
//				{
//					if (blendCurrentMontage != nullptr)
//					{
//						std::unique_ptr<Graphics::IAnimationPlayer>& animationPlayer = animations.at(currentMontage->basePoseID).player;
//						std::unique_ptr<Graphics::IAnimationPlayer>& blendAnimationPlayer = animations.at(blendCurrentMontage->basePoseID).player;
//
//						animationPlayer->Update(elapsedTime);
//						blendAnimationPlayer->Update(elapsedTime);
//
//						if (animationPlayer->IsPlaying())
//						{
//							s32 animationNodeCount = static_cast<s32>(nodes->size());
//							for (s32 animationNodeID = 0; animationNodeID < animationNodeCount; ++animationNodeID)
//							{
//								s16 bindNodeID = animations.at(currentMontage->basePoseID).bindNodeIDs.at(animationNodeID);
//								if (bindNodeID < 0) continue;
//
//								s16 blendBindNodeID = animations.at(blendCurrentMontage->basePoseID).bindNodeIDs.at(animationNodeID);
//								if (blendBindNodeID < 0) continue;
//
//								ModelObject::Node& node = nodes->at(animationNodeID);
//
//								Math::Vector3 scale = node.scale;
//								Math::Quaternion rotate = node.rotate;
//								Math::Vector3 translate = node.translate;
//
//								Math::Vector3 blendScale = node.scale;
//								Math::Quaternion blendRotate = node.rotate;
//								Math::Vector3 blendTranslate = node.translate;
//
//								animationPlayer->CalculateScale(animationNodeID, scale);
//								animationPlayer->CalculateRotate(animationNodeID, rotate);
//								animationPlayer->CalculateTranslate(animationNodeID, translate);
//
//								blendAnimationPlayer->CalculateScale(animationNodeID, blendScale);
//								blendAnimationPlayer->CalculateRotate(animationNodeID, blendRotate);
//								blendAnimationPlayer->CalculateTranslate(animationNodeID, blendTranslate);
//
//								node.scale = Math::Vector3Lerp(scale, blendScale, blendRate);
//								node.rotate = Math::QuaternionSlerp(rotate, blendRotate, blendRate);
//								node.translate = Math::Vector3Lerp(translate, blendTranslate, blendRate);
//							}
//						}
//					}
//					else
//					{
//						std::unique_ptr<Graphics::IAnimationPlayer>& animationPlayer = animations.at(currentMontage->basePoseID).player;
//						animationPlayer->Update(elapsedTime);
//
//						if (animationPlayer->IsPlaying())
//						{
//							s32 animationNodeCount = static_cast<s32>(nodes->size());
//							for (s32 animationNodeID = 0; animationNodeID < animationNodeCount; ++animationNodeID)
//							{
//								s16 bindNodeID = animations.at(currentMontage->basePoseID).bindNodeIDs.at(animationNodeID);
//								if (bindNodeID < 0) continue;
//
//								ModelObject::Node& node = nodes->at(animationNodeID);
//
//								Math::Vector3 scale = node.scale;
//								Math::Quaternion rotate = node.rotate;
//								Math::Vector3 translate = node.translate;
//
//								animationPlayer->CalculateScale(animationNodeID, scale);
//								animationPlayer->CalculateRotate(animationNodeID, rotate);
//								animationPlayer->CalculateTranslate(animationNodeID, translate);
//
//								node.scale = scale;
//								node.rotate = rotate;
//								node.translate = translate;
//							}
//						}
//					}
//				}
//#endif
//			}
//
//			void UpdateLayer(AnimationLayer* layers, s32 layerCount)
//			{
//				for (s32 layerIndex = 0; layerIndex < layerCount; ++layerIndex)
//				{
//					s32 blendCount = static_cast<s32>(layers[layerIndex].blendPoses.size());
//					if (blendCount <= 0) continue;
//
//					s32 animationNodeCount = static_cast<s32>(layers[layerIndex].blendedPose.size());
//					for (s32 animationNodeID = 0; animationNodeID < animationNodeCount; ++animationNodeID)
//					{
//						layers[layerIndex].blendPoses
//
//					}
//				}
//			}
//
//			void Update(Phoenix::Math::Vector3 pos, f32 elapsedTime)
//			{
//				if (currentMontage != nullptr)
//				{
//					if (blendCurrentMontage != nullptr)
//					{
//						std::unique_ptr<Graphics::IAnimationPlayer>& animationPlayer = animations.at(currentMontage->basePoseID).player;
//						std::unique_ptr<Graphics::IAnimationPlayer>& blendAnimationPlayer = animations.at(blendCurrentMontage->basePoseID).player;
//
//						animationPlayer->Update(elapsedTime);
//						blendAnimationPlayer->Update(elapsedTime);
//
//						if (animationPlayer->IsPlaying())
//						{
//							s32 animationNodeCount = static_cast<s32>(nodes->size());
//							for (s32 animationNodeID = 0; animationNodeID < animationNodeCount; ++animationNodeID)
//							{
//								s16 bindNodeID = animations.at(currentMontage->basePoseID).bindNodeIDs.at(animationNodeID);
//								if (bindNodeID < 0) continue;
//
//								s16 blendBindNodeID = animations.at(blendCurrentMontage->basePoseID).bindNodeIDs.at(animationNodeID);
//								if (blendBindNodeID < 0) continue;
//
//								ModelObject::Node& node = nodes->at(animationNodeID);
//
//								Math::Vector3 scale = node.scale;
//								Math::Quaternion rotate = node.rotate;
//								Math::Vector3 translate = node.translate;
//
//								Math::Vector3 blendScale = node.scale;
//								Math::Quaternion blendRotate = node.rotate;
//								Math::Vector3 blendTranslate = node.translate;
//
//								animationPlayer->CalculateScale(animationNodeID, scale);
//								animationPlayer->CalculateRotate(animationNodeID, rotate);
//								animationPlayer->CalculateTranslate(animationNodeID, translate);
//
//								blendAnimationPlayer->CalculateScale(animationNodeID, blendScale);
//								blendAnimationPlayer->CalculateRotate(animationNodeID, blendRotate);
//								blendAnimationPlayer->CalculateTranslate(animationNodeID, blendTranslate);
//
//								node.scale = Math::Vector3Lerp(scale, blendScale, blendRate);
//								node.rotate = Math::QuaternionSlerp(rotate, blendRotate, blendRate);
//								node.translate = Math::Vector3Lerp(translate, blendTranslate, blendRate);
//							}
//						}
//					}
//					else
//					{
//						std::unique_ptr<Graphics::IAnimationPlayer>& animationPlayer = animations.at(currentMontage->basePoseID).player;
//						animationPlayer->Update(elapsedTime);
//
//						s32 layerCount = static_cast<s32>(currentMontage->layerPoses.size());
//						if (0 < layerCount)
//						{
//							UpdateLayer(currentMontage->layerPoses.data(), layerCount);
//						}
//
//						if (animationPlayer->IsPlaying())
//						{
//							s32 animationNodeCount = static_cast<s32>(nodes->size());
//							for (s32 animationNodeID = 0; animationNodeID < animationNodeCount; ++animationNodeID)
//							{
//								s16 bindNodeID = animations.at(currentMontage->basePoseID).bindNodeIDs.at(animationNodeID);
//								if (bindNodeID < 0) continue;
//
//								ModelObject::Node& node = nodes->at(animationNodeID);
//
//								Math::Vector3 scale = node.scale;
//								Math::Quaternion rotate = node.rotate;
//								Math::Vector3 translate = node.translate;
//
//								animationPlayer->CalculateScale(animationNodeID, scale);
//								animationPlayer->CalculateRotate(animationNodeID, rotate);
//								animationPlayer->CalculateTranslate(animationNodeID, translate);
//
//								node.scale = scale;
//								node.rotate = rotate;
//								node.translate = translate;
//							}
//						}
//					}
//				}
//			}
//
//			// �A�j���[�V�����m�[�h�ƃo�C���h
//			void BindAnimationNodes(Animation& animation)
//			{
//				const Graphics::AnimationData& data = animation.resource->GetAnimationData();
//				animation.bindNodeIDs.resize(data.nodeNames.size());
//
//				for (size_t i = 0; i < data.nodeNames.size(); ++i)
//				{
//					animation.bindNodeIDs.at(i) = -1;
//
//					const std::string& nodeName = data.nodeNames[i];
//					for (size_t j = 0; j < nodes->size(); ++j)
//					{
//						ModelObject::Node& node = nodes->at(j);
//						if (nodeName == node.name)
//						{
//							animation.bindNodeIDs.at(i) = static_cast<s16>(static_cast<s32>(j));
//						}
//					}
//				}
//			}
//
//			// �A�j���[�V�����N���b�v������
//			const Graphics::AnimationData::Clip* FindAnimationClip(const char* name)
//			{
//				for (Animation& animation : animations)
//				{
//					const Graphics::AnimationData& data = animation.resource->GetAnimationData();
//					for (const Graphics::AnimationData::Clip& clip : data.clips)
//					{
//						if (clip.name == name)
//						{
//							return &clip;
//						}
//					}
//				}
//				return nullptr;
//			}
//
//		public:
//			template<class Archive>
//			void serialize(Archive& archive, u32 version);
//		};
//	}
//}

#pragma once

#include <vector>
#include "Phoenix/Math/PhoenixMath.h"
#include "Phoenix/Math//Arithmetic.h"
#include "Phoenix/Graphics/GraphicsDevice.h"
#include "Phoenix/Graphics/Model.h"
#include "Phoenix/Graphics/Animation.h"
#include "Phoenix/Graphics/Texture.h"
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

		struct CollisionData
		{
			Phoenix::Math::Vector3 pos;
			Phoenix::f32 radius = 0.0f;
			Phoenix::s32 boneIndex = 0;
		};

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

			struct MeshNode
			{
				std::vector<Math::Matrix> boneTransform;
				u32 boneTransformCount = 0;

				MeshNode() {}
			};

			struct Material
			{
				std::string name;
				std::vector<std::unique_ptr<Graphics::ITexture>> textures;
				std::vector<Math::Color> colors;
			};

		private:
			std::shared_ptr<Graphics::IModelResource> modelResource;
			std::unique_ptr<Animator> animator;
			std::vector<Node> nodes;
			std::vector<MeshNode> meshNodes;
			std::vector<Material> materials;
			std::unique_ptr<OS::IResourceManager> resourceManamger;
			std::unique_ptr<OS::IFileStream> file;

			// �{�[�����񋓗p
			std::vector<const char*> boneNames;

		public:
			ModelObject() {}
			~ModelObject() {}

		public:
			// ����
			static std::unique_ptr<ModelObject> Create();

			// ������
			void Initialize(Graphics::IGraphicsDevice* graphicsDevice);

			// ���f���̓ǂݍ���
			void Load(Graphics::IGraphicsDevice* graphicsDevice, const char* filename);

			// �A�j���[�V�����̓ǂݍ���
			s32 LoadAnimation(const char* filename, s32 index);

			/// <summary>
			/// �A�j���[�V�������C���[�̒ǉ�
			/// </summary>
			/// <param name="beginNodeName"> �n�_�̃{�[���� </param>
			/// <param name="endNodeName"> �I�_�̃{�[���� </param>
			/// <returns> �A�j���[�V�������C���[�ԍ� </returns>
			s32 AddAnimationLayer(const s8* beginNodeName, const s8* endNodeName);

			/// <summary>
			/// �A�j���[�V�������C���[�̒ǉ�
			/// </summary>
			/// <param name="beginNodeIndex"> �n�_�̃{�[���ԍ� </param>
			/// <param name="endNodeIndex"> �I�_�̃{�[���ԍ� </param>
			/// <returns> �A�j���[�V�������C���[�ԍ� </returns>
			s32 AddAnimationLayer(s32 beginNodeIndex = -1, s32 endNodeIndex = -1);

			/// <summary>
			/// �A�j���[�V�������C���[�ɃA�j���[�V�����X�e�[�g��ǉ�
			/// </summary>
			/// <param name="addAnimationIndex"> �A�j���[�V�����ԍ� </param>
			/// <param name="animationLayerIndex"> �A�j���[�V�������C���[�ԍ� </param>
			/// <returns> �A�j���[�V�����X�e�[�g�ԍ� </returns>
			s32 AddAnimationStateToLayer(s32 addAnimationIndex, s32 animationLayerIndex);

			/// <summary>
			/// �A�j���[�V�������C���[�Ƀu�����h�c���[��ǉ�
			/// </summary>
			/// <param name="animationLayerIndex"> �A�j���[�V�������C���[�ԍ� </param>
			/// <returns> �u�����h�c���[�ԍ� </returns>
			s32 AddBlendTreeToLayer(s32 animationLayerIndex);

			/// <summary>
			/// �u�����h�c���[�Ƀu�����h�A�j���[�V�����X�e�[�g��ǉ�
			/// </summary>
			/// <param name="addAnimationIndex"> �A�j���[�V�����ԍ� </param>
			/// <param name="plot"> 2������̈ʒu(X�͈� : -1.0f ~ 1.0f, Y�͈� : -1.0f ~ 1.0f, Z�͈� : 0.0f) </param>
			/// <param name="animationLayerIndex"> �A�j���[�V�������C���[�ԍ� </param>
			/// <param name="blendTreeIndex"> �u�����h�c���[�ԍ� </param>
			/// <returns> �u�����h�c���[�̃X�e�[�g�ԍ� </returns>
			s32 AddBlendAnimationStateToBlendTree(s32 addAnimationIndex, Phoenix::Math::Vector3 plot, s32 animationLayerIndex, s32 blendTreeIndex);

			// �s����X�V
			void UpdateTransform(f32 elapsedTime);

			// ���[�J���ϊ��s����X�V
			void UpdateLocalTransform();

			// ���[���h�ϊ��s����X�V
			void UpdateWorldTransform();

			// �{�[���ϊ��s����X�V
			void UpdateBoneTransform();

			/// <summary>
			/// �w�背�C���[�̃X�e�[�g�̃A�j���[�V�����Đ�
			/// </summary>
			/// <param name="layerIndex"> �A�j���[�V�������C���[�ԍ� </param>
			/// <param name="stateIndex"> �A�j���[�V�����X�e�[�g�ԍ� </param>
			/// <param name="clip"> �A�j���[�V�����N���b�v�ԍ� </param>
			/// <param name="fadeTime"> ���̃A�j���[�V�����Ƀu�����h���Ă����X�s�[�h(�u�����h���[�g) </param>
			void PlayAnimation(u32 layerIndex, u32 stateIndex, u32 clip, f32 fadeTime = 0.0f);

			/// <summary>
			/// �w�背�C���[�̃u�����h�c���[�̃A�j���[�V�����Đ�
			/// </summary>
			/// <param name="layerIndex"> �A�j���[�V�������C���[�ԍ� </param>
			/// <param name="stateIndex"> �A�j���[�V�����X�e�[�g�ԍ� </param>
			/// <param name="clip"> �A�j���[�V�����N���b�v�ԍ� </param>
			/// <param name="fadeTime"> ���̃A�j���[�V�����Ƀu�����h���Ă����X�s�[�h(�u�����h���[�g) </param>
			void PlayBlendTreeAnimation(u32 layerIndex, u32 blendTreeIndex, u32 clip, f32 fadeTime = 0.0f);

			/// <summary>
			/// �w�背�C���[�̃X�e�[�g�̃A�j���[�V���������Đ�
			/// </summary>
			/// <param name="layerIndex"> �A�j���[�V�������C���[�ԍ� </param>
			/// <param name="stateIndex"> �A�j���[�V�����X�e�[�g�ԍ� </param>
			/// <param name="clip"> �A�j���[�V�����N���b�v�ԍ� </param>
			/// <param name="fadeTime"> ���̃A�j���[�V�����Ƀu�����h���Ă����X�s�[�h(�u�����h���[�g) </param>
			void SimultaneousPlayAnimation(u32 layerIndex, u32 blendTreeIndex, u32 clip, f32 fadeTime = 0.0f);

			/// <summary>
			/// �w�背�C���[�̃u�����h�c���[�̃A�j���[�V���������Đ�
			/// </summary>
			/// <param name="layerIndex"> �A�j���[�V�������C���[�ԍ� </param>
			/// <param name="stateIndex"> �A�j���[�V�����X�e�[�g�ԍ� </param>
			/// <param name="clip"> �A�j���[�V�����N���b�v�ԍ� </param>
			/// <param name="fadeTime"> ���̃A�j���[�V�����Ƀu�����h���Ă����X�s�[�h(�u�����h���[�g) </param>
			void SimultaneousPlayBlendTreeAniamation(u32 layerIndex, u32 blendTreeIndex, u32 clip, f32 fadeTime = 0.0f);

			// �ꎞ��~/�ĊJ
			void PauseAnimation(bool pause);

			// ��~
			void StopAnimation();

			// ���[�v�Đ��ݒ�
			void SetLoopAnimation(bool loop);

			// ���[�v�����Đ��ݒ�
			void SetBlendLoopAnimation(bool loop);

			// �Đ���
			bool IsPlaying();

			// ���[�v�Đ���
			bool IsLoopAnimation();

			// �A�j���[�V�����̍X�V
			void UpdateAnimation(f32 elapsedTime);

			// �Đ����Ԓ�(�b)�擾
			f32 GetLength();

			// �Đ����x�{���ݒ�
			void SetSpeed(f32 speed);

			// �Đ����x�{���擾
			f32 GetSpeed();

			// �u�����h����
			bool IsBlend();

			// �Đ��ʒu�ݒ�
			void SetCurrentTime(f32 seconds);

			// �Đ��ʒu�擾
			f32 GetCurrentTime();

			// �c��Đ����Ԏ擾
			f32 GetLastTime();

			// �Đ��J�n�ʒu�ݒ�
			void SetBeginTime(f32 seconds);

			// �Đ��J�n�ʒu�擾
			f32 GetBeginTime();

			// �Đ��I���ʒu�ݒ�
			void SetEndTime(f32 seconds);

			// �Đ��I���ʒu�擾
			f32 GetEndTime();

			// ���[�v�Đ��J�n�ʒu�ݒ�
			void SetLoopBeginTime(f32 seconds);

			// ���[�v�Đ��J�n�ʒu�擾
			f32 GetLoopBeginTime();

			// ���[�v�Đ��I���ʒu�ݒ�
			void SetLoopEndTime(f32 seconds);

			// ���[�v�Đ��I���ʒu�擾
			f32 GetLoopEndTime();

			// 
			void SetBlendRate(f32 rate);
			void SetBlendRate(Phoenix::Math::Vector3 rate);

			void SetHipID(const Phoenix::s8* nodeName);

			// ���f�����\�[�X�̎擾
			Graphics::IModelResource* GetModelResource() { return modelResource.get(); }

			// �m�[�h�̎擾
			std::vector<Node>* GetNodes() { return &nodes; }

			// �{�[�����擾
			std::vector<const char*> GetBoneNames() { return boneNames; }

			// �{�[���g�����X�t�H�[���̎擾
			Math::Matrix* GetBoneTransforms(u32 meshIndex) { return meshNodes.at(meshIndex).boneTransform.data(); }
			Math::Matrix GetBoneTransforms(u32 meshIndex, u32 boneIndex) { return meshNodes.at(meshIndex).boneTransform.at(boneIndex); }

			// �{�[���C���f�b�N�X�̎擾
			u32 GetBoneIndex(const char* name)
			{
				for (u32 i = 0; i < nodes.size(); ++i)
				{
					if (FND::StrCmp(nodes[i].name, name) == 0)
					{
						return i;
					}
				}

				return -1;
			}

			// ���b�V���m�[�h�̃T�C�Y�擾
			sizeT GetMeshNodes() { return meshNodes.size(); }

			// �{�[���g�����X�t�H�[���̃T�C�Y�擾
			u32 GetBoneTransformCount(u32 meshIndex) { return meshNodes.at(meshIndex).boneTransformCount; }

			// �}�e���A���̃e�N�X�`���擾
			Graphics::ITexture* GetTexture(u32 index, u32 texIndex)
			{
				return materials.at(index).textures.at(texIndex).get();
			}

			// �}�e���A���̃T�C�Y�擾
			sizeT GetMaterialSize() { return materials.size(); }

			// �}�e���A���̃e�N�X�`���T�C�Y�擾
			sizeT GetTextureSize(u32 index) { return materials.at(index).textures.size(); }
		};

#if 0
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

			struct BlendAnimation
			{
				Animation* blendCurrentAnimation = nullptr;
				Phoenix::Math::Vector3 plot;
			};

			struct AnimationLayer
			{
				Animation* baseAnimation = nullptr;
				std::vector<ModelObject::Node> nodes;
				std::vector<BlendAnimation> blendAnimations;
				s32 beginNodeIndex = -1;
				s32 endNodeIndex = -1;
			};

		private:
			std::vector<Animation> animations;
			std::vector<AnimationLayer> animationLayers;
			std::vector<ModelObject::Node>* nodes;
			std::unique_ptr<OS::IFileStream> file;

			//Animation* currentAnimation = nullptr;
			//Animation* blendCurrentAnimation = nullptr; // �����Đ��p

			AnimationLayer* currentAnimationLayer = nullptr;
			AnimationLayer* blendCurrentAnimationLayer[2] = { nullptr, nullptr };
			s32 blendCurrentAnimationLayerSize = 2;

			//f32 blendRate = 0.0f;
			Phoenix::Math::Vector3 blendRate = { 0.0f, 0.0f, 0.0f };

		public:
			void Initialize(ModelObject* model)
			{
				file = OS::IFileStream::Create();
				file->Initialize(nullptr);

				nodes = model->GetNodes();
			}

			// �A�j���[�V�������\�[�X�ǂݍ���
			s32 LoadResource(OS::IResourceManager* resourceManamger, const char* filename, s32 index)
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
						return -1;
					}
					Graphics::AnimationData data;
					if (!loader->Load(data))
					{
						return -1;
					}
					Graphics::AnimationData::Serialize(data, animation.filename.c_str());
				}
				LoadResource(resourceManamger, animation);

				return index;
			}

			void LoadResource(OS::IResourceManager* resourceManamger, Animation& animation)
			{
				animation.resource = resourceManamger->LoadImmediate<Graphics::IAnimationResource>(animation.filename.c_str());

				if (animation.resource)
				{
					animation.player = Graphics::IAnimationPlayer::Create();
					animation.player->Initialize(animation.resource);
					BindAnimationNodes(animation);
				}
			}

			void AddLayer(s32 setBaseAnimationIndex, const s8* beginNodeName, const s8* endNodeName)
			{
				s32 index = static_cast<s32>(animationLayers.size());
				animationLayers.emplace_back(AnimationLayer());

				animationLayers.at(index).nodes = *nodes;
				animationLayers.at(index).baseAnimation = &animations.at(setBaseAnimationIndex);

				if (beginNodeName == nullptr)
				{
					return;
				}
				if (endNodeName == nullptr)
				{
					return;
				}

				s32 animationNodeCount = static_cast<s32>(nodes->size());
				for (s32 animationNodeID = 0; animationNodeID < animationNodeCount; ++animationNodeID)
				{
					if (FND::StrCmp(nodes->at(animationNodeID).name, beginNodeName) == 0)
					{
						animationLayers.at(index).beginNodeIndex = animationNodeID;
					}
					if (FND::StrCmp(nodes->at(animationNodeID).name, endNodeName) == 0)
					{
						animationLayers.at(index).endNodeIndex = animationNodeID;
					}
				}
			}

			void AddLayer(s32 setBaseAnimationIndex, s32 beginNodeIndex, s32 endNodeIndex)
			{
				s32 index = static_cast<s32>(animationLayers.size());
				animationLayers.emplace_back(AnimationLayer());

				animationLayers.at(index).nodes = *nodes;
				animationLayers.at(index).baseAnimation = &animations.at(setBaseAnimationIndex);
				animationLayers.at(index).beginNodeIndex = beginNodeIndex;
				animationLayers.at(index).endNodeIndex = endNodeIndex;
			}

			void AddBlendAnimationToLayer(s32 addBlendAnimationIndex, s32 animationLayerIndex, Phoenix::Math::Vector3 plot)
			{
				s32 animationLayerCount = static_cast<s32>(animationLayers.size());
				if (animationLayerIndex < 0 || animationLayerCount <= animationLayerIndex)
				{
					return;
				}

				s32 animationBlendCount = static_cast<s32>(animationLayers.at(animationLayerIndex).blendAnimations.size());
				animationLayers.at(animationLayerIndex).blendAnimations.emplace_back(BlendAnimation());

				animationLayers.at(animationLayerIndex).blendAnimations.at(animationBlendCount).plot = plot;
				animationLayers.at(animationLayerIndex).blendAnimations.at(animationBlendCount).blendCurrentAnimation = &animations.at(addBlendAnimationIndex);
			}

			// �Đ�
			void Play(u32 layerIndex, u32 bank, u32 clip, f32 fadeTime = 0.0f)
			{
				if (layerIndex < 0 || layerIndex >= animationLayers.size())
				{
					return;
				}

				AnimationLayer& animatioLayer = animationLayers.at(layerIndex);
				if (!animatioLayer.baseAnimation->resource)
				{
					return;
				}
				const Graphics::AnimationData& data = animatioLayer.baseAnimation->resource->GetAnimationData();

				if (clip < 0 || clip >= data.clips.size())
				{
					return;
				}

				currentAnimationLayer = &animatioLayer;
				blendCurrentAnimationLayer[0] = nullptr;
				blendCurrentAnimationLayer[1] = nullptr;
				animatioLayer.baseAnimation->player->Play(clip);
				animatioLayer.baseAnimation->player->SetBlendTime(fadeTime);

				s32 blendAnimationsCount = static_cast<s32>(animatioLayer.blendAnimations.size());
				if (blendAnimationsCount <= 0)
				{
					return;
				}

				for (s32 blendAnimationsID = 0; blendAnimationsID < blendAnimationsCount; ++blendAnimationsID)
				{
					animatioLayer.blendAnimations.at(blendAnimationsID).blendCurrentAnimation->player->Play(clip);
					animatioLayer.blendAnimations.at(blendAnimationsID).blendCurrentAnimation->player->SetBlendTime(fadeTime);
				}
			}

			// �����Đ�
			void BlendPlay(u32 layerIndex, u32 bank, u32 clip, f32 fadeTime = 0.0f)
			{
				if (layerIndex < 0 || layerIndex >= animationLayers.size())
				{
					return;
				}

				AnimationLayer& animatioLayer = animationLayers.at(layerIndex);
				if (!animatioLayer.baseAnimation->resource)
				{
					return;
				}
				const Graphics::AnimationData& data = animatioLayer.baseAnimation->resource->GetAnimationData();

				if (clip < 0 || clip >= data.clips.size())
				{
					return;
				}

				if (blendCurrentAnimationLayer[0] == nullptr) blendCurrentAnimationLayer[0] = &animatioLayer;
				else if (blendCurrentAnimationLayer[1] == nullptr) blendCurrentAnimationLayer[1] = &animatioLayer;
				animatioLayer.baseAnimation->player->Play(clip);
				animatioLayer.baseAnimation->player->SetBlendTime(fadeTime);

				s32 blendAnimationsCount = static_cast<s32>(animatioLayer.blendAnimations.size());
				if (blendAnimationsCount <= 0)
				{
					return;
				}

				for (s32 blendAnimationsID = 0; blendAnimationsID < blendAnimationsCount; ++blendAnimationsID)
				{
					animatioLayer.blendAnimations.at(blendAnimationsID).blendCurrentAnimation->player->Play(clip);
					animatioLayer.blendAnimations.at(blendAnimationsID).blendCurrentAnimation->player->SetBlendTime(fadeTime);
				}
			}

			// �ꎞ��~/�ĊJ
			void Pause(bool pause)
			{
				currentAnimationLayer->baseAnimation->player->Pause(pause);

				s32 blendAnimationsCount = static_cast<s32>(currentAnimationLayer->blendAnimations.size());
				if (blendAnimationsCount <= 0)
				{
					return;
				}

				for (s32 blendAnimationsID = 0; blendAnimationsID < blendAnimationsCount; ++blendAnimationsID)
				{
					currentAnimationLayer->blendAnimations.at(blendAnimationsID).blendCurrentAnimation->player->Pause(pause);
				}
			}

			// ��~
			void Stop()
			{
				currentAnimationLayer->baseAnimation->player->Stop();

				s32 blendAnimationsCount = static_cast<s32>(currentAnimationLayer->blendAnimations.size());
				if (blendAnimationsCount <= 0)
				{
					return;
				}

				for (s32 blendAnimationsID = 0; blendAnimationsID < blendAnimationsCount; ++blendAnimationsID)
				{
					currentAnimationLayer->blendAnimations.at(blendAnimationsID).blendCurrentAnimation->player->Stop();
				}
			}

			// ���[�v�Đ��ݒ�
			void SetLoop(bool loop)
			{
				currentAnimationLayer->baseAnimation->player->SetLoop(loop);

				s32 blendAnimationsCount = static_cast<s32>(currentAnimationLayer->blendAnimations.size());
				if (blendAnimationsCount <= 0)
				{
					return;
				}

				for (s32 blendAnimationsID = 0; blendAnimationsID < blendAnimationsCount; ++blendAnimationsID)
				{
					currentAnimationLayer->blendAnimations.at(blendAnimationsID).blendCurrentAnimation->player->SetLoop(loop);
				}
			}

			// ���[�v�����Đ��ݒ�
			void SetBlendLoop(bool loop)
			{
				for (s32 i = 0; i < blendCurrentAnimationLayerSize; ++i)
				{
					if (blendCurrentAnimationLayer[i] != nullptr)
					{
						blendCurrentAnimationLayer[i]->baseAnimation->player->SetLoop(loop);

						s32 blendAnimationsCount = static_cast<s32>(blendCurrentAnimationLayer[i]->blendAnimations.size());
						if (blendAnimationsCount <= 0)
						{
							return;
						}

						for (s32 blendAnimationsID = 0; blendAnimationsID < blendAnimationsCount; ++blendAnimationsID)
						{
							blendCurrentAnimationLayer[i]->blendAnimations.at(blendAnimationsID).blendCurrentAnimation->player->SetLoop(loop);
						}
					}
				}
			}

			// �Đ���
			bool IsPlaying()
			{
				return currentAnimationLayer->baseAnimation->player->IsPlaying();
			}

			// ���[�v�Đ���
			bool IsLoop()
			{
				return currentAnimationLayer->baseAnimation->player->IsLoop();
			}

			// �Đ����Ԓ�(�b)�擾
			f32 GetLength()
			{
				return currentAnimationLayer->baseAnimation->player->GetLength();
			}

			// �Đ����x�{���ݒ�
			void SetSpeed(f32 speed)
			{
				currentAnimationLayer->baseAnimation->player->SetSpeed(speed);

				s32 blendAnimationsCount = static_cast<s32>(currentAnimationLayer->blendAnimations.size());
				if (blendAnimationsCount <= 0)
				{
					return;
				}

				for (s32 blendAnimationsID = 0; blendAnimationsID < blendAnimationsCount; ++blendAnimationsID)
				{
					currentAnimationLayer->blendAnimations.at(blendAnimationsID).blendCurrentAnimation->player->SetSpeed(speed);
				}
			}

			// �Đ����x�{���擾
			f32 GetSpeed()
			{
				return currentAnimationLayer->baseAnimation->player->GetSpeed();
			}

			// �u�����h����
			bool IsBlend()
			{
				return currentAnimationLayer->baseAnimation->player->IsBlend();
			}
			// �Đ��ʒu�ݒ�
			void SetCurrentTime(f32 seconds)
			{
				currentAnimationLayer->baseAnimation->player->SetCurrentTime(seconds);

				s32 blendAnimationsCount = static_cast<s32>(currentAnimationLayer->blendAnimations.size());
				if (blendAnimationsCount <= 0)
				{
					return;
				}

				for (s32 blendAnimationsID = 0; blendAnimationsID < blendAnimationsCount; ++blendAnimationsID)
				{
					currentAnimationLayer->blendAnimations.at(blendAnimationsID).blendCurrentAnimation->player->SetCurrentTime(seconds);
				}
			}

			// �Đ��ʒu�擾
			f32 GetCurrentTime()
			{
				return currentAnimationLayer->baseAnimation->player->GetCurrentTime();
			}

			// �c��Đ����Ԏ擾
			f32 GetLastTime()
			{
				return currentAnimationLayer->baseAnimation->player->GetLastTime();
			}

			// �Đ��J�n�ʒu�ݒ�
			void SetBeginTime(f32 seconds)
			{
				currentAnimationLayer->baseAnimation->player->SetBeginTime(seconds);

				s32 blendAnimationsCount = static_cast<s32>(currentAnimationLayer->blendAnimations.size());
				if (blendAnimationsCount <= 0)
				{
					return;
				}

				for (s32 blendAnimationsID = 0; blendAnimationsID < blendAnimationsCount; ++blendAnimationsID)
				{
					currentAnimationLayer->blendAnimations.at(blendAnimationsID).blendCurrentAnimation->player->SetBeginTime(seconds);
				}
			}

			// �Đ��J�n�ʒu�擾
			f32 GetBeginTime()
			{
				return currentAnimationLayer->baseAnimation->player->GetBeginTime();
			}

			// �Đ��I���ʒu�ݒ�
			void SetEndTime(f32 seconds)
			{
				currentAnimationLayer->baseAnimation->player->SetEndTime(seconds);

				s32 blendAnimationsCount = static_cast<s32>(currentAnimationLayer->blendAnimations.size());
				if (blendAnimationsCount <= 0)
				{
					return;
				}

				for (s32 blendAnimationsID = 0; blendAnimationsID < blendAnimationsCount; ++blendAnimationsID)
				{
					currentAnimationLayer->blendAnimations.at(blendAnimationsID).blendCurrentAnimation->player->GetBeginTime();
				}
			}

			// �Đ��I���ʒu�擾
			f32 GetEndTime()
			{
				return currentAnimationLayer->baseAnimation->player->GetEndTime();
			}

			// ���[�v�Đ��J�n�ʒu�ݒ�
			void SetLoopBeginTime(f32 seconds)
			{
				currentAnimationLayer->baseAnimation->player->SetLoopBeginTime(seconds);

				s32 blendAnimationsCount = static_cast<s32>(currentAnimationLayer->blendAnimations.size());
				if (blendAnimationsCount <= 0)
				{
					return;
				}

				for (s32 blendAnimationsID = 0; blendAnimationsID < blendAnimationsCount; ++blendAnimationsID)
				{
					currentAnimationLayer->blendAnimations.at(blendAnimationsID).blendCurrentAnimation->player->SetLoopBeginTime(seconds);
				}
			}

			// ���[�v�Đ��J�n�ʒu�擾
			f32 GetLoopBeginTime()
			{
				return currentAnimationLayer->baseAnimation->player->GetLoopBeginTime();
			}

			// ���[�v�Đ��I���ʒu�ݒ�
			void SetLoopEndTime(f32 seconds)
			{
				currentAnimationLayer->baseAnimation->player->SetLoopEndTime(seconds);

				s32 blendAnimationsCount = static_cast<s32>(currentAnimationLayer->blendAnimations.size());
				if (blendAnimationsCount <= 0)
				{
					return;
				}

				for (s32 blendAnimationsID = 0; blendAnimationsID < blendAnimationsCount; ++blendAnimationsID)
				{
					currentAnimationLayer->blendAnimations.at(blendAnimationsID).blendCurrentAnimation->player->SetLoopEndTime(seconds);
				}
			}

			// ���[�v�Đ��I���ʒu�擾
			f32 GetLoopEndTime()
			{
				return currentAnimationLayer->baseAnimation->player->GetLoopEndTime();
			}

			// �u�����h���[�g�̐ݒ�
			void SetBlendRate(f32 rate)
			{
				blendRate.x = rate;
				blendRate.y = 0.0f;
				blendRate.z = 0.0f;
			}

			void SetBlendRate(Phoenix::Math::Vector3 rate)
			{
				blendRate = rate;
			}

			// �A�j���[�V�����o���N�C���f�b�N�X�擾
			u32 GetAnimationBankIndex(const char* name) const
			{
				for (size_t i = 0; i < animations.size(); ++i)
				{
					if (0 == FND::StrCmp(name, OS::Path::GetFileNameWithoutExtension(animations.at(i).filename.c_str())))
					{
						return static_cast<u32>(i);
					}
				}
				return -1;
			}

			void NodeBlend(ModelObject::Node& currentNode, Math::Vector3& scale, Math::Quaternion& rotate, Math::Vector3& translate, Math::Vector3& blendScale, Math::Quaternion& blendRotate, Math::Vector3& blendTranslate, f32 blendRate, Graphics::IAnimationPlayer* player, s32 nodeID)
			{
				player->CalculateScale(nodeID, blendScale);
				player->CalculateRotate(nodeID, blendRotate);
				player->CalculateTranslate(nodeID, blendTranslate);

				currentNode.scale = Math::Vector3Lerp(scale, blendScale, fabsf(blendRate));
				currentNode.rotate = Math::QuaternionSlerp(rotate, blendRotate, fabsf(blendRate));
				currentNode.translate = Math::Vector3Lerp(translate, blendTranslate, fabsf(blendRate));
			}

			void CheckBlendOfEachAxis(ModelObject::Node& currentNode, Math::Vector3& scale, Math::Quaternion& rotate, Math::Vector3& translate, Math::Vector3& blendScale, Math::Quaternion& blendRotate, Math::Vector3& blendTranslate, f32 blendRate, f32 plot, Graphics::IAnimationPlayer* player, s32 nodeID)
			{
				if (!(fabsf(blendRate - 0.0f) <= Phoenix::Math::Epsilon))
				{
					if (0.0f < blendRate)
					{
						if (0.0f < plot)
						{
							NodeBlend(currentNode, scale, rotate, translate, blendScale, blendRotate, blendTranslate, blendRate, player, nodeID);
						}
					}
					else if (blendRate < 0.0f)
					{
						if (plot < 0.0f)
						{
							NodeBlend(currentNode, scale, rotate, translate, blendScale, blendRotate, blendTranslate, blendRate, player, nodeID);
						}
					}
				}
			}

			void UpdateLayer(AnimationLayer* animationLayer, f32 elapsedTime)
			{
				std::unique_ptr<Graphics::IAnimationPlayer>& animationPlayer = animationLayer->baseAnimation->player;
				animationPlayer->Update(elapsedTime);

				// ���C���[���̃u�����h
				{
					s32 blendAnimationCount = static_cast<s32>(animationLayer->blendAnimations.size());
					if (0 < blendAnimationCount)
					{
						for (s32 blendAnimationsID = 0; blendAnimationsID < blendAnimationCount; ++blendAnimationsID)
						{
							animationLayer->blendAnimations.at(blendAnimationsID).blendCurrentAnimation->player->Update(elapsedTime);
						}
					}
					if (animationPlayer->IsPlaying())
					{
						s32 animationNodeCount = static_cast<s32>(animationLayer->nodes.size());
						for (s32 animationNodeID = 0; animationNodeID < animationNodeCount; ++animationNodeID)
						{
							s16 bindNodeID = animationLayer->baseAnimation->bindNodeIDs.at(animationNodeID);
							if (bindNodeID < 0) continue;

							ModelObject::Node& node = nodes->at(animationNodeID);
							ModelObject::Node& totalNode = animationLayer->nodes.at(animationNodeID);
							ModelObject::Node currentNodeX = animationLayer->nodes.at(animationNodeID);
							ModelObject::Node currentNodeY = animationLayer->nodes.at(animationNodeID);

							Math::Vector3 scale = node.scale;
							Math::Quaternion rotate = node.rotate;
							Math::Vector3 translate = node.translate;

							Math::Vector3 blendScale[] = { node.scale, node.scale };
							Math::Quaternion blendRotate[] = { node.rotate, node.rotate };
							Math::Vector3 blendTranslate[] = { node.translate, node.translate };

							animationPlayer->CalculateScale(animationNodeID, scale);
							animationPlayer->CalculateRotate(animationNodeID, rotate);
							animationPlayer->CalculateTranslate(animationNodeID, translate);

							if (blendRate == Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f) || blendAnimationCount <= 0)
							{
								totalNode.scale = scale;
								totalNode.rotate = rotate;
								totalNode.translate = translate;
							}
							else
							{
								for (s32 blendAnimationID = 0; blendAnimationID < blendAnimationCount; ++blendAnimationID)
								{
									CheckBlendOfEachAxis
									(
										currentNodeX,
										scale, rotate, translate, 
										blendScale[0], blendRotate[0], blendTranslate[0],
										blendRate.x, 
										animationLayer->blendAnimations.at(blendAnimationID).plot.x, 
										animationLayer->blendAnimations.at(blendAnimationID).blendCurrentAnimation->player.get(), 
										animationNodeID
									);
									CheckBlendOfEachAxis
									(
										currentNodeY,
										scale, rotate, translate,
										blendScale[1], blendRotate[1], blendTranslate[1],
										blendRate.y,
										animationLayer->blendAnimations.at(blendAnimationID).plot.y,
										animationLayer->blendAnimations.at(blendAnimationID).blendCurrentAnimation->player.get(),
										animationNodeID
									);
								}

								totalNode.scale = Math::Vector3Lerp(currentNodeX.scale, currentNodeY.scale, fabsf(blendRate.y));
								totalNode.rotate = Math::QuaternionSlerp(currentNodeX.rotate, currentNodeY.rotate, fabsf(blendRate.y));
								totalNode.translate = Math::Vector3Lerp(currentNodeX.translate, currentNodeY.translate, fabsf(blendRate.y));
							}
						}
					}
				}
			}

			void Update(f32 elapsedTime)
			{
				if (currentAnimationLayer != nullptr)
				{
					if (blendCurrentAnimationLayer[0] != nullptr)
					{
						if (blendCurrentAnimationLayer[1] != nullptr)
						{
							UpdateLayer(currentAnimationLayer, elapsedTime);
							UpdateLayer(blendCurrentAnimationLayer[0], elapsedTime);
							UpdateLayer(blendCurrentAnimationLayer[1], elapsedTime);

							s32 animationNodeCount = static_cast<s32>(nodes->size());
							for (s32 animationNodeID = 0; animationNodeID < animationNodeCount; ++animationNodeID)
							{
								s16 bindNodeID = currentAnimationLayer->baseAnimation->bindNodeIDs.at(animationNodeID);
								if (bindNodeID < 0) continue;

								ModelObject::Node& node = nodes->at(animationNodeID);
								ModelObject::Node& currentNode = currentAnimationLayer->nodes.at(animationNodeID);
								ModelObject::Node& blendCurrentNode01 = blendCurrentAnimationLayer[0]->nodes.at(animationNodeID);
								ModelObject::Node& blendCurrentNode02 = blendCurrentAnimationLayer[1]->nodes.at(animationNodeID);

								Math::Vector3 scale = currentNode.scale;
								Math::Quaternion rotate = currentNode.rotate;
								Math::Vector3 translate = currentNode.translate;

								if (blendCurrentAnimationLayer[0]->beginNodeIndex <= animationNodeID && animationNodeID <= blendCurrentAnimationLayer[0]->endNodeIndex)
								{
									scale = blendCurrentNode01.scale;
									rotate = blendCurrentNode01.rotate;
									translate = blendCurrentNode01.translate;
								}
								if (blendCurrentAnimationLayer[1]->beginNodeIndex <= animationNodeID && animationNodeID <= blendCurrentAnimationLayer[1]->endNodeIndex)
								{
									scale = blendCurrentNode02.scale;
									rotate = blendCurrentNode02.rotate;
									translate = blendCurrentNode02.translate;
								}

								node.scale = scale;
								node.rotate = rotate;
								node.translate = translate;
							}
						}
						else
						{
							UpdateLayer(currentAnimationLayer, elapsedTime);
							UpdateLayer(blendCurrentAnimationLayer[0], elapsedTime);

							s32 animationNodeCount = static_cast<s32>(nodes->size());
							for (s32 animationNodeID = 0; animationNodeID < animationNodeCount; ++animationNodeID)
							{
								s16 bindNodeID = currentAnimationLayer->baseAnimation->bindNodeIDs.at(animationNodeID);
								if (bindNodeID < 0) continue;

								ModelObject::Node& node = nodes->at(animationNodeID);
								ModelObject::Node& currentNode = currentAnimationLayer->nodes.at(animationNodeID);
								ModelObject::Node& blendCurrentNode = blendCurrentAnimationLayer[0]->nodes.at(animationNodeID);

								Math::Vector3 scale = currentNode.scale;
								Math::Quaternion rotate = currentNode.rotate;
								Math::Vector3 translate = currentNode.translate;

								if (animationNodeID == 1 || (blendCurrentAnimationLayer[0]->beginNodeIndex <= animationNodeID && animationNodeID <= blendCurrentAnimationLayer[0]->endNodeIndex))
								{
									scale = blendCurrentNode.scale;
									rotate = blendCurrentNode.rotate;
									translate = blendCurrentNode.translate;
								}

								node.scale = scale;
								node.rotate = rotate;
								node.translate = translate;
							}
						}
					}
					else
					{
						UpdateLayer(currentAnimationLayer, elapsedTime);

						s32 animationNodeCount = static_cast<s32>(nodes->size());
						for (s32 animationNodeID = 0; animationNodeID < animationNodeCount; ++animationNodeID)
						{
							s16 bindNodeID = currentAnimationLayer->baseAnimation->bindNodeIDs.at(animationNodeID);
							if (bindNodeID < 0) continue;

							ModelObject::Node& node = nodes->at(animationNodeID);
							ModelObject::Node& currentNode = currentAnimationLayer->nodes.at(animationNodeID);

							Math::Vector3 scale = currentNode.scale;
							Math::Quaternion rotate = currentNode.rotate;
							Math::Vector3 translate = currentNode.translate;

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
#else 
	
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

			struct AnimationState
			{
				Animation* animation = nullptr;
			};
		
			struct BlendAnimationState
			{
				Animation* animation = nullptr;
				Phoenix::Math::Vector3 plot;
			};

			struct BlendTree
			{
				std::vector<BlendAnimationState> states;
				Phoenix::Math::Vector3 late;
			};
		
			struct AnimationLayer
			{
				std::vector<AnimationState> states;
				std::vector<BlendTree> blendTrees;

				AnimationState* currentState;
				BlendTree* currentBlendTree;

				s32 beginNodeIndex = -1;
				s32 endNodeIndex = -1;
			};
		
		private:
			std::vector<Animation> animations;
			std::vector<AnimationLayer> animationLayers;
			std::vector<ModelObject::Node>* nodes;
			std::unique_ptr<OS::IFileStream> file;
		
			AnimationLayer* currentAnimationLayer = nullptr;
			AnimationLayer* blendCurrentAnimationLayer[2] = { nullptr, nullptr };
			s32 blendCurrentAnimationLayerSize = 2;
		
			//f32 blendRate = 0.0f;
			Phoenix::Math::Vector3 blendRate = { 0.0f, 0.0f, 0.0f };

			Phoenix::u32 hipNodeID = 0;
		
		public:
			void Initialize(ModelObject* model)
			{
				file = OS::IFileStream::Create();
				file->Initialize(nullptr);
		
				nodes = model->GetNodes();
			}
		
			// �A�j���[�V�������\�[�X�ǂݍ���
			s32 LoadResource(OS::IResourceManager* resourceManamger, const char* filename, s32 index)
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
						return -1;
					}
					Graphics::AnimationData data;
					if (!loader->Load(data))
					{
						return -1;
					}
					Graphics::AnimationData::Serialize(data, animation.filename.c_str());
				}
				LoadResource(resourceManamger, animation);
		
				return index;
			}
		
			void LoadResource(OS::IResourceManager* resourceManamger, Animation& animation)
			{
				animation.resource = resourceManamger->LoadImmediate<Graphics::IAnimationResource>(animation.filename.c_str());
		
				if (animation.resource)
				{
					animation.player = Graphics::IAnimationPlayer::Create();
					animation.player->Initialize(animation.resource);
					BindAnimationNodes(animation);
				}
			}

			/// <summary>
			/// �A�j���[�V�������C���[�̒ǉ�
			/// </summary>
			/// <param name="beginNodeName"> �n�_�̃{�[���� </param>
			/// <param name="endNodeName"> �I�_�̃{�[���� </param>
			/// <returns> �A�j���[�V�������C���[�ԍ� </returns>
			s32 AddLayer(const s8* beginNodeName, const s8* endNodeName)
			{
				s32 index = static_cast<s32>(animationLayers.size());
				animationLayers.emplace_back(AnimationLayer());

				if (beginNodeName == nullptr)
				{
					return index;
				}
				if (endNodeName == nullptr)
				{
					return index;
				}

				s32 animationNodeCount = static_cast<s32>(nodes->size());
				for (s32 animationNodeID = 0; animationNodeID < animationNodeCount; ++animationNodeID)
				{
					if (FND::StrCmp(nodes->at(animationNodeID).name, beginNodeName) == 0)
					{
						animationLayers.at(index).beginNodeIndex = animationNodeID;
					}
					if (FND::StrCmp(nodes->at(animationNodeID).name, endNodeName) == 0)
					{
						animationLayers.at(index).endNodeIndex = animationNodeID;
					}
				}

				return index;
			}
			
			/// <summary>
			/// �A�j���[�V�������C���[�̒ǉ�
			/// </summary>
			/// <param name="beginNodeIndex"> �n�_�̃{�[���ԍ� </param>
			/// <param name="endNodeIndex"> �I�_�̃{�[���ԍ� </param>
			/// <returns> �A�j���[�V�������C���[�ԍ� </returns>
			s32 AddLayer(s32 beginNodeIndex = -1, s32 endNodeIndex = -1)
			{
				s32 index = static_cast<s32>(animationLayers.size());
				animationLayers.emplace_back(AnimationLayer());
				animationLayers.at(index).beginNodeIndex = beginNodeIndex;
				animationLayers.at(index).endNodeIndex = endNodeIndex;

				return index;
			}

			/// <summary>
			/// �A�j���[�V�������C���[�ɃA�j���[�V�����X�e�[�g��ǉ�
			/// </summary>
			/// <param name="addAnimationIndex"> �A�j���[�V�����ԍ� </param>
			/// <param name="animationLayerIndex"> �A�j���[�V�������C���[�ԍ� </param>
			/// <returns> �A�j���[�V�����X�e�[�g�ԍ� </returns>
			s32 AddAnimationStateToLayer(s32 addAnimationIndex, s32 animationLayerIndex)
			{
				s32 animationLayerCount = static_cast<s32>(animationLayers.size());
				if (animationLayerIndex < 0 || animationLayerCount <= animationLayerIndex)
				{
					return -1;
				}

				s32 stateCount = static_cast<s32>(animationLayers.at(animationLayerIndex).states.size());
				animationLayers.at(animationLayerIndex).states.emplace_back(AnimationState());
				animationLayers.at(animationLayerIndex).states.at(stateCount).animation = &animations.at(addAnimationIndex);

				return stateCount;
			}

			/// <summary>
			/// �A�j���[�V�������C���[�Ƀu�����h�c���[��ǉ�
			/// </summary>
			/// <param name="animationLayerIndex"> �A�j���[�V�������C���[�ԍ� </param>
			/// <returns> �u�����h�c���[�ԍ� </returns>
			s32 AddBlendTreeToLayer(s32 animationLayerIndex)
			{
				s32 animationLayerCount = static_cast<s32>(animationLayers.size());
				if (animationLayerIndex < 0 || animationLayerCount <= animationLayerIndex)
				{
					return -1;
				}

				s32 blendTreeCount = static_cast<s32>(animationLayers.at(animationLayerIndex).blendTrees.size());
				animationLayers.at(animationLayerIndex).blendTrees.emplace_back(BlendTree());
				animationLayers.at(animationLayerIndex).blendTrees.at(blendTreeCount).late = Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f);

				return blendTreeCount;
			}

			/// <summary>
			/// �u�����h�c���[�Ƀu�����h�A�j���[�V�����X�e�[�g��ǉ�
			/// </summary>
			/// <param name="addAnimationIndex"> �A�j���[�V�����ԍ� </param>
			/// <param name="plot"> 2������̈ʒu(X�͈� : -1.0f ~ 1.0f, Y�͈� : -1.0f ~ 1.0f, Z�͈� : 0.0f) </param>
			/// <param name="animationLayerIndex"> �A�j���[�V�������C���[�ԍ� </param>
			/// <param name="blendTreeIndex"> �u�����h�c���[�ԍ� </param>
			/// <returns> �u�����h�c���[�̃X�e�[�g�ԍ� </returns>
			s32 AddBlendAnimationStateToBlendTree(s32 addAnimationIndex, Phoenix::Math::Vector3 plot, s32 animationLayerIndex, s32 blendTreeIndex)
			{
				s32 animationLayerCount = static_cast<s32>(animationLayers.size());
				if (animationLayerIndex < 0 || animationLayerCount <= animationLayerIndex)
				{
					return -1;
				}

				s32 blendTreeCount = static_cast<s32>(animationLayers.at(animationLayerIndex).blendTrees.size());
				if (blendTreeIndex < 0 || blendTreeCount <= blendTreeIndex)
				{
					return -1;
				}

				s32 stateCount = static_cast<s32>(animationLayers.at(animationLayerIndex).blendTrees.at(blendTreeIndex).states.size());
				animationLayers.at(animationLayerIndex).blendTrees.at(blendTreeIndex).states.emplace_back(BlendAnimationState());
				animationLayers.at(animationLayerIndex).blendTrees.at(blendTreeIndex).states.at(stateCount).animation = &animations.at(addAnimationIndex);
				animationLayers.at(animationLayerIndex).blendTrees.at(blendTreeIndex).states.at(stateCount).plot = plot;

				return stateCount;
			}
		
			/// <summary>
			/// �w�背�C���[�̃X�e�[�g�̃A�j���[�V�����Đ�
			/// </summary>
			/// <param name="layerIndex"> �A�j���[�V�������C���[�ԍ� </param>
			/// <param name="stateIndex"> �A�j���[�V�����X�e�[�g�ԍ� </param>
			/// <param name="clip"> �A�j���[�V�����N���b�v�ԍ� </param>
			/// <param name="fadeTime"> ���̃A�j���[�V�����Ƀu�����h���Ă����X�s�[�h(�u�����h���[�g) </param>
			void Play(u32 layerIndex, u32 stateIndex, u32 clip, f32 fadeTime = 0.0f)
			{
				if (layerIndex < 0 || layerIndex >= animationLayers.size())
				{
					return;
				}

				AnimationLayer& animatioLayer = animationLayers.at(layerIndex);
				if (stateIndex < 0 || stateIndex >= animatioLayer.states.size())
				{
					return;
				}

				AnimationState& animatioState = animatioLayer.states.at(stateIndex);
				if (!animatioState.animation->resource)
				{
					return;
				}

				const Graphics::AnimationData& data = animatioState.animation->resource->GetAnimationData();
				if (clip < 0 || clip >= data.clips.size())
				{
					return;
				}
		
				currentAnimationLayer = &animatioLayer;
				blendCurrentAnimationLayer[0] = nullptr;
				blendCurrentAnimationLayer[1] = nullptr;

				animatioLayer.currentState = &animatioState;
				animatioLayer.currentState->animation->player->Play(clip);
				animatioLayer.currentState->animation->player->SetBlendTime(fadeTime);
				animatioLayer.currentBlendTree = nullptr;
			}
			
			/// <summary>
			/// �w�背�C���[�̃u�����h�c���[�̃A�j���[�V�����Đ�
			/// </summary>
			/// <param name="layerIndex"> �A�j���[�V�������C���[�ԍ� </param>
			/// <param name="stateIndex"> �A�j���[�V�����X�e�[�g�ԍ� </param>
			/// <param name="clip"> �A�j���[�V�����N���b�v�ԍ� </param>
			/// <param name="fadeTime"> ���̃A�j���[�V�����Ƀu�����h���Ă����X�s�[�h(�u�����h���[�g) </param>
			void PlayBlendTree(u32 layerIndex, u32 blendTreeIndex, u32 clip, f32 fadeTime = 0.0f)
			{
				if (layerIndex < 0 || layerIndex >= animationLayers.size())
				{
					return;
				}

				AnimationLayer& animatioLayer = animationLayers.at(layerIndex);
				if (blendTreeIndex < 0 || blendTreeIndex >= animatioLayer.blendTrees.size())
				{
					return;
				}

				BlendTree& blendTree = animatioLayer.blendTrees.at(blendTreeIndex);
				for (auto& state : blendTree.states)
				{
					if (!state.animation->resource)
					{
						return;
					}
				}

				for (auto& state : blendTree.states)
				{
					const Graphics::AnimationData& data = state.animation->resource->GetAnimationData();
					if (clip < 0 || clip >= data.clips.size())
					{
						return;
					}
				}

				currentAnimationLayer = &animatioLayer;
				blendCurrentAnimationLayer[0] = nullptr;
				blendCurrentAnimationLayer[1] = nullptr;

				animatioLayer.currentState = nullptr;
				animatioLayer.currentBlendTree = &blendTree;
				for (auto& state : blendTree.states)
				{
					state.animation->player->Play(clip);
					state.animation->player->SetBlendTime(fadeTime);
				}
			}
		
			/// <summary>
			/// �w�背�C���[�̃X�e�[�g�̃A�j���[�V���������Đ�
			/// </summary>
			/// <param name="layerIndex"> �A�j���[�V�������C���[�ԍ� </param>
			/// <param name="stateIndex"> �A�j���[�V�����X�e�[�g�ԍ� </param>
			/// <param name="clip"> �A�j���[�V�����N���b�v�ԍ� </param>
			/// <param name="fadeTime"> ���̃A�j���[�V�����Ƀu�����h���Ă����X�s�[�h(�u�����h���[�g) </param>
			void BlendPlay(u32 layerIndex, u32 stateIndex, u32 clip, f32 fadeTime = 0.0f)
			{
				if (layerIndex < 0 || layerIndex >= animationLayers.size())
				{
					return;
				}

				AnimationLayer& animatioLayer = animationLayers.at(layerIndex);
				if (stateIndex < 0 || stateIndex >= animatioLayer.states.size())
				{
					return;
				}

				AnimationState& animatioState = animatioLayer.states.at(stateIndex);
				if (!animatioState.animation->resource)
				{
					return;
				}

				const Graphics::AnimationData& data = animatioState.animation->resource->GetAnimationData();
				if (clip < 0 || clip >= data.clips.size())
				{
					return;
				}

				if (blendCurrentAnimationLayer[0] == nullptr) blendCurrentAnimationLayer[0] = &animatioLayer;
				else if (blendCurrentAnimationLayer[1] == nullptr) blendCurrentAnimationLayer[1] = &animatioLayer;

				animatioLayer.currentState = &animatioState;
				animatioLayer.currentState->animation->player->Play(clip);
				animatioLayer.currentState->animation->player->SetBlendTime(fadeTime);
				animatioLayer.currentBlendTree = nullptr;
			}

			/// <summary>
			/// �w�背�C���[�̃u�����h�c���[�̃A�j���[�V���������Đ�
			/// </summary>
			/// <param name="layerIndex"> �A�j���[�V�������C���[�ԍ� </param>
			/// <param name="stateIndex"> �A�j���[�V�����X�e�[�g�ԍ� </param>
			/// <param name="clip"> �A�j���[�V�����N���b�v�ԍ� </param>
			/// <param name="fadeTime"> ���̃A�j���[�V�����Ƀu�����h���Ă����X�s�[�h(�u�����h���[�g) </param>
			void BlendPlayBlendTree(u32 layerIndex, u32 blendTreeIndex, u32 clip, f32 fadeTime = 0.0f)
			{
				if (layerIndex < 0 || layerIndex >= animationLayers.size())
				{
					return;
				}

				AnimationLayer& animatioLayer = animationLayers.at(layerIndex);
				if (blendTreeIndex < 0 || blendTreeIndex >= animatioLayer.blendTrees.size())
				{
					return;
				}

				BlendTree& blendTree = animatioLayer.blendTrees.at(blendTreeIndex);
				for (auto& state : blendTree.states)
				{
					if (!state.animation->resource)
					{
						return;
					}
				}

				for (auto& state : blendTree.states)
				{
					const Graphics::AnimationData& data = state.animation->resource->GetAnimationData();
					if (clip < 0 || clip >= data.clips.size())
					{
						return;
					}
				}

				if (blendCurrentAnimationLayer[0] == nullptr) blendCurrentAnimationLayer[0] = &animatioLayer;
				else if (blendCurrentAnimationLayer[1] == nullptr) blendCurrentAnimationLayer[1] = &animatioLayer;

				animatioLayer.currentState = nullptr;
				animatioLayer.currentBlendTree = &blendTree;
				for (auto& state : blendTree.states)
				{
					state.animation->player->Play(clip);
					state.animation->player->SetBlendTime(fadeTime);
				}
			}
		
			// �ꎞ��~/�ĊJ
			void Pause(bool pause)
			{
				if (!currentAnimationLayer) return;
				if (currentAnimationLayer->currentState)
				{
					currentAnimationLayer->currentState->animation->player->Pause(pause);
				}
				else if (currentAnimationLayer->currentBlendTree)
				{
					for (auto& state : currentAnimationLayer->currentBlendTree->states)
					{
						state.animation->player->Pause(pause);
					}
				}
			}
		
			// ��~
			void Stop()
			{
				if (!currentAnimationLayer) return;
				if (currentAnimationLayer->currentState)
				{
					currentAnimationLayer->currentState->animation->player->Stop();
				}
				else if (currentAnimationLayer->currentBlendTree)
				{
					for (auto& state : currentAnimationLayer->currentBlendTree->states)
					{
						state.animation->player->Stop();
					}
				}
			}
		
			// ���[�v�Đ��ݒ�
			void SetLoop(bool loop)
			{
				if (!currentAnimationLayer) return;
				if (currentAnimationLayer->currentState)
				{
					currentAnimationLayer->currentState->animation->player->SetLoop(loop);
				}
				else if (currentAnimationLayer->currentBlendTree)
				{
					for (auto& state : currentAnimationLayer->currentBlendTree->states)
					{
						state.animation->player->SetLoop(loop);
					}
				}
			}
		
			// ���[�v�����Đ��ݒ�
			void SetBlendLoop(bool loop)
			{
				for (s32 i = 0; i < blendCurrentAnimationLayerSize; ++i)
				{
					if (!blendCurrentAnimationLayer[i]) continue;
					if (blendCurrentAnimationLayer[i]->currentState)
					{
						blendCurrentAnimationLayer[i]->currentState->animation->player->SetLoop(loop);
					}
					else if (blendCurrentAnimationLayer[i]->currentBlendTree)
					{
						for (auto& state : blendCurrentAnimationLayer[i]->currentBlendTree->states)
						{
							state.animation->player->SetLoop(loop);
						}
					}
				}
			}
		
			// �Đ���
			bool IsPlaying()
			{
				if (currentAnimationLayer->currentState)
				{
					return currentAnimationLayer->currentState->animation->player->IsPlaying();
				}
				else if (currentAnimationLayer->currentBlendTree)
				{
					return currentAnimationLayer->currentBlendTree->states.at(0).animation->player->IsPlaying();
				}

				return false;
			}
		
			// ���[�v�Đ���
			bool IsLoop()
			{
				return currentAnimationLayer->currentState->animation->player->IsLoop();
			}
		
			// �Đ����Ԓ�(�b)�擾
			f32 GetLength()
			{
				return currentAnimationLayer->currentState->animation->player->GetLength();
			}
		
			// �Đ����x�{���ݒ�
			void SetSpeed(f32 speed)
			{
				currentAnimationLayer->currentState->animation->player->SetSpeed(speed);
			}
		
			// �Đ����x�{���擾
			f32 GetSpeed()
			{
				return currentAnimationLayer->currentState->animation->player->GetSpeed();
			}
		
			// �u�����h����
			bool IsBlend()
			{
				return currentAnimationLayer->currentState->animation->player->IsBlend();
			}
			// �Đ��ʒu�ݒ�
			void SetCurrentTime(f32 seconds)
			{
				currentAnimationLayer->currentState->animation->player->SetCurrentTime(seconds);
			}
		
			// �Đ��ʒu�擾
			f32 GetCurrentTime()
			{
				return currentAnimationLayer->currentState->animation->player->GetCurrentTime();
			}
		
			// �c��Đ����Ԏ擾
			f32 GetLastTime()
			{
				return currentAnimationLayer->currentState->animation->player->GetLastTime();
			}
		
			// �Đ��J�n�ʒu�ݒ�
			void SetBeginTime(f32 seconds)
			{
				currentAnimationLayer->currentState->animation->player->SetBeginTime(seconds);
			}
		
			// �Đ��J�n�ʒu�擾
			f32 GetBeginTime()
			{
				return currentAnimationLayer->currentState->animation->player->GetBeginTime();
			}
		
			// �Đ��I���ʒu�ݒ�
			void SetEndTime(f32 seconds)
			{
				currentAnimationLayer->currentState->animation->player->SetEndTime(seconds);
			}
		
			// �Đ��I���ʒu�擾
			f32 GetEndTime()
			{
				return currentAnimationLayer->currentState->animation->player->GetEndTime();
			}
		
			// ���[�v�Đ��J�n�ʒu�ݒ�
			void SetLoopBeginTime(f32 seconds)
			{
				currentAnimationLayer->currentState->animation->player->SetLoopBeginTime(seconds);
			}
		
			// ���[�v�Đ��J�n�ʒu�擾
			f32 GetLoopBeginTime()
			{
				return currentAnimationLayer->currentState->animation->player->GetLoopBeginTime();
			}
		
			// ���[�v�Đ��I���ʒu�ݒ�
			void SetLoopEndTime(f32 seconds)
			{
				currentAnimationLayer->currentState->animation->player->SetLoopEndTime(seconds);
			}
		
			// ���[�v�Đ��I���ʒu�擾
			f32 GetLoopEndTime()
			{
				return currentAnimationLayer->currentState->animation->player->GetLoopEndTime();
			}
		
			// �u�����h���[�g�̐ݒ�
			void SetBlendRate(f32 rate)
			{
				blendRate.x = rate;
				blendRate.y = 0.0f;
				blendRate.z = 0.0f;
			}
		
			void SetBlendRate(Phoenix::Math::Vector3 rate)
			{
				blendRate = rate;
			}

			void SetHipID(const Phoenix::s8* nodeName)
			{
				for (u32 i = 0; i < nodes->size(); ++i)
				{
					if (FND::StrCmp(nodes->at(i).name, nodeName) == 0)
					{
						hipNodeID = i;
						break;
					}
				}
			}
		
			// �A�j���[�V�����o���N�C���f�b�N�X�擾
			u32 GetAnimationBankIndex(const char* name) const
			{
				for (size_t i = 0; i < animations.size(); ++i)
				{
					if (0 == FND::StrCmp(name, OS::Path::GetFileNameWithoutExtension(animations.at(i).filename.c_str())))
					{
						return static_cast<u32>(i);
					}
				}
				return -1;
			}
		
			void NodeBlend(ModelObject::Node& currentNode, Math::Vector3& scale, Math::Quaternion& rotate, Math::Vector3& translate, Math::Vector3& blendScale, Math::Quaternion& blendRotate, Math::Vector3& blendTranslate, f32 blendRate, Graphics::IAnimationPlayer* player, s32 nodeID)
			{
				player->CalculateScale(nodeID, blendScale);
				player->CalculateRotate(nodeID, blendRotate);
				player->CalculateTranslate(nodeID, blendTranslate);
		
				currentNode.scale = Math::Vector3Lerp(scale, blendScale, fabsf(blendRate));
				currentNode.rotate = Math::QuaternionSlerp(rotate, blendRotate, fabsf(blendRate));
				currentNode.translate = Math::Vector3Lerp(translate, blendTranslate, fabsf(blendRate));
			}
		
			void CheckBlendOfEachAxis(ModelObject::Node& currentNode, Math::Vector3& scale, Math::Quaternion& rotate, Math::Vector3& translate, Math::Vector3& blendScale, Math::Quaternion& blendRotate, Math::Vector3& blendTranslate, f32 blendRate, f32 plot, Graphics::IAnimationPlayer* player, s32 nodeID)
			{
				if (!(fabsf(blendRate) <= Phoenix::Math::Epsilon))
				{
					if (0.0f < blendRate)
					{
						if (0.0f < plot)
						{
							NodeBlend(currentNode, scale, rotate, translate, blendScale, blendRotate, blendTranslate, blendRate, player, nodeID);
						}
					}
					else if (blendRate < 0.0f)
					{
						if (plot < 0.0f)
						{
							NodeBlend(currentNode, scale, rotate, translate, blendScale, blendRotate, blendTranslate, blendRate, player, nodeID);
						}
					}
				}
			}
		
			std::vector<ModelObject::Node> UpdateLayer(AnimationLayer* animationLayer, f32 elapsedTime)
			{
				std::vector<ModelObject::Node> nodes = *this->nodes;
				s32 animationNodeCount = static_cast<s32>(this->nodes->size());

				if (animationLayer->currentState)
				{
					std::unique_ptr<Graphics::IAnimationPlayer>& animationPlayer = animationLayer->currentState->animation->player;
					animationPlayer->Update(elapsedTime);

					for (s32 animationNodeID = 0; animationNodeID < animationNodeCount; ++animationNodeID)
					{
						s16 bindNodeID = animationLayer->currentState->animation->bindNodeIDs.at(animationNodeID);
						if (bindNodeID < 0) continue;

						ModelObject::Node& node = nodes.at(animationNodeID);
						ModelObject::Node& totalNode = nodes.at(animationNodeID);

						Math::Vector3 scale = node.scale;
						Math::Quaternion rotate = node.rotate;
						Math::Vector3 translate = node.translate;

						animationPlayer->CalculateScale(animationNodeID, scale);
						animationPlayer->CalculateRotate(animationNodeID, rotate);
						animationPlayer->CalculateTranslate(animationNodeID, translate);

						totalNode.scale = scale;
						totalNode.rotate = rotate;
						totalNode.translate = translate;
					}
				}
				else if (animationLayer->currentBlendTree)
				{
					for (auto& state : animationLayer->currentBlendTree->states)
					{
						std::unique_ptr<Graphics::IAnimationPlayer>& animationPlayer = state.animation->player;
						animationPlayer->Update(elapsedTime);

						for (s32 animationNodeID = 0; animationNodeID < animationNodeCount; ++animationNodeID)
						{
							s16 bindNodeID = state.animation->bindNodeIDs.at(animationNodeID);
							if (bindNodeID < 0) continue;

							ModelObject::Node& node = nodes.at(animationNodeID);
							ModelObject::Node& totalNode = nodes.at(animationNodeID);
							ModelObject::Node currentNodeX = nodes.at(animationNodeID);
							ModelObject::Node currentNodeY = nodes.at(animationNodeID);

							Math::Vector3 scale = node.scale;
							Math::Quaternion rotate = node.rotate;
							Math::Vector3 translate = node.translate;

							Math::Vector3 blendScale[] = { node.scale, node.scale };
							Math::Quaternion blendRotate[] = { node.rotate, node.rotate };
							Math::Vector3 blendTranslate[] = { node.translate, node.translate };

							if (blendRate == Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f))
							{
								totalNode.scale = scale;
								totalNode.rotate = rotate;
								totalNode.translate = translate;
							}
							else if (state.plot == Phoenix::Math::Vector3(0.0f, 0.0f, 0.0f))
							{
								animationPlayer->CalculateScale(animationNodeID, scale);
								animationPlayer->CalculateRotate(animationNodeID, rotate);
								animationPlayer->CalculateTranslate(animationNodeID, translate);

								totalNode.scale = scale;
								totalNode.rotate = rotate;
								totalNode.translate = translate;
							}
							else
							{
								CheckBlendOfEachAxis
								(
									currentNodeX,
									scale, rotate, translate,
									blendScale[0], blendRotate[0], blendTranslate[0],
									blendRate.x,
									state.plot.x,
									state.animation->player.get(),
									animationNodeID
								);
								CheckBlendOfEachAxis
								(
									currentNodeY,
									scale, rotate, translate,
									blendScale[1], blendRotate[1], blendTranslate[1],
									blendRate.y,
									state.plot.y,
									state.animation->player.get(),
									animationNodeID
								);

								totalNode.scale = Math::Vector3Lerp(currentNodeX.scale, currentNodeY.scale, fabsf(blendRate.y));				// ��ԗ���fabsf(blendRate.y)�����邱�Ƃ�
								totalNode.rotate = Math::QuaternionSlerp(currentNodeX.rotate, currentNodeY.rotate, fabsf(blendRate.y));			// �傫�����y���ɂ���currentNodeY���D�悳��āA
								totalNode.translate = Math::Vector3Lerp(currentNodeX.translate, currentNodeY.translate, fabsf(blendRate.y));	// ���������x���ɂ���currentNodeX���D�悳���B
							}
						}
					}
				}

				return nodes;
			}
		
			void Update(f32 elapsedTime)
			{
				if (currentAnimationLayer)
				{
					std::vector<ModelObject::Node> nodes = UpdateLayer(currentAnimationLayer, elapsedTime);
					s32 animationNodeCount = static_cast<s32>(this->nodes->size());
					for (s32 animationNodeID = 0; animationNodeID < animationNodeCount; ++animationNodeID)
					{
						if (currentAnimationLayer->currentState)
						{
							s16 bindNodeID = currentAnimationLayer->currentState->animation->bindNodeIDs.at(animationNodeID);
							if (bindNodeID < 0) continue;

							ModelObject::Node& node = this->nodes->at(animationNodeID);
							ModelObject::Node& currentNode = nodes.at(animationNodeID);

							Math::Vector3 scale = currentNode.scale;
							Math::Quaternion rotate = currentNode.rotate;
							Math::Vector3 translate = currentNode.translate;

							node.scale = scale;
							node.rotate = rotate;
							node.translate = translate;
						}
						else if (currentAnimationLayer->currentBlendTree)
						{
							bool contionue = false;
							for (auto& state : currentAnimationLayer->currentBlendTree->states)
							{
								s16 bindNodeID = state.animation->bindNodeIDs.at(animationNodeID);
								if (bindNodeID < 0) contionue = true;
							}
							if (contionue) continue;

							ModelObject::Node& node = this->nodes->at(animationNodeID);
							ModelObject::Node& currentNode = nodes.at(animationNodeID);

							Math::Vector3 scale = currentNode.scale;
							Math::Quaternion rotate = currentNode.rotate;
							Math::Vector3 translate = currentNode.translate;

							node.scale = scale;
							node.rotate = rotate;
							node.translate = translate;
						}
					}

					for (Phoenix::s32 layerCount = 0; layerCount < blendCurrentAnimationLayerSize; ++layerCount)
					{
						if (!blendCurrentAnimationLayer[layerCount]) continue;

						std::vector<ModelObject::Node> blendNodes = UpdateLayer(blendCurrentAnimationLayer[layerCount], elapsedTime);
						s32 animationNodeCount = static_cast<s32>(this->nodes->size());
						for (s32 animationNodeID = 0; animationNodeID < animationNodeCount; ++animationNodeID)
						{
							if (blendCurrentAnimationLayer[layerCount]->currentState)
							{
								s16 bindNodeID = blendCurrentAnimationLayer[layerCount]->currentState->animation->bindNodeIDs.at(animationNodeID);
								if (bindNodeID < 0) continue;

								ModelObject::Node& node = this->nodes->at(animationNodeID);
								ModelObject::Node& blendCurrentNode = blendNodes.at(animationNodeID);

								Math::Vector3 scale = node.scale;
								Math::Quaternion rotate = node.rotate;
								Math::Vector3 translate = node.translate;

								if (animationNodeID == hipNodeID || (blendCurrentAnimationLayer[layerCount]->beginNodeIndex <= animationNodeID && animationNodeID <= blendCurrentAnimationLayer[layerCount]->endNodeIndex))
								{
									scale = blendCurrentNode.scale;
									rotate = blendCurrentNode.rotate;
									translate = blendCurrentNode.translate;
								}

								node.scale = scale;
								node.rotate = rotate;
								node.translate = translate;
							}
							else if (blendCurrentAnimationLayer[layerCount]->currentBlendTree)
							{
								bool contionue = false;
								for (auto& state : blendCurrentAnimationLayer[layerCount]->currentBlendTree->states)
								{
									s16 bindNodeID = state.animation->bindNodeIDs.at(animationNodeID);
									if (bindNodeID < 0) contionue = true;
								}
								if (contionue) continue;

								ModelObject::Node& node = this->nodes->at(animationNodeID);
								ModelObject::Node& blendCurrentNode = blendNodes.at(animationNodeID);

								Math::Vector3 scale = node.scale;
								Math::Quaternion rotate = node.rotate;
								Math::Vector3 translate = node.translate;

								if (animationNodeID == hipNodeID || (blendCurrentAnimationLayer[layerCount]->beginNodeIndex <= animationNodeID && animationNodeID <= blendCurrentAnimationLayer[layerCount]->endNodeIndex))
								{
									scale = blendCurrentNode.scale;
									rotate = blendCurrentNode.rotate;
									translate = blendCurrentNode.translate;
								}

								node.scale = scale;
								node.rotate = rotate;
								node.translate = translate;
							}
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
#endif
}