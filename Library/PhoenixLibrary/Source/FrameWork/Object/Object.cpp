#include "pch.h"
#include "Phoenix/FrameWork/Object/Object.h"


namespace Phoenix
{
	namespace FrameWork
	{
		// 初期化
		void ModelObject::Initialize(Graphics::IGraphicsDevice* graphicsDevice)
		{
			resourceManamger = Phoenix::OS::IResourceManager::Create();
			resourceManamger->Initialize(nullptr);
			resourceManamger->RegisterFactory("ani", Phoenix::Graphics::IAnimationResourceFactory::Create());
			resourceManamger->RegisterFactory("mdl", Phoenix::Graphics::IModelResourceFactory::Create(graphicsDevice->GetDevice()));

			file = OS::IFileStream::Create();
			file->Initialize(nullptr);
		}

		// モデルの読み込み
		void ModelObject::Load(const char* filename)
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

			animator = std::make_unique<Animator>();
			animator->Initialize(this);
			LoadAnimation(filename, -1);
		}

		// アニメーションの読み込み
		void ModelObject::LoadAnimation(const char* filename, s32 index)
		{
			animator->LoadResource(resourceManamger.get(), filename, index);
		}

		// ローカル変換行列計算
		void ModelObject::CalculateLocalTransform()
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

		// ワールド変換行列計算
		void ModelObject::CalculateWorldTransform(const Math::Matrix& worldTransform)
		{
			if (boneTransform.size() < nodes.size()) boneTransform.resize(nodes.size());

			int i = 0;
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

				boneTransform[i] = node.worldTransform;
				i++;
			}

			boneTransformCount = boneTransform.size();
		}

		// アニメーションの再生
		void ModelObject::PlayAnimation(u32 bank, u32 clip, f32 fadeTime)
		{
			animator->Play(bank, clip, fadeTime);
		}

		// アニメーションの更新
		void ModelObject::UpdateAnimation(f32 elapsedTime)
		{
			animator->Update(elapsedTime);
		}
	}
}