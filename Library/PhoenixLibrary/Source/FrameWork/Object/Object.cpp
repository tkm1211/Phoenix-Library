#include "pch.h"
#include "Phoenix/FrameWork/Object/Object.h"


namespace Phoenix
{
	namespace FrameWork
	{
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

			const std::vector<Graphics::ModelData::Material>& resourceMaterials = modelResource->GetModelData().materials;
			materials.resize(resourceMaterials.size());

			for (int i = 0; i < materials.size(); ++i)
			{
				materials.at(i).texture = Graphics::ITexture::Create();
				materials.at(i).texture->Initialize(graphicsDevice->GetDevice(), resourceMaterials.at(i).textureFilename.c_str());
			}

			animator = std::make_unique<Animator>();
			animator->Initialize(this);
			LoadAnimation(filename, -1);
		}

		// �A�j���[�V�����̓ǂݍ���
		void ModelObject::LoadAnimation(const char* filename, s32 index)
		{
			animator->LoadResource(resourceManamger.get(), filename, index);
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
			for (u32 i = 0; i < nodes.size(); ++i)
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
			for (u32 i = 0; i < nodes.size(); ++i)
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
			Graphics::IModelResource* modelResource = GetModelResource();
			const Graphics::ModelData& modelData = modelResource->GetModelData();

			if (meshNodes.size() < modelData.meshes.size())
			{
				meshNodes.resize(modelData.meshes.size());
			}

			for (u32 i = 0; i < meshNodes.size(); ++i)
			{
				MeshNode& meshNode = meshNodes.at(i);

				if (meshNode.boneTransform.size() < modelData.meshes.at(i).nodeIndices.size())
				{
					meshNode.boneTransform.resize(modelData.meshes.at(i).nodeIndices.size());
				}

				for (u32 j = 0; j < meshNode.boneTransform.size(); ++j)
				{
					const Node& node = nodes.at(modelData.meshes.at(i).nodeIndices.at(j));
					meshNode.boneTransform.at(j) = Math::MatrixMultiply(modelData.meshes.at(i).offsetTransforms.at(j), node.worldTransform);
				}
				meshNode.boneTransformCount = meshNode.boneTransform.size();
			}
		}

		// �A�j���[�V�����̍Đ�
		void ModelObject::PlayAnimation(u32 bank, u32 clip, f32 fadeTime)
		{
			animator->Play(bank, clip, fadeTime);
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

		// ���[�v�Đ���
		bool ModelObject::IsLoopAnimation()
		{
			return animator->IsLoop();
		}

		// �A�j���[�V�����̍X�V
		void ModelObject::UpdateAnimation(f32 elapsedTime)
		{
			animator->Update(elapsedTime);
		}
	}
}