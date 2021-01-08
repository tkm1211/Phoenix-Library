#include "pch.h"
#include "Phoenix/FrameWork/Object/Object.h"


namespace Phoenix
{
	namespace FrameWork
	{
		// ����
		std::unique_ptr<ModelObject> ModelObject::Create()
		{
			return std::make_unique<ModelObject>();
		}

		// ������
		void ModelObject::Initialize(Graphics::IGraphicsDevice* graphicsDevice)
		{
			resourceManamger = Phoenix::OS::IResourceManager::Create();
			resourceManamger->Initialize(nullptr);
			resourceManamger->RegisterFactory("ani", Phoenix::Graphics::IAnimationResourceFactory::Create());
			resourceManamger->RegisterFactory("mdl", Phoenix::Graphics::IModelResourceFactory::Create(graphicsDevice->GetDevice()));

			file = OS::IFileStream::Create();
			file->Initialize(nullptr);
		}

		// ���f���̓ǂݍ���
		void ModelObject::Load(Graphics::IGraphicsDevice* graphicsDevice, const char* filename)
		{
			const char* fullPass = OS::Path::GetFullPath(filename);
			std::string animFullPass = std::string(fullPass);

			std::string modelFilename;
			modelFilename = OS::Path::ChangeFileExtension(fullPass, "mdl");

			if (OS::Path::CheckFileExtension(fullPass, "fbx") && !file->Exists(modelFilename.c_str()))
			{
				std::unique_ptr<Loader::ILoader> loader = Loader::ILoader::Create();
				if (!loader->Initialize(fullPass))
				{
					return;
				}
				Graphics::ModelData data;
				if (!loader->Load(data, OS::Path::GetDirectoryName(filename)))
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

			const std::vector<Graphics::ModelData::Node>& resourceNodes = modelResource->GetModelData().nodes;
			nodes.resize(resourceNodes.size());

			for (sizeT nodeIndex = 0; nodeIndex < nodes.size(); nodeIndex++)
			{
				auto&& src = resourceNodes.at(nodeIndex);
				auto&& dst = nodes.at(nodeIndex);

				dst.name = src.name.c_str();
				dst.parent = src.parentIndex >= 0 ? &nodes.at(src.parentIndex) : nullptr;
				dst.scale = src.scale;
				dst.rotate = src.rotate;
				dst.translate = src.translate;
			}

			const std::vector<Graphics::ModelData::Material>& resourceMaterials = modelResource->GetModelData().materials;
			materials.resize(resourceMaterials.size());

			for (sizeT i = 0; i < materials.size(); ++i)
			{
				ModelObject::Material& material = materials.at(i);
				material.name = resourceMaterials.at(i).name;

				material.textures.resize(resourceMaterials.at(i).textureFilename.size());
				material.colors.resize(resourceMaterials.at(i).color.size());

				for (sizeT j = 0; j < material.textures.size(); ++j)
				{
					material.colors.at(j) = resourceMaterials.at(i).color.at(j);
					material.textures.at(j) = Graphics::ITexture::Create();
					material.textures.at(j)->Initialize(graphicsDevice->GetDevice(), resourceMaterials.at(i).textureFilename.at(j).c_str(), static_cast<Graphics::MaterialType>(j), material.colors.at(j));
				}
			}

			boneNames.resize(nodes.size());
			for (sizeT i = 0; i < boneNames.size(); ++i)
			{
				boneNames.at(i) = nodes.at(i).name;
			}

			animator = std::make_unique<Animator>();
			animator->Initialize(this);
			LoadAnimation(animFullPass.c_str(), -1);
		}

		// �A�j���[�V�����̓ǂݍ���
		s32 ModelObject::LoadAnimation(const char* filename, s32 index)
		{
			return animator->LoadResource(resourceManamger.get(), filename, index);
		}

		s32 ModelObject::AddAnimationLayer(const s8* beginNodeName, const s8* endNodeName)
		{
			return animator->AddLayer(beginNodeName, endNodeName);
		}

		s32 ModelObject::AddAnimationLayer(s32 beginNodeIndex, s32 endNodeIndex)
		{
			return animator->AddLayer(beginNodeIndex, endNodeIndex);
		}

		s32 ModelObject::AddAnimationStateToLayer(s32 addAnimationIndex, s32 animationLayerIndex)
		{
			return animator->AddAnimationStateToLayer(addAnimationIndex, animationLayerIndex);
		}

		s32 ModelObject::AddBlendTreeToLayer(s32 animationLayerIndex)
		{
			return animator->AddBlendTreeToLayer(animationLayerIndex);
		}

		s32 ModelObject::AddBlendAnimationStateToBlendTree(s32 addAnimationIndex, Phoenix::Math::Vector3 plot, s32 animationLayerIndex, s32 blendTreeIndex)
		{
			return animator->AddBlendAnimationStateToBlendTree(addAnimationIndex, plot, animationLayerIndex, blendTreeIndex);
		}

		// �s����X�V
		void ModelObject::UpdateTransform(f32 elapsedTime)
		{
			UpdateAnimation(elapsedTime);
			UpdateLocalTransform();
			UpdateWorldTransform();
			UpdateBoneTransform();
		}

		// ���[�J���ϊ��s����X�V
		void ModelObject::UpdateLocalTransform()
		{
			for (sizeT i = 0; i < nodes.size(); ++i)
			{
				Math::Matrix scale, rotate, translate;
				scale = Math::MatrixScaling(nodes[i].scale.x, nodes[i].scale.y, nodes[i].scale.z);
				rotate = Math::MatrixRotationQuaternion(&nodes[i].rotate);
				translate = Math::MatrixTranslation(nodes[i].translate.x, nodes[i].translate.y, nodes[i].translate.z);

				nodes[i].localTransform = scale * rotate * translate;
			}
		}

		// ���[���h�ϊ��s����X�V
		void ModelObject::UpdateWorldTransform()
		{
			for (sizeT i = 0; i < nodes.size(); ++i)
			{
				if (nodes[i].parent != nullptr)
				{
					nodes[i].worldTransform = nodes[i].localTransform * nodes[i].parent->worldTransform;
				}
				else
				{
					nodes[i].worldTransform = nodes[i].localTransform;
				}
			}
		}

		// �{�[���ϊ��s����X�V
		void ModelObject::UpdateBoneTransform()
		{
			const Graphics::ModelData& modelData = GetModelResource()->GetModelData();

			if (meshNodes.size() < modelData.meshes.size())
			{
				meshNodes.resize(modelData.meshes.size());
			}

			for (sizeT i = 0; i < meshNodes.size(); ++i)
			{
				MeshNode& meshNode = meshNodes.at(i);

				if (meshNode.boneTransform.size() < modelData.meshes.at(i).nodeIndices.size())
				{
					meshNode.boneTransform.resize(modelData.meshes.at(i).nodeIndices.size());
				}

				for (sizeT j = 0; j < meshNode.boneTransform.size(); ++j)
				{
					meshNode.boneTransform.at(j) = Math::MatrixMultiply(modelData.meshes.at(i).offsetTransforms.at(j), nodes.at(modelData.meshes.at(i).nodeIndices.at(j)).worldTransform);
				}
				meshNode.boneTransformCount = static_cast<u32>(meshNode.boneTransform.size());
			}
		}

		// �A�j���[�V�����̍Đ�
		void ModelObject::PlayAnimation(u32 layerIndex, u32 stateIndex, u32 clip, f32 fadeTime)
		{
			animator->Play(layerIndex, stateIndex, clip, fadeTime);
		}

		// �u�����h�c���[�̃A�j���[�V�����Đ�
		void ModelObject::PlayBlendTreeAnimation(u32 layerIndex, u32 blendTreeIndex, u32 clip, f32 fadeTime)
		{
			animator->PlayBlendTree(layerIndex, blendTreeIndex, clip, fadeTime);
		}

		// �A�j���[�V�����̓����Đ�
		void ModelObject::SimultaneousPlayAnimation(u32 layerIndex, u32 blendTreeIndex, u32 clip, f32 fadeTime)
		{
			animator->BlendPlay(layerIndex, blendTreeIndex, clip, fadeTime);
		}

		// �u�����h�c���[�̃A�j���[�V���������Đ�
		void ModelObject::SimultaneousPlayBlendTreeAniamation(u32 layerIndex, u32 blendTreeIndex, u32 clip, f32 fadeTime)
		{
			animator->BlendPlayBlendTree(layerIndex, blendTreeIndex, clip, fadeTime);
		}

		// �ꎞ��~/�ĊJ
		void ModelObject::PauseAnimation(bool pause)
		{
			animator->Pause(pause);
		}

		// ��~
		void ModelObject::StopAnimation()
		{
			animator->Stop();
		}

		// ���[�v�Đ��ݒ�
		void ModelObject::SetLoopAnimation(bool loop)
		{
			animator->SetLoop(loop);
		}

		// ���[�v�����Đ��ݒ�
		void ModelObject::SetBlendLoopAnimation(bool loop)
		{
			animator->SetBlendLoop(loop);
		}

		// �Đ���
		bool ModelObject::IsPlaying()
		{
			return animator->IsPlaying();
		}

		// ���[�v�Đ�����
		bool ModelObject::IsLoopAnimation()
		{
			return animator->IsLoop();
		}

		// �A�j���[�V�����̍X�V
		void ModelObject::UpdateAnimation(f32 elapsedTime)
		{
			animator->Update(elapsedTime);
		}

		// �Đ����Ԓ�(�b)�擾
		f32 ModelObject::GetLength()
		{
			return animator->GetLength();
		}

		// �Đ����x�{���ݒ�
		void ModelObject::SetSpeed(f32 speed)
		{
			animator->SetSpeed(speed);
		}

		// �Đ����x�{���擾
		f32 ModelObject::GetSpeed()
		{
			return animator->GetSpeed();
		}

		// �u�����h����
		bool ModelObject::IsBlend()
		{
			return animator->IsBlend();
		}

		// �Đ��ʒu�ݒ�
		void ModelObject::SetCurrentTime(f32 seconds)
		{
			animator->SetCurrentTime(seconds);
		}

		// �Đ��ʒu�擾
		f32 ModelObject::GetCurrentTime()
		{
			return animator->GetCurrentTime();
		}

		// �c��Đ����Ԏ擾
		f32 ModelObject::GetLastTime()
		{
			return animator->GetLastTime();
		}

		// �Đ��J�n�ʒu�ݒ�
		void ModelObject::SetBeginTime(f32 seconds)
		{
			animator->SetBeginTime(seconds);
		}

		// �Đ��J�n�ʒu�擾
		f32 ModelObject::GetBeginTime()
		{
			return animator->GetBeginTime();
		}

		// �Đ��I���ʒu�ݒ�
		void ModelObject::SetEndTime(f32 seconds)
		{
			animator->SetEndTime(seconds);
		}

		// �Đ��I���ʒu�擾
		f32 ModelObject::GetEndTime()
		{
			return animator->GetEndTime();
		}

		// ���[�v�Đ��J�n�ʒu�ݒ�
		void ModelObject::SetLoopBeginTime(f32 seconds)
		{
			animator->SetLoopBeginTime(seconds);
		}

		// ���[�v�Đ��J�n�ʒu�擾
		f32 ModelObject::GetLoopBeginTime()
		{
			return animator->GetLoopBeginTime();
		}

		// ���[�v�Đ��I���ʒu�ݒ�
		void ModelObject::SetLoopEndTime(f32 seconds)
		{
			animator->SetLoopEndTime(seconds);
		}

		// ���[�v�Đ��I���ʒu�擾
		f32 ModelObject::GetLoopEndTime()
		{
			return animator->GetLoopEndTime();
		}

		// �u�����h���[�g�̐ݒ�
		void ModelObject::SetBlendRate(f32 rate)
		{
			animator->SetBlendRate(rate);
		}
		void ModelObject::SetBlendRate(Phoenix::Math::Vector3 rate)
		{
			animator->SetBlendRate(rate);
		}

		void ModelObject::SetHipID(const Phoenix::s8* nodeName)
		{
			animator->SetHipID(nodeName);
		}
	}
}