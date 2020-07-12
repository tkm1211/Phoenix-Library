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
		void ModelObject::Load(Graphics::IGraphicsDevice* graphicsDevice, const char* filename)
		{
			const char* fullPass = OS::Path::GetFullPath(filename);

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

			for (u32 i = 0; i < materials.size(); ++i)
			{
				ModelObject::Material& material = materials.at(i);
				material.name = resourceMaterials.at(i).name;

				material.textures.resize(resourceMaterials.at(i).textureFilename.size());
				material.colors.resize(resourceMaterials.at(i).color.size());

				for (u32 j = 0; j < material.textures.size(); ++j)
				{
					material.colors.at(j) = resourceMaterials.at(i).color.at(j);
					material.textures.at(j) = Graphics::ITexture::Create();
					material.textures.at(j)->Initialize(graphicsDevice->GetDevice(), resourceMaterials.at(i).textureFilename.at(j).c_str(), static_cast<Graphics::MaterialType>(j), material.colors.at(j));
				}
			}

			animator = std::make_unique<Animator>();
			animator->Initialize(this);
			LoadAnimation(fullPass, -1);
		}

		// アニメーションの読み込み
		void ModelObject::LoadAnimation(const char* filename, s32 index)
		{
			animator->LoadResource(resourceManamger.get(), filename, index);
		}

		// 行列を更新
		void ModelObject::UpdateTransform(f32 elapsedTime)
		{
			UpdateAnimation(elapsedTime);
			UpdateLocalTransform();
			UpdateWorldTransform();
			UpdateBoneTransform();
		}

		// ローカル変換行列を更新
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

		// ワールド変換行列を更新
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

		// ボーン変換行列を更新
		void ModelObject::UpdateBoneTransform()
		{
			const Graphics::ModelData& modelData = GetModelResource()->GetModelData();

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
					meshNode.boneTransform.at(j) = Math::MatrixMultiply(modelData.meshes.at(i).offsetTransforms.at(j), nodes.at(modelData.meshes.at(i).nodeIndices.at(j)).worldTransform);
				}
				meshNode.boneTransformCount = meshNode.boneTransform.size();
			}
		}

		// アニメーションの再生
		void ModelObject::PlayAnimation(u32 bank, u32 clip, f32 fadeTime)
		{
			animator->Play(bank, clip, fadeTime);
		}

		// 一時停止/再開
		void ModelObject::PauseAnimation(bool pause)
		{
			animator->Pause(pause);
		}

		// 停止
		void ModelObject::StopAnimation()
		{
			animator->Stop();
		}

		// ループ再生設定
		void ModelObject::SetLoopAnimation(bool loop)
		{
			animator->SetLoop(loop);
		}

		// 再生中
		bool ModelObject::IsPlaying()
		{
			return animator->IsPlaying();
		}

		// ループ再生中か
		bool ModelObject::IsLoopAnimation()
		{
			return animator->IsLoop();
		}

		// アニメーションの更新
		void ModelObject::UpdateAnimation(f32 elapsedTime)
		{
			animator->Update(elapsedTime);
		}

		// 再生時間長(秒)取得
		f32 ModelObject::GetLength()
		{
			return animator->GetLength();
		}

		// 再生速度倍率設定
		void ModelObject::SetSpeed(f32 speed)
		{
			animator->SetSpeed(speed);
		}

		// 再生速度倍率取得
		f32 ModelObject::GetSpeed()
		{
			return animator->GetSpeed();
		}

		// ブレンド中か
		bool ModelObject::IsBlend()
		{
			return animator->IsBlend();
		}
	}
}